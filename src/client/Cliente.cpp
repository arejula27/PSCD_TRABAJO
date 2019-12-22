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
#include "Socket.hpp"
#include "PoblacionActual.hpp"
#include "Monitorizacion.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
void controlEstadistico(Poblacion& personas){
	int bestFit,avgFit;
	//esperar fusion 
	Estadistico datos(10);/*nose a que se refiere la n*/
	bestFit = datos.mejorFit(personas);
	avgFit = datos.mediaFit(personas);
	datos.agnadirDatos(10,bestFit,avgFit);
}
void controlGenetico(int NUM_SERVERS,int SERVER_PORT, int MAX_GENS, Poblacion& personas, PobActual &pa){
	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socketServ[NUM_SERVERS];
	for (int i = 0; i<NUM_SERVERS; i++){
		cout << "Direciion ip de servidor " << i+1 << ":";
		//flush si sabemos xd
		string serverAddr;
		cin >> serverAddr;
		socketServ[i].ini(serverAddr,SERVER_PORT);
	}
	int server_fd[NUM_SERVERS];
	// Conectamos con el servidor. Probamos varias conexiones
	
	int server_fd[NUM_SERVERS];
	int serversAceptados = 0;
	for (int i = 0; i<NUM_SERVERS; i++){
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
	Poblacion pobs[serversAceptados];
	for (int i = 0; i < serversAceptados; i++){
		pobs[i].getMatrixFrom(personas);
	}

	for (int i = 0; i < MAX_GENS && !pa.finEjec(); i++){

		personas.dividir(serversAceptados,pobs);
		for (int j = 0; j < 3; j++){
			// 0 cruzar
			// 1 mutar
			// 2 seleccionar
			for(int k = 0; k < serversAceptados; k++){
				string msg = to_string(j) + "," + pobs[i].codificar((0==i)?ALL_POB:UPGRADE_POB);
				socketServ[i].Send(server_fd[i],msg);
			}
			for(int k = 0; k < serversAceptados; k++){
				string resp;
				socketServ[i].Recv(server_fd[i],resp,MESSAGE_SIZE);
				pobs[i].descodificar(resp,UPGRADE_POB);
			}

		}
		personas.fusionar(serversAceptados,pobs);

	}

    // Cerramos el socket
	for (int i = 0; i < serversAceptados; i++){
		int error_code = socketServ[i].Close(server_fd[i]);
		if(error_code == -1){
			cerr << "Error cerrando el socket: " << strerror(errno) << endl;
		}
	} 
}
int main(int argc, char const *argv[]){

	const string MENS_FIN("END OF SERVICE");
    int SERVER_PORT = 2000;
	//El numero de servidores que tenemos que lanzar
	const int NUM_SERVERS = atoi(argv[1]);
	//Numero de personas en nuestra poblacion
	int NUM_PERSONAS = atoi(argv[2]);
	//Numero maximo de generaciones a crear
	int MAX_GENS = atoi(argv[3]);
	PobActual pa(MAX_GENS);
	
	cout << "Que fichero quieres abrir ? ";
	string fichero;
	cin >> fichero;
	Poblacion proletariado(NUM_PERSONAS,fichero);

	thread estadistico (&controlEstadistico,ref(proletariado), ref(pa));
	thread GAcontrol (&controlGenetico,NUM_SERVERS,SERVER_PORT,MAX_GENS,ref(proletariado),ref(pa));

	return 0;
}
