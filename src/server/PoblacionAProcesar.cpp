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
void PoblacionAProcesar::seleccionar() {
    unique_lock<mutex> lck(mtx);

}

//----------------------------------------------------------
// funcion cruzar
void PoblacionAProcesar::cruzar(int id, int id2) {     // Poner parametros para llamar a cruzar
    unique_lock<mutex> lck(mtx);
<<<<<<< HEAD
    Caminante cam = poblacion.getCaminante(id);
    Caminante cam2 = poblacion.getCaminante(id2);
    cam.cruzar(cam,cam2);
=======
    poblacion.cruzar(id,id2);
>>>>>>> 2730dc1c650405389869ed8c0a99f98748f97600

}

//----------------------------------------------------------
// funcion mutar   
void PoblacionAProcesar::mutar(int id) {
    unique_lock<mutex> lck(mtx);
<<<<<<< HEAD
    Caminante cam = poblacion.getCaminante(id);
    cam.mutar();
=======
    poblacion.mutar(id);
>>>>>>> 2730dc1c650405389869ed8c0a99f98748f97600
}

//----------------------------------------------------------


// Devuelve la poblacion
Poblacion PoblacionAProcesar::getPoblacion() {
    return this->poblacion;
}
