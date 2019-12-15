//*****************************************************************
// File:   Servidor.cpp
// Date:   diciembre 2019
//*****************************************************************

#include <iostream>
#include "Socket.hpp"
#include <thread>
#include <cstring>
#include "PoblacionAProcesar.hpp"
//#include "caminante.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

//-------------------------------------------------------------
int main(int argc, char *argv[]) {
	char MENS_FIN[]="END OF SERVICE";
	
    if(argc != 2) {	// Comprobar que se introduce el puerto de escucha
        cout << "Introduce un puerto (Por ejemplo: ./Servidor 2000)" << endl;
        exit(1);
    }

    int SERVER_PORT = atoi(argv[1]);
	cout << "Puerto seleccionado: " << SERVER_PORT << endl;

	Socket socket(SERVER_PORT);		// Creación del socket

	// Bind 
	int socket_fd =socket.Bind();
	if (socket_fd == -1) {
		string mensError(strerror(errno));
    	cerr << "Error en el bind: " + mensError + "\n";
		exit(1);
	}

	// Listen
    int max_connections = 1;
	int error_code = socket.Listen(max_connections);
	if (error_code == -1) {
		string mensError(strerror(errno));
    	cerr << "Error en el listen: " + mensError + "\n";
		socket.Close(socket_fd);	// Se cierra el socket
		exit(1);
	}

	// Accept
	int client_fd = socket.Accept();
	if (client_fd == -1) {
		string mensError(strerror(errno));
    	cerr << "Error en el accept: " + mensError + "\n";
		socket.Close(socket_fd);	// Se cierra el socket
		exit(1);
	}

	// Buffer para recibir el mensaje
	string buffer;  // Alamacena el mensaje

	bool out = false; // Inicialmente no salir del bucle
	while (!out) {
		// Recibimos el mensaje del cliente
		int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
		if(rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			socket.Close(client_fd);
			socket.Close(socket_fd);
		}

		cout << "Subpoblacion recibida: " << buffer << endl;
		
		if (buffer == MENS_FIN) {	// Si recibimos "END OF SERVICE" se cierra la comunicacion
			out = true; 
		} else {
			// Operar con la sub-poblacion (seleccionar, cruzar y mutar)
			PoblacionAProcesar pAp(buffer); 	// Construir monitor con la sub-poblacion recibida

			// thread procesos[n];

			// lanzamiento de procesos

			//generar cadena resultado

			string nuevaSubPoblacion = "la respuesta que sea";
			
            // Send, enviar nueva sub-poblacion al cliente
			int send_bytes = socket.Send(client_fd, nuevaSubPoblacion);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
    			cerr << "Error al enviar datos: " + mensError + "\n";
				socket.Close(client_fd);
				socket.Close(socket_fd);
				exit(1);
			}
		}
	}

	// Cerramos el socket del cliente
    error_code = socket.Close(client_fd);
    if (error_code == -1) {
    	string mensError(strerror(errno));
    	cerr << "Error cerrando el socket del cliente: " + mensError + "\n";
    }

    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if (error_code == -1) {
    	string mensError(strerror(errno));
    	cerr << "Error cerrando el socket del servidor: " + mensError + "\n";
    }

	cout << "Fin de la comunicacion" << endl;

    return error_code;
}