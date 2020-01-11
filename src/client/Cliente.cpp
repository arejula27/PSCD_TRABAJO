//*****************************************************************
// File:   Cliente.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de cliente con comunicación síncrona mediante sockets
//
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include "Socket.hpp"
#include "PoblacionActual.hpp"

using namespace std;

const int MESSAGE_SIZE = 100000; //mensajes de no más 10000 caracteres
const int MAX_SERVERS = 3; //El numero de servidores maximo que tenemos que lanzar

void leerconfig(int &numServers,int &puertoCs, int &gen, int &puerto, int &numCiudades, string IPs[], int &numPersonas, string &fDatos){
    string buffer;
    ifstream f;
    f.open("cliente.config");
    if(f.is_open()){
        while(!f.eof() && getline(f,buffer)){
            if(buffer.find("Puerto:")==0){
                puerto = stoi(&buffer[strlen("Puerto:")]);
                cout <<"Puerto = "<<puerto<<endl;
            }
            else if(buffer.find("PuertoCs:") == 0)
            {
                puertoCs = stoi(&buffer[strlen("PuertoCs:")]);
                cout << "Puerto = " << puerto << endl;
            }

            else if(buffer.find("numCiudades:")==0){
                numCiudades = stoi(&buffer[strlen("numCiudades:")]);
                cout <<"numCiudades = "<<numCiudades<<endl;
            }
            else if (buffer.find("generaciones:") == 0)
            {
                gen = stoi(&buffer[strlen("generaciones:")]);
                cout << "Generaciones= " << gen << endl;
            }
            else if(buffer.find("SERVERS:{")==0){
                numServers=0;
                getline(f,buffer);
                for(int i=0 ; buffer != "}" ; i++){
                    IPs[i] = buffer;
                    numServers++;
                    cout <<"IP "<<i+1<<" = "<<IPs[i]<<endl;
                    getline(f,buffer);
                }
            }
            else if(buffer.find("numPersonas:")==0){
                numPersonas = stoi(&buffer[strlen("numPersonas:")]);
                cout <<"numPersonas = "<<numPersonas<<endl;
            }
            else if(buffer.find("Fichero de datos:")==0){
                fDatos = (&buffer[strlen("Fichero de datos:")]);
                cout <<"Fichero de datos: = "<<fDatos<<endl;
            }
            
        }
    }
    else{
        cout<<"No se ha podido abrir config"<<endl;
        exit(1);
    }

    f.close();
}


/*void calcEstadisticas(Poblacion& personas,int ID,PobActual &pa,float &mejorFit,float &media){
    pa.syncro(ID);
    #warning darle valor a fit para calcular el % IGUAL VAR GLOBAL
    float fit;
    float porcentaje = personas.stats(fit,mejorFit,media);
    pa.finProceso(ID);
}*/
//He comentado esta funcion porque utilizaba dos funciones del monitor que gestionaban la 
//concurrencia de los hilos pero como ya no lanzamos hilos es una gilipollez segur teniendola 
// solo con invocar a stats en imprimirCSV es mejor.
//Fdo: Aaron

void imprimirCSV (Poblacion &personas,PobActual &pa){
    ofstream f("salida.csv");

    pa.esperaGA();
    f << "ID poblacion" << "," << "Mejor Fitness" << "," << "Fitness Medio" << endl;
    int i = 0;
    float mejorFit,media;
    while (i< MAX_GENS){
        float porcentaje = personas.stats(0.8,mejorFit,media);
        //calcEstadisticas(personas,i+1,pa,mejorFit,media);
        f << i+1 << "," << mejorFit << "," << media << endl;
        string info = to_string(i+1) + " , " + to_string(mejorFit) + " , " + to_string(media); 
        pa.guardarDatos(info);
        pa.esperaGA();
        i++;
    }
    f.close();
}

void servCliente(Socket &soc, int client_fd, PobActual &pa, Poblacion &personas){
    const string MENS_FIN("END OF SERVICE");// mensaje de fin de conexión
    const string CABECERA("ID Poblacion , Mejor Fitness , Fitness Medio Poblacion");
    string msg; 
    int bufflen = 512;
    int send_bytes;
    int rcv_bytes;
    bool acabado = false;
    bool salir = false;
    //codigo
    rcv_bytes = soc.Recv(client_fd, msg, bufflen); //recibimos la peticion de entrada
    send_bytes = soc.Send(client_fd, CABECERA);
    int i = 0;
    while(i < MAX_GENS && !pa.finEjec(personas)){
        pa.dormirSer();
        pa.extraerDatos(msg);
        send_bytes = soc.Send(client_fd, msg);
        i++;
    }
    string FIN = ("Se han acabado las mutaciones");
    send_bytes = soc.Send(client_fd, FIN);
    soc.Close(client_fd);//cerramos la conexión
    pa.endPr();//indicamos que hemos acabado

}

void finalizador(PobActual &pa,int max,int port){
    const string MENS_FIN("FIN_SERV");
    pa.dormir();//bloqueamos el proceso hasta que sea requerido
    
    if(pa.maxCli() < max) //si es necesario se desbloque el accept
    {
        
        //este proceso se lanza en el propio server
        string SERVER_ADDRESS = "localhost";
        // el puerto es el mismo que el del servidor
        int SERVER_PORT = port;

        // Creación del socket con el que se llevará a cabo
        // la comunicación con el servidor.
        Socket socket(SERVER_ADDRESS, SERVER_PORT);

        const int MAX_ATTEMPS = 10;
        int count = 0;
        int socket_fd;
        do
        {
            // Conexión con el servidor
            socket_fd = socket.Connect();
            count++;

            // Si error --> esperamos 1 segundo para reconectar
            if (socket_fd == -1)
            {
                this_thread::sleep_for(chrono::seconds(1));
            }
        } while (socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if (socket_fd == -1)
    {
        exit(1);
    }

    int send_bytes = socket.Send(socket_fd,MENS_FIN );
    if (send_bytes == -1)
    {
        cerr << "Error al mandar datos: " + string(strerror(errno)) + "\n";
        // Cerramos los sockets
        socket.Close(socket_fd);
        exit(1);
    }
    socket.Close(socket_fd);// cerramos el socket

    }
    cout<<"Servidor finalizado correctamente, ";
}

void controlEstadistico(Poblacion& personas,PobActual &pa,int puertoCS){
    cout<<"CONTROL ESTADISTICO\n";
    thread impr(&imprimirCSV,ref(personas),ref(pa));
    
    const int max_connections = 20;
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = puertoCS; //normalmente será un parámetro de invocación
    thread cliente[max_connections];
    int client_fd[max_connections];
    int clientes_conect = 0;
    
    thread finalizar (&finalizador,ref(pa),max_connections,SERVER_PORT);

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket socket(SERVER_PORT);

    // Bind
    int socket_fd = socket.Bind();
    if (socket_fd == -1)
    {
        cerr << "Error en el bind: " + string(strerror(errno)) + "\n";
        exit(1);
    }

    // Listen
    int error_code = socket.Listen(max_connections);
    if (error_code == -1)
    {
        cerr << "Error en el listen: " + string(strerror(errno)) + "\n";
        // Cerramos el socket
        socket.Close(socket_fd);
        exit(1);
    }
    cout<<"Server iniciado en el puerto: "<<SERVER_PORT<<endl;
    cout<<"Esperando clientes... "<<endl;


    //mientras queden conexiones por hacer y no esta 
    for (int i = 0; i < max_connections && !pa.finEjec(personas); i++)
    {
        // Accept
        //cout << "dale" << endl;
        client_fd[i] = socket.Accept();

        if (client_fd[i] == -1)
        {
            cerr << "Error en el accept: " + string(strerror(errno)) + "\n";
            // Cerramos el socket
            socket.Close(socket_fd);
            exit(1);
        }

        cliente[i] = thread(&servCliente, ref(socket), client_fd[i],ref(pa),ref(personas));
        if(!pa.finEjec(personas))
        {
            cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
        } 
        pa.nuevoCl();
        
    }

    //¿Qué pasa si algún thread acaba inesperadamente?
    for (int i = 0; i < pa.maxCli(); i++)
    {
        cliente[i].join();
    }

    finalizar.join();
    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if (error_code == -1)
    {
        cerr << "Error cerrando el socket del servidor: " + string(strerror(errno)) + " aceptado" + "\n";
    }
}


void controlGenetico(int numServers, int puerto, Poblacion &personas, PobActual &pa, string IPs[],int gen){
	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socketServ[numServers];
	for (int i = 0; i<numServers; i++){
		socketServ[i].ini(IPs[i],puerto);
	}
	int server_fd[numServers];
	// Conectamos con el servidor. Probamos varias conexiones
	int serversAceptados = 0;
	for (int i = 0; i<numServers; i++){
		const int MAX_ATTEMPS = 10;
		int count = 0;
		do {
			// Conexión con el servidor
			server_fd[i] = socketServ[i].Connect();
			count++;

			// Si error --> esperamos 1 segundo para reconectar
			if(server_fd[i] == -1){
				this_thread::sleep_for(chrono::seconds(1));
			}
		} while(server_fd[i] == -1 && count < MAX_ATTEMPS);
		// Chequeamos si se ha realizado la conexión
    	if(server_fd[i] == -1){
    	cerr <<  "No conectado a server: " << server_fd << endl;
    	}else{
			serversAceptados++;
		}
	}
	if(serversAceptados==0){
		cerr<<"No se ha logrado conectar con ningún server"<<endl;
		exit(0);
	}
	cout <<"Servidores aceptados:"<< serversAceptados << endl;
	Poblacion pobs[serversAceptados];
	/*
	for (int i = 0; i < serversAceptados; i++){
		pobs[i].getMatrixFrom(personas);
	}
	*/
	for (int i = 0; i < gen && !pa.finEjec(personas); i++){
		cout <<"Generación: "<< (i+1) << endl;
		personas.dividir(serversAceptados,pobs);
		for (int j = 0; j < 3; j++){
			// 0 cruzar
			// 1 mutar
			// 2 seleccionar
			for(int k = 0; k < serversAceptados; k++){
               // cout << pobs[i].getNumCam() <<" ?22" <<endl;

                string msg;
               if(i==0&&j==0){
                    msg = to_string(j) + "," + to_string(personas.getNumCities()) + ":" + personas.codificarMatriz() + pobs[k].codificar(UPGRADE_POB);
               }
               else{
                   msg = to_string(j) + ","  + pobs[k].codificar(UPGRADE_POB);
               }
				socketServ[k].Send(server_fd[k],msg);
				cout << "Mensaje enviado a servidor, generación: "<<i+1<< endl;
                //cout<<msg<<endl;
			}
			for(int k = 0; k < serversAceptados; k++){
				string resp;
				socketServ[k].Recv(server_fd[k],resp,MESSAGE_SIZE);
				cout << "Mensaje recibido del servidor, generación: " << i + 1 << endl;
                //cout<<resp<<endl;
				pobs[k].descodificar(resp,UPGRADE_POB);

                
            }

		}
		personas.fusionar(serversAceptados,pobs);
        pa.esperaEstadistico();
	}
	//mando mensaje de finalizacion
	for (int i = 0; i < serversAceptados; i++){
		#warning cambiar msg de finalizacion
		string fin = "END OF SERVICE";
		socketServ[i].Send(server_fd[i],fin);
	}

    // Cerramos el socket
    for (int i = 0; i < serversAceptados; i++){
        int error_code = socketServ[i].Close(server_fd[i]);
        if(error_code == -1){
            cerr << "Error cerrando el socket: " << strerror(errno) << endl;
        }
    }
    pa.despertarTodos();
}

int main(int argc, char const *argv[]){


    const string MENS_FIN("END OF SERVICE");

    int puertoServer;
    int numPersonas;
    int cities;
    int numServers;
    int puertoCS; //puerto del cliente que actua como server 
    string fichero;
    string IPs[MAX_SERVERS];
    
    
    #warning la ciudad a inicial se puede cambiar
    leerconfig(numServers,puertoCs,gen,puertoServer, cities, IPs, numPersonas, fichero);
    PobActual pa(gen);
    Poblacion proletariado(numPersonas,3,cities,fichero);

    thread estadistico (&controlEstadistico,ref(proletariado),ref(pa),puertoCS);
    thread GAcontrol (&controlGenetico,numServers, puertoServer,ref(proletariado),ref(pa), IPs);
    estadistico.join();
    GAcontrol.join();
    cout<<"FIN DE SERVICIO, MIRAR salida.csv PARA VER EL HISTÓRICO"<<endl;
    return 0;
}
