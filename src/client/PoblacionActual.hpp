//*****************************************************************
// File:   PoblacionActual.hpp
// Author: Íñigo Aréjula, Sergio Cartiel, Aaron Ibáñez, Héctor García y Marcos Hernando
// Date:   Noviembre-Enero 2019
//*****************************************************************

#ifndef PoblacionActual
#define PoblacionActual

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "caminante.hpp"

using namespace std;
//Numero maximo de generaciones que crearemos

class PobActual
{
public:
    //Construye el monitor
    PobActual(int gen);
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
    //Inserta los datos en la matriz del estadistico
    void agnadirDatos(int numGen, int mejorFit, int avgFit);
    //añadir funcion bool que indique cuando acabar
    bool finEjec(Poblacion &personas);
    //haced sleep de GA y despierta estadistico 
    void esperaEstadistico();
    // esperar GA control hace signal de ga xq ya ha cogido la poblacion y 
    //wait de control estadistico
    void esperaGA();
    void syncro(int id);
    void finProceso(int id);
    void despertarTodos();
    void dormirFinalizador();
    void nuevoCliente();
    void finCliente();
    bool finalAccepts();
    void guardarDatos(string info);
    void extraerDatos(string &datos);
    void dormirServidor();
    bool finGA();
private:
    string datosCompartidos;
    int **historico;
    mutex mtx;
    int numGen; //Número de poblaciones generadas hasta el momento
    //Variables condicion para sincronizar estadisitico y GA
    condition_variable dormir_estadistico,dormir_GA;
    condition_variable calcEstadistico, Finalizador, servidor;
    bool *sync;
    int clientesConectados;
    bool finAccepts;
    bool finPrograma;
};
#endif






