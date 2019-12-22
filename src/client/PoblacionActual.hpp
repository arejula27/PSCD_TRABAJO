#ifndef PoblacionActual
#define PoblacionActual

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "caminante.hpp"

using namespace std;

class PobActual
{
    friend Caminante;
public:
    //Construye el monitor
    PobActual(int n);
    //Destructor
    ~PobActual();
    //Crea poblaciones según el parámetro n (llama a uno de los 3 constructores)
    Poblacion crearPob(int n, int caminantes, string entrada);
    //Divide la poblacion en n subpoblaciones
    void dividir(Poblacion pob,int n, Poblacion pobs[]);
    //Devuelve una poblacionn fruto de fusionar pobs[]
    Poblacion fusionar(Poblacion pobs[]);
    //Prepara los datos para ser enviados por el socket
    void codificar(Poblacion pob,int flg = ALL_POB);
    //flg: 1=ALL_POB 2=UPGRADE_POB
    void descodificar(Poblacion pob,string mag, int flg);
    //Devuelve el numero de generacion hasta el momento
    int numGeneracion();
     //Devuelve el mejor fitness de la poblacion
    int mejorFit(Poblacion pob);
    //Devuelve la media de fitness de la poblacion
    int mediaFit(Poblacion pob);
    //Inserta los datos en la matriz del estadistico
    void agnadirDatos(int numGen, int mejorFit, int avgFit);
    //añadir funcion bool que indique cuando acabar
    bool finEjec();
private:
    int historico[][2];
    mutex mtx;
    int numGen; //Número de poblaciones generadas hasta el momento
    //Condicion para que el estadistico pueda tomar los datos como validos
    condition_variable dormir_estadistico,dormir_GA; //Es necesario?
    bool finCalculo;
    
};
#endif






