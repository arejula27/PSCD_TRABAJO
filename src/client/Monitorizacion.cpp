#include "caminante.hpp"
#include "Monitorizacion.hpp"


Estadistico::Estadistico(int n){
    historico[n][2];
}


int Estadistico::mejorFit(Poblacion pob){
    unique_lock <mutex> lck(mtx);
    pob.mejorFit();
}

int Estadistico::mediaFit(Poblacion pob){
    unique_lock <mutex> lck(mtx);
    pob.mediaFit(); 
}

void Estadistico::agnadirDatos(int numGen, int mejorFit, int avgFit){
    unique_lock <mutex> lck(mtx);
    historico[numGen][0]=mejorFit;
    historico[numGen][1]=avgFit;
}
