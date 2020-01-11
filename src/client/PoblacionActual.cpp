#include "PoblacionActual.hpp"

//n = numero de personas
PobActual:: PobActual(int gen){
    historico= new int*[gen];
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
void PobActual::finProceso(int id){
    unique_lock <mutex> lck(mtx);
    sync[id-1] = true;
    calcEstadistico.notify_all();
}

void PobActual::despertarTodos(){
    unique_lock <mutex> lck(mtx);
    calcEstadistico.notify_all();
    dormir_estadistico.notify_all();
    dormir_GA.notify_all();
}