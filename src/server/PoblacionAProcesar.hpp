//*****************************************************************
// File:	PoblacionAProcesar.hpp
// Date:	Diciembre 2019
//*****************************************************************

#ifndef POBLACIONAPROCESAR_H
#define POBLACIONAPROCEAR_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <thread>
#include "caminante.hpp"

using namespace std;

class PoblacionAProcesar {
private:
	Poblacion poblacion;
	int numCaminantes;	// numero de caminantes en la poblacion

	condition_variable cond;
	mutex mtx;


public:
	//  Inicialización del monitor, se le pasa el vector de caminantes para
	//para operar sobre ellos 
	PoblacionAProcesar(Poblacion poblacion_);
	void seleccionar();
	void cruzar();
	void mutar();
	// Devuelve la poblacion
	Poblacion getPoblacion();
};

#endif
