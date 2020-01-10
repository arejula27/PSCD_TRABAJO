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
#include "../lib/caminante/caminante.hpp"

using namespace std;

class PoblacionAProcesar {
private:
	Poblacion poblacion;

	condition_variable cond;
	mutex mtx;


public:
	//  Inicialización del monitor, se le pasa el vector de caminantes para
	//para operar sobre ellos 
	PoblacionAProcesar(Poblacion poblacion_);

	void cruzar(int id, int id2);

	void mutar(int id);

	void seleccionar();  //QUITAR CUANDO SE VAYA A QUITAR LO DE ABAJO///
	//void seleccionar(int id); //DESCOMENTAR CUANDO ESTE HECHO SELECCIONAR///
	
	// Devuelve la poblacion
	Poblacion getPoblacion();
};

#endif
