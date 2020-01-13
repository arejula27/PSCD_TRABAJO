//*****************************************************************
// File:	PoblacionAprocesar.cpp
// Date:	Diciembre 2019
//*****************************************************************

#include "PoblacionAProcesar.hpp"

//----------------------------------------------------------
PoblacionAProcesar::PoblacionAProcesar(Poblacion poblacion_) {
	poblacion = poblacion_;
}


//------------------------------------------------------------------------------------------------
// funcion cruzar
void PoblacionAProcesar::cruzar(int id, int id2) {     // Poner parametros para llamar a cruzar
    unique_lock<mutex> lck(mtx);
  
    
    poblacion.cruzar(id,id2);

}

//------------------------------------------------------------------------------------------------
// funcion mutar   
void PoblacionAProcesar::mutar(int id) {
    unique_lock<mutex> lck(mtx);
    cout<<id<<endl;
      static int cont =0;
      cout<<"mutar: "<<cont<<endl;
      cont++;
    poblacion.mutar(id);
}

//------------------------------------------------------------------------------------------------
// funcion seleccionar
void PoblacionAProcesar::seleccionar() {   
    unique_lock<mutex> lck(mtx);
    poblacion.seleccionar(poblacion.getNumCamOrig());
}


// Devuelve la poblacion
Poblacion PoblacionAProcesar::getPoblacion() {
    return poblacion;
}

//Guarda el numCam actual en numCamOrig de la poblacion
void PoblacionAProcesar::setNumCamOrig() {
    
    poblacion.setNumCamOrig();
}
