#include "PoblacionActual.hpp"

//n = numero de personas
PobActual:: PobActual(){
    numGen = 0;
    //historico[MAX_GENS][2];
    for (int i = 0; i < MAX_GENS; i++)
    {
        sync[i] = false;
    }
}

PobActual:: ~PobActual(){

}

void PobActual::codificar(Poblacion pob, int flg){
    unique_lock <mutex> lck(mtx);
    pob.codificar(flg);

}


void PobActual::descodificar(Poblacion pob,string msg,int flg){
    unique_lock <mutex> lck(mtx);
    pob.descodificar(msg,flg);
    
}




/*Poblacion PobActual::crearPob(int n, int caminantes, string entrada){
    unique_lock <mutex> lck(mtx);
    if(n=1) return Poblacion();
    if(n=2) return Poblacion(caminantes,entrada);
    else return Poblacion(entrada);
    numGen++;  
}*/

void PobActual::dividir(Poblacion pob,int n,Poblacion pobs[]){
    unique_lock <mutex> lck(mtx);
    pob.dividir(n,pobs);
}

Poblacion PobActual::fusionar(Poblacion pobs[]){
    numGen++; //Al fusionar se considera la siguiente generacion
    //Esperando a que implementen los constructores de Poblacion
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