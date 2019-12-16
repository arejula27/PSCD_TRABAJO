#include "caminante.hpp"
#include "Monitorizacion.hpp"


Monitorizacion:: Monitorizacion(){

}

Monitorizacion:: ~Monitorizacion(){

}

void Monitorizacion::codificar(Poblacion pob, int flg){
    unique_lock <mutex> lck(mtx);
    pob.codificar(flg);

}


void Monitorizacion::descodificar(Poblacion pob,string msg,int flg){
    unique_lock <mutex> lck(mtx);
    pob.descodificar(msg,flg);

}




Poblacion Monitorizacion::crearPob(int n){
    unique_lock <mutex> lck(mtx);
    //No se con cual de las tres ha que llamar

}

void Monitorizacion::dividir(Poblacion pob,int n,Poblacion pobs[]){
    unique_lock <mutex> lck(mtx);
    pob.dividir(n,pobs);
}

Poblacion Monitorizacion::fusionar(Poblacion pobs[]){
    
    //No tengo claro como hacerla
}

int Monitorizacion::mejorFit(Poblacion pob){
    unique_lock <mutex> lck(mtx);
    pob.mejorFit();
}

int Monitorizacion::mediaFit(Poblacion pob){
    unique_lock <mutex> lck(mtx);
    pob.mediaFit(); 
}