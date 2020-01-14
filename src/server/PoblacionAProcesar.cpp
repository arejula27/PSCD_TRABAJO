//*****************************************************************
// File:   PoblacionAProcesar.cpp
// Author: Íñigo Aréjula, Sergio Cartiel, Aaron Ibáñez, Héctor García y Marcos Hernando
// Date:   Noviembre-Enero 2019
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
    unique_lock<mutex>lck(mtx);
    poblacion.mutar(id);
}

//------------------------------------------------------------------------------------------------
// funcion mutar
void PoblacionAProcesar::mutar_v2(int id, float media)
{
    unique_lock<mutex> lck(mtx);
    poblacion.mutar_v2(id);
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

float PoblacionAProcesar::media(){

    return poblacion.media();
}


