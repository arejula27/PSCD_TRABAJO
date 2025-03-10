//*****************************************************************
// File:   PoblacionAProcesar.hpp
// Author: Íñigo Aréjula, Sergio Cartiel, Aaron Ibáñez, Héctor García y Marcos Hernando
// Date:   Noviembre-Enero 2019
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

	void mutar_v2(int id, float media);

	void seleccionar();  
	
	void seleccionar_v2(); 
	
	// Devuelve la poblacion
	Poblacion getPoblacion();

	void setNumCamOrig();

	int getNumCamOrig();

	float media();

};

#endif
