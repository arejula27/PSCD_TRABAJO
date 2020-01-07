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

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
void calcEstadisticas(Poblacion& personas,int ID,PobActual &pa,float &mejorFit,float &media){
	pa.syncro(ID);
	#warning darle valor a fit para calcular el % IGUAL VAR GLOBAL
	float fit;
	float porcentaje = personas.stats(personas,fit,mejorFit,media);
	pa.finProceso(ID);
}
void controlEstadistico(Poblacion& personas,PobActual &pa){
	ofstream f("salida.csv");
	f << "ID poblacion" << "," << "Mejor Fitness" << "," << "Fitness Medio" << endl;
	thread estadisticas[MAX_GENS];
	int i = 0;
	float mejorFit,media;
	while (i < MAX_GENS && !pa.finEjec(personas)){
		estadisticas[i] = thread(&calcEstadisticas,ref(personas),i+1,ref(pa),ref(mejorFit),ref(media));
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
	for (int i = 0; i < max; i++){
		estadisticas[i].join();
	}
}
void controlGenetico(const int NUM_SERVERS,const int SERVER_PORT, Poblacion &personas, PobActual &pa){
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
	cout << "llega" << endl;
	Poblacion pobs[serversAceptados];
	cout << "llega2" << endl;
	for (int i = 0; i < serversAceptados; i++){
		pobs[i].getMatrixFrom(personas);
	}
	cout << "llega3" << endl;

	for (int i = 0; i < MAX_GENS && !pa.finEjec(personas); i++){
		cout << "llega4" << endl;
		personas.dividir(serversAceptados,pobs);
		cout << "llega5" << endl;
		for (int j = 0; j < 3; j++){
			// 0 cruzar
			// 1 mutar
			// 2 seleccionar
			for(int k = 0; k < serversAceptados; k++){
				string msg = to_string(j) + "," + pobs[i].codificar((0==i)?ALL_POB:UPGRADE_POB);
				cout << "Enviando: " << endl;
				cout << msg << endl;
				socketServ[i].Send(server_fd[i],msg);
			}
			for(int k = 0; k < serversAceptados; k++){
				string resp;
				socketServ[i].Recv(server_fd[i],resp,MESSAGE_SIZE);
				pobs[i].descodificar(resp,UPGRADE_POB);
			}

		}
		personas.fusionar(serversAceptados,pobs);
		pa.esperaEstadistico();
	}
	//mando mensaje de finalizacion
	for (int i = 0; i < serversAceptados; i++){
		#warning cambiar msg de finalizacion
		string fin = "todos tochos";
		socketServ[i].Send(server_fd[i],fin);
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
	PobActual pa;
	cout << "Que fichero quieres abrir ? ";
	string fichero;
	cin >> fichero;
	cout << "Cuantas ciudades vamos a tener ? " << endl;
	int cities;
	cin >> cities;
	#warning la ciudad a inicial se puede cambiar
	Poblacion proletariado(NUM_PERSONAS,3,cities,fichero);

	thread estadistico (&controlEstadistico,ref(proletariado),ref(pa));
	thread GAcontrol (&controlGenetico,NUM_SERVERS,SERVER_PORT,ref(proletariado),ref(pa));
	estadistico.join();
	GAcontrol.join();

	return 0;
}
