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
const int NUM_PROCESOS_MAX = 5;	//numero de procesos concurrente maximo

//-------------------------------------------------------------
void procesoCruzar(PoblacionAProcesar &pAp, int id, int n) {
	if(id == n-1) {	// si es el ultimo
		pAp.cruzar(id,0);	// se cruzara con el primero
	}
	else {
		pAp.cruzar(id,id+1);
	}
	
}

void procesoMutar(PoblacionAProcesar &pAp, int id) {
	pAp.mutar(id);
}

void procesoSeleccionar(PoblacionAProcesar &pAp) {
	pAp.seleccionar();
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
			socket.Close(socket_fd);
		}
		cout << "Recibido " << endl;
		
		if (buffer == MENS_FIN) {	// Si recibimos "END OF SERVICE" se cierra la comunicacion
			out = true; 
		} else {
			if(primera_vez) {	//primera vez
				primera_vez = false;
				pob.descodificar(&buffer[2],ALL_POB);
			}
			else {				//actualizar
				pob.descodificar(&buffer[2],UPGRADE_POB);
			}
			// Operar con la sub-poblacion (seleccionar, cruzar y mutar)
			int operacion = atoi(&buffer[0]);	// Coger la operacion a realizar
			PoblacionAProcesar pAp(pob);	// Construir monitor con la sub-poblacion recibida
			int n = pob.getNumCam();				// Obtener numero de caminantes
			int div_n = n/NUM_PROCESOS_MAX;
			int resto = n%NUM_PROCESOS_MAX;
			cout << "Numero de caminantes recibidos: " << n << endl;
			cout << "Numero de iteraciones de 5 procesos: " << div_n << endl;
			thread proceso[n];
			int id = 0;
			switch(operacion) {
				case 0:		// Cruzar
					// Cruzar de 5 en 5
					for(int j=0; j<NUM_PROCESOS_MAX; j++) {
						for(int i=0; i<div_n; i++) {
							cout << "Se va a cruzar " << id << " y " << id+1 << endl;
							proceso[id] = thread(&procesoCruzar,ref(pAp),id,n);
							id++;
						}
						for(int i=0; i<NUM_PROCESOS_MAX; i++) {
							proceso[id].join();
							id++;
						}
					}
					// Cruzar los restantes
					for(int i=0; i<resto; i++) {
						cout << "Se va a cruzar " << id << " y " << id+1 << endl;
						proceso[id] = thread(&procesoCruzar,ref(pAp),id,n);
						id++;
					}
					for(int i=0; i<resto; i++) {
						proceso[id].join();
						id++;
					}
					break;
				case 1:		// Mutar
					// Mutar de 5 en 5
					for(int j=0; j<div_n; j++) {
						for(int i=0; i<NUM_PROCESOS_MAX; i++) {
							cout << "Se va a mutar " << id << endl;
							proceso[id] = thread(&procesoMutar,ref(pAp),id);
							id++;
						}
						for(int i=0; i<NUM_PROCESOS_MAX; i++) {
							proceso[id].join();
							id++;
						}
					}
					// Mutar los restantes
					for(int i=0; i<resto; i++) {
						cout << "Se va a mutar " << id << endl;
						proceso[id] = thread(&procesoMutar,ref(pAp),id);
						id++;
					}
					for(int i=0; i<resto; i++) {
						proceso[id].join();
						id++;
					}
					break;
				case 2:		// Seleccionar
					cout << "Comienza proeso de seleccion" << endl;
					procesoSeleccionar(ref(pAp));
					break;
				default:	// Operacion incorrecta
					cout << "ERROR en operacion recibida" << endl;
					// Enviar mensaje de error de operacion
					string errorMsg = "ERROR-OP";
					int send_bytes = socket.Send(client_fd,errorMsg);
					if(send_bytes == -1) {
						string mensError(strerror(errno));
						cerr << "Error al enviar datos: " + mensError + "\n";
						socket.Close(client_fd);
						exit(1);
					}

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