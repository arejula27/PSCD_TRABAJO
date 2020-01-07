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
<<<<<<< HEAD
	void seleccionar(int id);
	void cruzar(int id, int id2);
	void mutar(int id);
=======

	void seleccionar();

	void cruzar(int id, int id2);

	void mutar(int id);

>>>>>>> 2730dc1c650405389869ed8c0a99f98748f97600
	// Devuelve la poblacion
	Poblacion getPoblacion();
};

#endif
