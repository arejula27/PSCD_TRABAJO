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
void controlEstadistico(Socket& soc, int client_fd,Poblacion& personas){
	int bestFit,avgFit;
	//esperar fusion 
	Estadistico datos(10);/*nose a que se refiere la n*/
	bestFit = datos.mejorFit(personas);
	avgFit = datos.mediaFit(personas);
	datos.agnadirDatos(10,bestFit,avgFit);
}
void controlGenetico(Socket& soc, int client_fd, Poblacion& personas,PobActual &gente){
	//Esperar estadistico
	Poblacion subPob1; Poblacion subPob2; Poblacion subPob3;
	Poblacion pobs[3];
	pobs [0] = subPob1;
	pobs [1] = subPob2;
	pobs [2] = subPob3;
	gente.dividir(3,pobs);

	

	
	
}
int main() {

	
	const string MENS_FIN("END OF SERVICE");
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = "localhost";
    int SERVER_PORT = 2000;

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
	const int MAX_ATTEMPS = 10;
	int count = 0;
	int socket_fd;
	do {
		// Conexión con el servidor
    	socket_fd = socket.Connect();
    	count++;

    	// Si error --> esperamos 1 segundo para reconectar
    	if(socket_fd == -1){
    	    this_thread::sleep_for(chrono::seconds(1));
    	}
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1){
    	return socket_fd;
    }
	//una vez hemos podido conectar con el servidor inicializamos la poblacion
	string mensaje,personas;
	//inicializo el monitor pobActual
	PobActual gente;
	int n;
	int caminantes = 312;
	//falta la funcion que lea el flujo y lo pase a string
	//creo una poblacion y la igualo al resultado de crearPob
	Poblacion proletariado = gente.crearPob(n,caminantes,personas);
	thread estadistico[10];
	thread GAcontrol[10];
	estadistico[0] = thread(&controlEstadistico,ref(socket),count,ref(proletariado));
	GAcontrol[0] = thread(&controlGenetico,ref(socket),count,ref(proletariado));
	do{
		// Leer mensaje de la entrada estandar
		cout << "Frase para contar las vocales: ";
		getline(cin, mensaje);
		// Enviamos el mensaje, como cadena de C
	    int send_bytes = socket.Send(socket_fd, mensaje);

	    if(send_bytes == -1){
			cerr << "Error al enviar datos: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		if(mensaje != MENS_FIN){
		    // Buffer para almacenar la respuesta, como char[]
		    string buffer;

		    // Recibimos la respuesta del servidor  
		    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);

		    // Mostramos la respuesta
		    cout << "Mensaje enviado: '" << mensaje << "'" << endl;
		    cout << "Numero de vocales: " << buffer << endl;
		}
	} while(mensaje != MENS_FIN);

    // Cerramos el socket
    int error_code = socket.Close(socket_fd);
    if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    // Despedida
	cout << "Bye bye" << endl;

    return error_code;
}
