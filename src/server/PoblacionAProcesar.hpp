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

	//Selecciona caminantes y los pone en las posiciones desde ini hasta fin
	//n es actualizado con el numero de caminantes seleccionado y si vale numCamOrig
	//numCam tambien recibe ese valor
	void seleccionar(int ini, int fin, int &n);  
	
	// Devuelve la poblacion
	Poblacion getPoblacion();
};

#endif
