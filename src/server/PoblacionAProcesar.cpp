//*****************************************************************
// File:	PoblacionAprocesar.cpp
// Date:	Diciembre 2019
//*****************************************************************

#include "PoblacionAProcesar.hpp"

//----------------------------------------------------------
PoblacionAProcesar::PoblacionAProcesar(Poblacion poblacion_) {
	this->poblacion = poblacion_;
};


//----------------------------------------------------------
// funcion seleccionar
void PoblacionAProcesar::seleccionar(int id) {
    unique_lock<mutex> lck(mtx);

}

//----------------------------------------------------------
// funcion cruzar
void PoblacionAProcesar::cruzar(int id, int id2) {     // Poner parametros para llamar a cruzar
    unique_lock<mutex> lck(mtx);
    Caminante cam = poblacion.getCaminante(id);
    Caminante cam2 = poblacion.getCaminante(id2);
    cam.Cruzar(cam,cam2);

}

//----------------------------------------------------------
// funcion mutar   
void PoblacionAProcesar::mutar(int id) {
    unique_lock<mutex> lck(mtx);
    Caminante cam = poblacion.getCaminante(id);
    cam.Mutar();
}

//----------------------------------------------------------


// Devuelve la poblacion
Poblacion PoblacionAProcesar::getPoblacion() {
    return this->poblacion;
}
