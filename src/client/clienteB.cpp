//*****************************************************************
// File:   clienteB.cpp
// Author: Íñigo Aréjula, Sergio Cartiel, Aaron Ibáñez, Héctor García y Marcos Hernando
// Date:   Noviembre-Enero 2019
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include "Socket.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
void leerconfig(int &puerto, string &ipServer){
    string buffer;
    ifstream f;
    f.open("clienteB.config");
    if(f.is_open()){
        while(!f.eof() && getline(f,buffer)){
            if(buffer.find("Puerto:")==0){
                puerto = stoi(&buffer[strlen("Puerto:")]);
                cout <<"Puerto = "<<puerto<<endl;
            }
            else if(buffer.find("SERVERS:{")==0){
                getline(f,buffer);
                ipServer = buffer;
                cout <<"IP = "<< ipServer <<endl;
                getline(f,buffer); // lee la llave del final aunque no sirva de nada
            }
        }      
    }
    else{
        cerr<<"No se ha podido abrir config"<<endl;
        exit(1);
    }
    f.close();
}

int main(int argc, char *argv[]) {
    cout<<"GENERACIÓN    MEJOR FIT     MEDIA FIT"<<endl;
    cout<<"-------------------------------------"<<endl;
    const string MENS_FIN("END OF SERVICE");
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS;
    int SERVER_PORT;
    leerconfig(SERVER_PORT,SERVER_ADDRESS);

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
    cout<< "ID poblacion | Mejor Fitness | Fitness Medio" << endl;
    string mensaje;
    while (mensaje != MENS_FIN){
        mensaje = ("");
        int read_bytes = socket.Recv(socket_fd, mensaje, MESSAGE_SIZE);
        cout << mensaje << endl;
    }
    // Cerramos el socket
    int error_code = socket.Close(socket_fd);
    if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    // Despedida

    return error_code;
}