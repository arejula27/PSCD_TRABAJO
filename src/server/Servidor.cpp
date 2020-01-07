//*****************************************************************
// File:   Servidor.cpp
// Date:   diciembre 2019
//*****************************************************************

#include <iostream>
#include "Socket.hpp"
#include <thread>
#include <cstring>
#include "PoblacionAProcesar.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
const int ALL_POB = 1;
//const int UPGRADE_POB = 0;

//-------------------------------------------------------------
void procesoCaminante(PoblacionAProcesar &pAp, int id, int op, Socket socket, int client_fd) {
	switch(op) {
		case 0:		// Cruzar
			pAp.cruzar(id,id+1);
			break;
		case 1:		// Mutar
			pAp.mutar(id);
			break;
		case 2:		// Seleccionar
			pAp.seleccionar();
			break;
		default: 	// Operacion incorrecta
			cout << "ERROR en operacion recibida" << endl;
			// Send, enviar mensaje de error de operacion
			string errorMsg = "ERROR-OP";
			int send_bytes = socket.Send(client_fd,errorMsg);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
    			cerr << "Error al enviar datos: " + mensError + "\n";
				socket.Close(client_fd);
				exit(1);
			}
	}
}

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
    int max_connections = 10;
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

	cout << "Conexion establecida con el cliente: " << client_fd << endl;

	string buffer;  // Almacena el mensaje
	bool out = false; // Inicialmente no salir del bucle
	bool primera_vez = true;
	Poblacion pob;
	while (!out) {
		// Recibimos la peticion del cliente
		int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
		if(rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			socket.Close(client_fd);
		}
		cout << "Recibido mensaje " << endl;
		
		if (buffer == MENS_FIN) {
			cout<<"Ultimo mensaje"<<endl;// Si recibimos "END OF SERVICE" se cierra la comunicacion
			out = true; 
		} else {
			if(primera_vez) {	//primera vez
				primera_vez = false;
				pob.descodificar(&buffer[2],ALL_POB);
				cout<<"He decodificado el mensaje"<<endl;

			}
			else {	//actualizar
				pob.descodificar(&buffer[2],UPGRADE_POB);
			}
			// Operar con la sub-poblacion (seleccionar, cruzar y mutar)
			int operacion = atoi(&buffer[0]);	// Operacion a realizar
			PoblacionAProcesar pAp(pob);	// Construir monitor con la sub-poblacion recibida

			int n = pob.getNumCam();				// Obtener numero de caminantes
			thread proceso[n];	
			for(int i=0; i<n; i++) {
				proceso[i] = thread(&procesoCaminante,ref(pAp),i,operacion,socket,client_fd);
			}

			for(int i=0; i<n; i++) {
				proceso[i].join();
			}

			// Una vez termine pasar la poblacion del monitor a pob, para enviarlo
			pob = pAp.getPoblacion();

			string nuevaSubPoblacion = pob.codificar();	//generar cadena resultado
			
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