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
//#include "Monitorizacion.hpp"

using namespace std;

const int MESSAGE_SIZE = 100000; //mensajes de no más 10000 caracteres
const int MAX_SERVERS = 3; //El numero de servidores maximo que tenemos que lanzar

void leerconfig(int &numServers, int &puerto, int &numCiudades, string IPs[], int &numPersonas, string &fDatos){
    string buffer;
    ifstream f;
    f.open("cliente.config");
    if(f.is_open()){
        while(!f.eof() && getline(f,buffer)){
            if(buffer.find("Puerto:")==0){
                puerto = stoi(&buffer[strlen("Puerto:")]);
                cout<<"Puerto = "<<puerto<<endl;
            }
            else if(buffer.find("numCiudades:")==0){
                numCiudades = stoi(&buffer[strlen("numCiudades:")]);
                cout <<"numCiudades = "<<numCiudades<<endl;
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


void calcEstadisticas(Poblacion& personas,int ID,PobActual &pa,float &mejorFit,float &media){
    pa.syncro(ID);
    #warning darle valor a fit para calcular el % IGUAL VAR GLOBAL
    float fit;
    float porcentaje = personas.stats(fit,mejorFit,media);
    pa.finProceso(ID);
}


void controlEstadistico(Poblacion& personas,PobActual &pa){
    cout<<"CONTROL ESTADISTICO\n";
    ofstream f("salida.csv");
    pa.esperaGA();
    f << "ID poblacion" << "," << "Mejor Fitness" << "," << "Fitness Medio" << endl;
    int i = 0;
    float mejorFit,media;
    while (i<MAX_GENS){
        calcEstadisticas(personas,i+1,pa,mejorFit,media);
        f << i+1 << "," << mejorFit << "," << media << endl;
        pa.esperaGA();
        i++;
    }
    int max = 0;
    if (pa.finEjec(personas)){
        max = i;
    }else{
        max = MAX_GENS;
    }
    f.close();
}


void controlGenetico(int numServers, int puerto, Poblacion &personas, PobActual &pa, string IPs[]){
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
	for (int i = 0; i < MAX_GENS && !pa.finEjec(personas); i++){
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
    string fichero;
    string IPs[MAX_SERVERS];
    PobActual pa;
    
    #warning la ciudad a inicial se puede cambiar
    leerconfig(ref(numServers), ref(puertoServer), ref(cities), IPs, ref(numPersonas), ref(fichero));
 
    Poblacion proletariado(numPersonas,3,cities,fichero);

    thread estadistico (&controlEstadistico,ref(proletariado),ref(pa));
    thread GAcontrol (&controlGenetico,numServers, puertoServer,ref(proletariado),ref(pa), IPs);
    estadistico.join();
    GAcontrol.join();
    cout<<"FIN DE SERVICIO, MIRAR salida.csv PARA VER EL HISTÓRICO"<<endl;
    return 0;
}
