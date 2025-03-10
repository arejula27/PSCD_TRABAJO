//*****************************************************************
// File:   Cliente.cpp
// Author: Íñigo Aréjula, Sergio Cartiel, Aaron Ibáñez, Héctor García y Marcos Hernando
// Date:   Noviembre-Enero 2019
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
const int MAX_MSG_SIZE = 10000;     //Máximo tamaño de los paquetes enviados.
const string FIN_MSG_RECIBIDO = "*";    //Caracter usado para detectar el último paquete que se envía.

//Actualiza los parámetros de salida con los datos especificados en el fichero cliente.config
void leerconfig(int &numServers,int &puertoCs, int &gen, int &puerto, string IPs[], int &numPersonas, int ops[]){
    string buffer;
    ifstream f;
    f.open("cliente.config");
    if(f.is_open()){
        while(!f.eof() && getline(f,buffer)){
            if(buffer.find("Puerto:")==0){
                puerto = stoi(&buffer[strlen("Puerto:")]);
                cout<<"Puerto = "<<puerto<<endl;
            }
            else if(buffer.find("PuertoCs:") == 0)
            {
                puertoCs = stoi(&buffer[strlen("PuertoCs:")]);
                cout << "Puerto = " << puerto << endl;
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
                    cout<<"IP "<<i+1<<" = "<<IPs[i]<<endl;
                    getline(f,buffer);
                }
            }
            else if(buffer.find("numPersonas:")==0){
                numPersonas = stoi(&buffer[strlen("numPersonas:")]);
                cout <<"numPersonas = "<<numPersonas<<endl;
            }
           
            else if (buffer.find("Cruzar:") == 0)
            {
                string buff = (&buffer[strlen("Cruzar:")]);
                cout << "Versión cruzar:  " << buff << endl;
                if(buff=="v2"){

                }
                else{
                    ops[0] = 0;
                }
            }
            else if (buffer.find("Mutar:") == 0)
            {
                string buff = (&buffer[strlen("Mutar:")]);
                cout << "Versión mutar:  " << buff << endl;
                if (buff == "v2")
                {
                    ops[1] = 4;
                }
                else
                {
                    ops[1] = 1;
                }
            }
            else if (buffer.find("Seleccionar:") == 0)
            {
                string buff = (&buffer[strlen("Seleccionar:")]);
                cout << "Versión seleccionar:  " << buff<< endl;
                if (buff == "v2")
                {
                    ops[2] = 5;
                }
                else
                {
                    ops[2] = 2;
                }
            }
        }
    }
    else{
        cout<<"No se ha podido abrir config"<<endl;
        exit(1);
    }

    f.close();
}

//Cada vez que llega una actualizacion de poblacion,es decir llega una poblacion tras haberle hecho una 
// operacion geneticacalcula sus estadicticas, id de la poblacion, el mejor fitness de la poblacion y el 
//fitness medio de la misma
void imprimirCSV (Poblacion &personas,PobActual &pa, int MAX_GENS){
    ofstream f("salida.csv");
    pa.esperaGA();
    f << "ID poblacion" << ";" << "Mejor Fitness" << ";" << "Fitness Medio" << endl;
    int i = 0;
    float mejorFit,media;
    while (i<MAX_GENS){
        float porcentaje = personas.stats(0.8,mejorFit,media);
        f << i+1 << ";" << mejorFit << ";" << media << endl;
        string info = to_string(i+1) + " ; " + to_string(mejorFit) + " ; " + to_string(media); 
        pa.guardarDatos(info);

        pa.esperaGA();
        i++;
    }
    f.close();
}


void calcEstadisticas(Poblacion& personas,int ID,PobActual &pa,float &mejorFit,float &media){
    pa.syncro(ID);
    float fit;
    float porcentaje = personas.stats(fit,mejorFit,media);
    pa.finProceso(ID);
}



//Función que crea se conecta a servidores para solicitarles las operaciones seleccionar/cruzar/mutar en ese orden.
void controlGenetico(int numServers, int puerto, Poblacion &personas, PobActual &pa, string IPs[],int gen,int ops[]){
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
    int tamMsg, numPaquetes, tamLastPaquete, iterMsg;
    string msgPartido;

    bool prim = true;
	for (int i = 0; i < gen && !pa.finEjec(personas); i++){
		cout <<"Generación: "<< (i+1) << endl;
		personas.dividir(serversAceptados,pobs);
		for (int j = 0; j < 3; j++){
            

            // 0 cruzar
			// 1 mutar
			// 2 seleccionar
            if(j==2 ){
                for(int t = 0; t < serversAceptados; t++){
                    
                    pobs[t].calcFit();
                    
                } 
            }
			for(int k = 0; k < serversAceptados; k++){
                if(prim){
                  
                    string nc = personas.codificar(NCIT);
                    socketServ[k].Send(server_fd[k],nc);        //Envía el número de ciudades.
                }
                string msg;
              
                msg = to_string(ops[j]) + "," + pobs[k].codificar(UPGRADE_POB) + "*";
                
                tamMsg = msg.length();
                numPaquetes = tamMsg / MAX_MSG_SIZE;
                tamLastPaquete = tamMsg % MAX_MSG_SIZE;
                iterMsg = 0;
		cout << "Enviando mensaje a servidor("<<k<<"), operacion("<<j<<") generación: "<<i+1<< endl;
                while(numPaquetes >= 0){
                    msgPartido = &msg[iterMsg];
                    msgPartido.resize(MAX_MSG_SIZE);                //Envío por paquetes del mensaje.
                    iterMsg += msgPartido.length();
                    socketServ[k].Send(server_fd[k],msgPartido);
                    numPaquetes--;
                }
		cout<< "Enviado"<<endl;

            }
            prim = false;
            string aux;
	    
            for(int k = 0; k < serversAceptados; k++){
		string resp = "";
                string buffer;
		cout << "Recibiendo mensaje del servidor(" << k << "),  operacion (" << j << ") generación: " << i + 1 << endl;
		        do{
			        buffer = "";
			        int rcv_bytes = socketServ[k].Recv(server_fd[k],buffer,MESSAGE_SIZE);
			        resp += buffer;
			        aux = buffer.back();                        //Recibo del mensaje dividido en paquetes.
		        }while(buffer.find("*")==string::npos);
                cout<<"Recibido"<<endl;
                cout <<"Procesando nueva población..."<< endl;
                pobs[k].descodificar(&resp[resp.find_first_of("0123456789")],UPGRADE_POB);
            }

		}

		personas.fusionar(serversAceptados,pobs);
        pa.esperaEstadistico();
        
	}
	//mando mensaje de finalizacion
	for (int i = 0; i < serversAceptados; i++){
		string fin = "END OF SERVICE*";
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


//Servidor para clienteB (estadístico)
void servCliente(Socket& soc, int client_fd, PobActual &pa) {
	// Buffer para recibir el mensaje
    string buffer;
	int send_bytes;
	bool aux = true;
	const string MENS_FIN("END OF SERVICE");
	//Aumentamos el número de clientes conectados
	pa.nuevoCliente();
	bool res;
	while(!pa.finGA()) {
        pa.dormirServidor();
        pa.extraerDatos(buffer);
		//Enviamos mensaje de bienvenida
		send_bytes = soc.Send(client_fd, buffer);
		if(send_bytes == -1) {
			cerr << "Error al enviar datos: " + string(strerror(errno)) + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
			exit(1);
		}
	}
	send_bytes = soc.Send(client_fd, MENS_FIN);
	pa.finCliente();
	soc.Close(client_fd);
}

void finalizador(PobActual &ga, int SERVER_PORT)
{
	string SERVER_ADDRESS = "localhost";
	// Creación del socket para conectarse al servidor creado.
	Socket sock(SERVER_ADDRESS , SERVER_PORT);
    string buffer = "";
	//El proceso se bloquea hasta que el último cliente conectado acabe sus peticiones de reservas
	ga.dormirFinalizador();
	int sock_fd = sock.Connect();
	sock.Recv(sock_fd, buffer, MESSAGE_SIZE);
}



void controlEstadistico(PobActual &pa, Poblacion &persona, int SERVER_PORT, int max_connections, int GEN_MAX){
    thread cliente[max_connections];
	thread finalizar;
    thread imprimir;
    int client_fd[max_connections];

	

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_PORT);
	
	// Bind 
	int socket_fd =socket.Bind();
	if (socket_fd == -1) {
    	cerr << "Error en el bind: " + string(strerror(errno)) + "\n";
		exit(1);
	}

	// Listen
	int error_code = socket.Listen(max_connections);
	if (error_code == -1) {
		cerr << "Error en el listen: " + string(strerror(errno)) + "\n";
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}
	//Proceso dormido que ejecutará una conexión al servidor cuando el avión se haya llenado y salgan todos los clientes del servicio.
	finalizar = thread(&finalizador,ref(pa), SERVER_PORT);
    imprimir = thread(&imprimirCSV,ref(persona), ref(pa), GEN_MAX);
	int conected = 0;
	int i=0;
	//Si gestor.estaLleno significa que finalizador ha ejecutado su conexión con el servidor porque el avión estaba lleno y los clientes
	//salieron del servicio, por tanto se dejan de aceptar nuevas conexiones.
	while (i<max_connections && !pa.finalAccepts()) {
		// Accept
		client_fd[i] = socket.Accept();

		if(client_fd[i] == -1) {
			cerr << "Error en el accept: " + string(strerror(errno)) + "\n";
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		conected++;
		//Servidor que atenderá al cliente que se acaba de conectar.
		cliente[i] = thread(&servCliente, ref(socket), client_fd[i], ref(pa));
		i++;
	}
	for (int i=0; i<conected; i++) {
		cliente[i].join();
	}
	finalizar.join();
	imprimir.join();
    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if (error_code == -1) {
    	cerr << "Error cerrando el socket del servidor: " + string(strerror(errno)) + " aceptado" + "\n";
	}
}




int main(int argc, char const *argv[]){

    srand(time(nullptr));
    srand48(time(NULL));
    if (argc != 3)
    {
        cerr << "introduzca fichero y ciudad inicial" << endl;
        exit(1);
    }
    const string MENS_FIN("END OF SERVICE");
    int MAX_CONEXIONS_EST = 20;
    int puertoServer;
    int numPersonas;
    int numServers;
    int gen, puertoCs;
    string IPs[MAX_SERVERS];
    int ops[3] = {0, 1, 2};
    string aux = argv[1];
    string fichero = "./" + aux;
    int cities = stoi(&fichero[fichero.find_first_of("0123456789")]);
    int ciudIni = stoi(argv[2]);
    leerconfig(numServers, puertoCs, gen, puertoServer, IPs, numPersonas, ops);
    PobActual pa(gen);
    Poblacion proletariado(numPersonas, ciudIni, cities, fichero);

    thread estadistico (&controlEstadistico,ref(pa),ref(proletariado),puertoCs, MAX_CONEXIONS_EST, gen);
    thread GAcontrol (&controlGenetico,numServers, puertoServer,ref(proletariado),ref(pa), IPs,gen,ops);
    estadistico.join();
    GAcontrol.join();
    proletariado.mejCam();
    cout<<"FIN DE SERVICIO, MIRAR salida.csv PARA VER EL HISTÓRICO"<<endl;
    return 0;
}
