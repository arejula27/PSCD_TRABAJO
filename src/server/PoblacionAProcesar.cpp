//*****************************************************************
// File:	PoblacionAprocesar.cpp
// Date:	Diciembre 2019
//*****************************************************************

#include "PoblacionAProcesar.hpp"

//----------------------------------------------------------
PoblacionAProcesar::PoblacionAProcesar(Poblacion poblacion_) {
	this->poblacion = poblacion_;
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
    poblacion.mutar(id);
}

//------------------------------------------------------------------------------------------------
// funcion seleccionar
void PoblacionAProcesar::seleccionar(int ini, int fin, int &n) {   
    unique_lock<mutex> lck(mtx);
    poblacion.seleccionar(ini,fin,n);
    cout<<"Seleccionado hasta el "<<n<<endl;
    
    
}


// Devuelve la poblacion
Poblacion PoblacionAProcesar::getPoblacion() {
    return this->poblacion;
}
