#ifndef MONITORIZACION
#define MONITORIZACION

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "caminante.hpp"

using namespace std;

//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"? Modo no-main: var "logger" se declarará en el "main"
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    extern Logger logger;
#else
    #ifndef ADD_EVENT //para evitar redefiniciones
        #define ADD_EVENT(e)  // nada
    #endif
#endif

class Monitorizacion{
    friend Caminante;
public:
    //Construye el monitor
    Monitorizacion();
    //Destructor
    ~Monitorizacion();
    //Crea poblaciones según el parámetro n
    Poblacion crearPob(int n);
    //Divide la poblacion en n subpoblaciones
    void dividir(Poblacion pob,int n, Poblacion pobs[]);
    //Devuelve una poblacionn fruto de fusionar pobs[]
    Poblacion fusionar(Poblacion pobs[]);
    //Devuelve el mejor fitness de la poblacion
    int mejorFit(Poblacion pob);
    //Devuelve la media de fitness de la poblacion
    int mediaFit(Poblacion pob);
    //Prepara los datos para ser enviados por el socket
    void codificar(Poblacion pob,int flg = ALL_POB);
    //flg: 1=ALL_POB 2=UPGRADE_POB
    void descodificar(Poblacion pob,string mag, int flg);

private:

    mutex mtx;

    int numGen; //Número de poblaciones generadas hasta el momento
    
    //Condicion para que el estadistico pueda tomar los datos como validos
    condition_variable (datosListos); //Es necesario?
    
};
#endif