//*****************************************************************
// File:	PoblacionAprocesar.cpp
// Date:	Diciembre 2019
//*****************************************************************

#include "PoblacionAProcesar.hpp"

//----------------------------------------------------------
PoblacionAProcesar::PoblacionAProcesar(string poblacion_) {
	this->poblacion = poblacion_;
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
