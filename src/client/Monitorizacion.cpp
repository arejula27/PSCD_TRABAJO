#include "caminante.hpp"
#include "Monitorizacion.hpp"

Monitorizacion:: Monitorizacion(){

}

Monitorizacion:: ~Monitorizacion(){

}

void Monitorizacion::calculoFit(Caminante &caminante){
    unique_lock <mutex> lck(mtx);
}

int Monitorizacion::mejorFit(){
    unique_lock <mutex> lck(mtx);
    Poblacion.mejorFit();
}

int Monitorizacion::avgFit(){
    unique_lock <mutex> lck(mtx); 
}