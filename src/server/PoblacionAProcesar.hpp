//*****************************************************************
// File:	PoblacionAProcesar.hpp
// Date:	Diciembre 2019
//*****************************************************************

#ifndef POBLACIONAPROCESAR_H
#define POBLACIONAPROCESAR_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <thread>
//#include "caminante.hpp"

using namespace std;

class PoblacionAProcesar {
private:
	string poblacion;	// string que contiene Sub-poblacion recibida
	int numCaminantes;	// numero de caminantes en la poblacion
	Caminante caminantes[numCaminantes];	// vector de caminantes 


	condition_variable cond;
	mutex mtx;


public:
	//Inicialización del monitor
	PoblacionAProcesar(string poblacion_);
	void seleccionar();
	void cruzar();
	void mutar();
};

#endif
