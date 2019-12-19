//*****************************************************************
// File:	PoblacionAprocesar.cpp
// Date:	Diciembre 2019
//*****************************************************************

#include "PoblacionAProcesar.hpp"

//----------------------------------------------------------
PoblacionAProcesar::PoblacionAProcesar(Poblacion poblacion_) {
	this->poblacion = poblacion_;

    for(int i=0; i<numCaminantes; i++) {
        //caminantes[i] = obtener de poblacion
    }
};


//----------------------------------------------------------
// funcion seleccionar
void PoblacionAProcesar::seleccionar() {
    unique_lock<mutex> lck(mtx);

}

//----------------------------------------------------------
// funcion cruzar
void PoblacionAProcesar::cruzar() {     // Poner parametros para llamar a cruzar
    unique_lock<mutex> lck(mtx);
    //caminante.Cruzar(&o1,&o2);

}

//----------------------------------------------------------
// funcion mutar   
void PoblacionAProcesar::mutar() {
    unique_lock<mutex> lck(mtx);
    //caminante.Mutar();

}

//----------------------------------------------------------

/*
// Devuelve la poblacion
Poblacion PoblacionAProcesar::getPoblacion() {
    return this->poblacion;
}
*/