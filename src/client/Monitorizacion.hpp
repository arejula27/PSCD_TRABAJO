#ifndef Monitorizacion
#define Monitorizacion

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

class Estadistico
{
    friend Caminante;
public:
    Estadistico(int n);
    ~Estadistico();
     //Devuelve el mejor fitness de la poblacion
    int mejorFit(Poblacion pob);
    //Devuelve la media de fitness de la poblacion
    int mediaFit(Poblacion pob);
    //Inserta los datos en la matriz del estadistico
    void agnadirDatos(int numGen, int mejorFit, int avgFit);

private:
    int historico[][2];
    mutex mtx;
};

#endif