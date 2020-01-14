//*****************************************************************
// File:   PoblacionActual.cpp
// Author: Íñigo Aréjula, Sergio Cartiel, Aaron Ibáñez, Héctor García y Marcos Hernando
// Date:   Noviembre-Enero 2019
//*****************************************************************


#include "PoblacionActual.hpp"

//n = numero de personas
PobActual:: PobActual(int gen){
    finPrograma = false;
    historico= new int*[gen];
    clientesConectados = 0;
    finAccepts = false;
    for (int i = 0; i < gen; i++)
    {
        historico[i]=new int [2];
    }
    
    numGen = 0;
    sync = new bool[gen];
    for (int i = 0; i < gen; i++)
    {
        sync[i] = false;
    }
}

PobActual:: ~PobActual(){

}


int PobActual::numGeneracion(){
    return numGen;
}




void PobActual::agnadirDatos(int numGen, int mejorFit, int avgFit){
    unique_lock <mutex> lck(mtx);
    historico[numGen][0]=mejorFit;
    historico[numGen][1]=avgFit;
}
//añadir funcion bool que indique cuando acabar
bool PobActual::finEjec(Poblacion &personas){
    unique_lock <mutex> lck(mtx);
    #warning darle valor a fit para calcular el % IGUAL VAR GLOAL
    float fit,mejorFit,media;
    float porcentaje = personas.stats(fit,mejorFit,media);
    if (porcentaje <= 3.0){
        return true;
    }else{
        return false;
    }
}
//haced sleep de GA y despierta estadistico 
void PobActual::esperaEstadistico(){
    unique_lock <mutex> lck(mtx);
    dormir_estadistico.notify_all();
    dormir_GA.wait(lck);
}

// esperar GA control hace signal de ga xq ya ha cogido la poblacion y 
//wait de control estadistico
void PobActual::esperaGA(){
    unique_lock <mutex> lck(mtx);
    dormir_GA.notify_all();
    dormir_estadistico.wait(lck);
}

void PobActual::syncro(int id){
    unique_lock <mutex> lck(mtx);
    if (id !=1){
        while (!sync[id-2]){
            calcEstadistico.wait(lck);
        }
    }
}

void PobActual::dormirServidor(){
    unique_lock <mutex> lck(mtx);
    servidor.wait(lck);
}

//Guarda los datos de la poblacion actual en el string datosCompartidos para 
//que luego tengan acceso a ellos otras funciones.
void PobActual::guardarDatos(string info){
    unique_lock <mutex> lck(mtx);
    datosCompartidos = info;
    servidor.notify_all();
}
//Lee los datos de la poblacion que hay almacenados en el string datosCompartidos 
//y los devuelve por referecia;
void PobActual::extraerDatos(string &datos){
    unique_lock <mutex> lck(mtx);
    datos = datosCompartidos;
}

void PobActual::finProceso(int id){
    unique_lock <mutex> lck(mtx);
    sync[id-1] = true;
    calcEstadistico.notify_all();
}

void PobActual::nuevoCliente(){
    unique_lock <mutex> lck(mtx);
    clientesConectados++;
}

bool PobActual::finalAccepts(){
    unique_lock <mutex> lck(mtx);
    return finAccepts;
}

void PobActual::finCliente(){
    unique_lock <mutex> lck(mtx);
    clientesConectados--;
    if(clientesConectados == 0) {
        finAccepts = true;
        Finalizador.notify_all();
    }
}

bool PobActual::finGA(){
    return finPrograma;
}

void PobActual::despertarTodos(){
    unique_lock <mutex> lck(mtx);
    calcEstadistico.notify_all();
    dormir_estadistico.notify_all();
    dormir_GA.notify_all();
    servidor.notify_all();
    finPrograma = true;
    if(clientesConectados == 0){
        finAccepts = true;
        Finalizador.notify_all();
    }
}


void PobActual::dormirFinalizador(){
    unique_lock <mutex> lck(mtx);
    Finalizador.wait(lck);
}