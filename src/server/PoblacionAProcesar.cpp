//*****************************************************************
// File:	PoblacionAprocesar.cpp
// Date:	Diciembre 2019
//*****************************************************************

#include "PoblacionAProcesar.hpp"

//----------------------------------------------------------
PoblacionAProcesar::PoblacionAProcesar(Poblacion poblacion_) {
	poblacion = poblacion_;
   // cout << poblacion.codificar(UPGRADE_POB)<< endl;
   // cout << "enCon" << endl;
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
    unique_lock<mutex>lck(mtx);
    poblacion.mutar(id);
}

//------------------------------------------------------------------------------------------------
// funcion seleccionar
void PoblacionAProcesar::seleccionar() {   
    unique_lock<mutex> lck(mtx);
  
   
    
    poblacion.seleccionar();
}

void PoblacionAProcesar::seleccionar_v2() {   
    unique_lock<mutex> lck(mtx);
    poblacion.seleccionar_v2();
}


// Devuelve la poblacion
Poblacion PoblacionAProcesar::getPoblacion() {
    return poblacion;
}

void PoblacionAProcesar::setNumCamOrig(){
    poblacion.setNumCamOrig();
    
}

int PoblacionAProcesar::getNumCamOrig(){
    
    return poblacion.getNumCamOrig();
    
}


