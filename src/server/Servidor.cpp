//*****************************************************************
// File:   Servidor.cpp
// Date:   diciembre 2019
//*****************************************************************

#include <iostream>
#include "Socket.hpp"
#include <thread>
#include <cstring>
#include "PoblacionAProcesar.hpp"
#include <math.h>
#include <unistd.h>

using namespace std;

const int MESSAGE_SIZE = 10000; //mensajes de no más 4000 caracteres //SE SUPONE QUE ERA 4001¿?¿?¿?¿?¿?¿?
const int NUM_PROCESOS_MAX = 5;    //numero de procesos concurrente maximo
const double PORCENTAJE_EXTRA = 0.2;    //numero de caminantes de más que vamos a crear
const int MAX_MSG_SIZE = 10000;
const string FIN_MSG_RECIBIDO = "*";
//-------------------------------------------------------------
void procesoCruzar(PoblacionAProcesar &pAp, int comienzo, int div_n, int n, int j, int extra) {
    if(j != 4) {
        for(int i=comienzo;i<comienzo+div_n;i++){
            if(i == div_n) {
                pAp.cruzar(i,comienzo);
            }
            else {
                pAp.cruzar(i,i+1);
            }
        }
    }
    else {    // proceso extra
        for(int i=0; i<extra; i++) {
            bool puede = false;
            while(!puede) {
                int aleatorio = 0 + (rand() % static_cast<int>(n - 0 + 1));
                
                int aleatorio2 = 0 + (rand() % static_cast<int>(n - 0 + 1));
                if(aleatorio != aleatorio2) {
                    puede = true;
                    pAp.cruzar(aleatorio,aleatorio2);
                }
            }
        }
    }
}

void procesoMutar(PoblacionAProcesar &pAp, int comienzo, int div_n,int r) {
    for(int i=comienzo; i<comienzo+div_n; i++) {
		if(i<r){
		}else{
        pAp.mutar(i);
		}
	}
}

void procesoSeleccionar(PoblacionAProcesar &pAp) {
	
	pAp.seleccionar();
	
}


//-------------------------------------------------------------
int main(int argc, char *argv[]) {
    srand(time(NULL));
	int r;
    char MENS_FIN[]="END OF SERVICE*";
    
    if(argc != 2) {    // Comprobar que se introduce el puerto de escucha
        cout << "Introduce un puerto (Por ejemplo: ./Servidor 2000)" << endl;
        exit(1);
    }

    int SERVER_PORT = atoi(argv[1]);
    cout << "Puerto seleccionado: " << SERVER_PORT << endl;

    Socket socket(SERVER_PORT);        // Creación del socket

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

	cout << "Conexion establecida con el cliente "  << endl;

	string buffer;  // Almacena el mensaje
	bool out = false; // Inicialmente no salir del bucle
	bool primera_vez = true;
	string aux, msg;
	Poblacion pob;
	int gen=0;
	
	int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
	pob.descodificar(buffer,NCIT);
	cout<<"Recibido número de ciudades: "<<pob.getNumCities()<<endl;
	while (!out) {
		// Recibimos la peticion del cliente
		msg = "";
		do{
			buffer = "";
			int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
			if (rcv_bytes == -1)
			{
				 string mensError(strerror(errno));
				 cerr << "Error al recibir datos: " + mensError + "\n";
				 socket.Close(client_fd);
				 socket.Close(socket_fd);
			}
			msg += buffer;
			aux = buffer.back();
		}while(buffer.find("*")==string::npos);
		if (buffer == MENS_FIN) {	// Si recibimos "END OF SERVICE" se cierra la comunicacion
			cout << "Recibido mensaje de finalización, listo para cerrar el servidor"<< endl;
			out = true; 
		} else {
			cout << "Recibido mensaje " <<(gen)<<" generación "<<((gen++)/3+1)<< endl;
			cout <<"Procesando nueva población..."<< endl;
			pob.descodificar(&msg[msg.find_first_of("0123456789")+2], UPGRADE_POB);

			// Operar con la sub-poblacion (seleccionar, cruzar y mutar)
			int operacion = stoi(&msg[msg.find_first_of("0123456789")]); // Coger la operacion a realizar
			PoblacionAProcesar pAp(pob);  // Construir monitor con la sub-poblacion recibida
			int n = pob.getNumCam();	  // Obtener numero de caminantes
			int extra = n * PORCENTAJE_EXTRA;
			int div_n = n / (NUM_PROCESOS_MAX - 1);
			int resto = n % (NUM_PROCESOS_MAX - 1);
			cout << "Numero de caminantes recibidos: " << n << endl;
			cout << "Numero de iteraciones de 5 procesos: " << div_n << endl;
			cout << "Numero de caminantes extra: " << extra << endl;
			thread proceso[NUM_PROCESOS_MAX];
			int id = 0;
			int comienzo = 0;
			switch (operacion)
			{
			case 0: // Cruzar
				cout << "Cruzando población" << endl;
				pAp.setNumCamOrig();
				cout << "Seleccionara: " << pAp.getNumCamOrig() << endl;
				// Cruzar con 5 hilos
				for (int i = 0; i < NUM_PROCESOS_MAX; i++)
				{
					if (resto > 0)
					{
						proceso[i] = thread(&procesoCruzar, ref(pAp), comienzo, div_n + 1, n, i, extra);
						comienzo += div_n + 1;
						resto--;
					}
					else
					{ // hilo para cruzar los extra
						proceso[i] = thread(&procesoCruzar, ref(pAp), comienzo, div_n, n, i, extra);
						comienzo += div_n;
					}
				}
				for (int i = 0; i < NUM_PROCESOS_MAX; i++)
				{
					proceso[i].join();
				}

				r = n; //Almacena padres
				break;
			case 1: // Mutar
				cout << "Mutando población" << endl;
				// Mutar caminantes reptartido en 5 procesos
				/*
					for(int i=0; i<NUM_PROCESOS_MAX; i++) {
						if(i == 0) {
							proceso[i] = thread(&procesoMutar,ref(pAp),comienzo,div_n+resto,r);
						}
						else {
							proceso[i] = thread(&procesoMutar,ref(pAp),comienzo,div_n,r);
						}
						comienzo += div_n;
					}
					for(int i=0; i<NUM_PROCESOS_MAX; i++) {
						proceso[i].join();
					}
					*/
				procesoMutar(pAp, 0, n, 0);
				break;
			case 2: // Seleccionar
				cout << "Seleccionando población" << endl;
				//cout << &buffer[2] << endl;
				cout << "Va a seleccionar "<< endl;
				
				procesoSeleccionar(ref(pAp));
				break;
			default: // Operacion incorrecta
				cout << "ERROR en operacion recibida" << endl;
				// Enviar mensaje de error de operacion
				string errorMsg = "ERROR-OP*";
				int send_bytes = socket.Send(client_fd, errorMsg);
				if (send_bytes == -1)
				{
					string mensError(strerror(errno));
					cerr << "Error al enviar datos: " + mensError + "\n";
					socket.Close(client_fd);
					exit(1);
				}

			}

			
			int tamMsg, numPaquetes, tamLastPaquete, iterMsg;
    		string msgPartido;

			// Una vez termine pasar la poblacion del monitor a pob, para enviarlo
			pob = pAp.getPoblacion();
			cout  << pob.getNumCam() << " son los caminantes que va a enviar" << endl;
			string nuevaSubPoblacion = pob.codificar(UPGRADE_POB) + "*";	//generar cadena resultado
			// Send, enviar nueva sub-poblacion al cliente
			cout<<"ENVIO------\n";
			tamMsg = nuevaSubPoblacion.length();
            numPaquetes = tamMsg / MAX_MSG_SIZE;
            tamLastPaquete = tamMsg % MAX_MSG_SIZE;
            iterMsg = 0;
            while(numPaquetes >= 0){
                    msgPartido = &nuevaSubPoblacion[iterMsg];
                    msgPartido.resize(MAX_MSG_SIZE);
                    iterMsg += msgPartido.length();
                    socket.Send(client_fd,msgPartido);
                    numPaquetes--;
            }
            
            /*
			if(send_bytes == -1) {
                string mensError(strerror(errno));
                cerr << "Error al enviar datos: " + mensError + "\n";
                socket.Close(client_fd);
                socket.Close(socket_fd);
                exit(1);
            }
			*/
            cout << "Mensaje enviado al cliente" << endl;
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
