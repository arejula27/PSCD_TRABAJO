#include "caminante.hpp"
#include "PoblacionActual.hpp"

//n = numero de personas
PobActual:: PobActual(int n){
    numGen = 0;
    historico[n][2];

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


int PobActual::mejorFit(Poblacion pob){
    unique_lock <mutex> lck(mtx);
    pob.mejorFit();
}

int PobActual::mediaFit(Poblacion pob){
    unique_lock <mutex> lck(mtx);
    pob.mediaFit(); 
}

void PobActual::agnadirDatos(int numGen, int mejorFit, int avgFit){
    unique_lock <mutex> lck(mtx);
    historico[numGen][0]=mejorFit;
    historico[numGen][1]=avgFit;
}