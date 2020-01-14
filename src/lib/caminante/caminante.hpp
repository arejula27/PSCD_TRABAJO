//*****************************************************************
// File:   caminante.hpp
// Author: Iñigo Arejula & Sergio Cartiel
// Date:   Diciembre 2019/Enero 2020
// Coms:   Especificación de las clases caminante y población
//*****************************************************************



#ifndef CAMINANTE_Y_POBLACION
#define CAMINANTE_Y_POBLACION




#include <string.h>
#include <fstream>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
using namespace std;


const int UPGRADE_POB =0;
const int ALL_POB =1;
const int MATRX =2;
const int NCIT =4;
const int MAX_NUM_CIUDADES = 400


class Caminante
{
    //friend void Poblacion::calcFit();

private:
    int camino[MAX_NUM_CIUDADES];        //Vector para almacenar el camino del caminante.
    float fitness;          //Fitness del caminante 
public:

    Caminante();
    ~Caminante();

    //Actualiza el caminante con un camino aleatorio partiendo de <inicio> con <max> número de ciudades.
    void ini(int inicio, int max);

    //Actualiza camino del caminante según la cadena <MiCamino>, que tendrá el formato:
    // "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
    //Y actualiza <avance> con el número de letras entre "NumCiud1" y "fitness;", todo incluido.
    void desCodificar(const string MiCamino, int &avance, int max);

    //Devuelve el camino del caminante según la cadena <MiCamino>, que tendrá de formato:
    // "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
    string codificar();

    //Pre: dist es un puntero a una matriz con las distancias entre las ciudades que recorre el caminante.
    //Post: Calcula el fitness del caminante según las ciudades que recorre.
    //      fitness = 1000/(Distancia total recorrida)
    void calcMiFit(int **dist, int numCiuds);

    //Devuelve el fitness del caminante.
    float MyFit();

    //Función de mutar.
    void mutar(const int numCities);

    //Función de mutar.
    void mutar_v2(const int numCities, float media);

    //Modifica el camino del caminante con los genes cruzados de sus padres.
    void cruzar(const Caminante &c1, const Caminante &c2, const int numCities);

};




class Poblacion
{
    friend Caminante;
  
private:
    double mejorFitEver;    //Mejor fitness que ha habido en la población.
    string caminoMejorFit;  //Mejor caminante que ha habido en la población.
    int maxCami;            //Máximo número de caminantes que se pueden llegar a almacenar en la población.
    int sizeMatrix;         //Tamaño de la matriz dinámica en bytes.
    int numCities;          //Número de ciudades que recorren los caminantes de la población.
    int **dist;             //Matriz de distancias de las ciudades de la población.
    int numCam;             //Número de caminantes almacenados en la población.
    int numCamOrig;         //Número de caminantes actualizable con el valor actual de <numCam>.
    Caminante *caminantes;  //Vector dinámico de caminantes que almacena los caminantes de la población.
    

public:
    //Constructor por defecto
    Poblacion();


    //Inicializa la población con <numCamis> caminantes, los cuales empiezan su camino en
    //<ciudIni> y recorren <numCiuds> ciudades. 
    //Y almacena en la población la matriz de distancias para <numCiuds> ciudades almacenada en el
    //fichero <entrada>.
    Poblacion(int numCamis, int ciudIni, int numCiuds, string entrada = "entrada.txt");

    //Inicializa una población y actualiza sus datos con el string <data> que ha sido devuelto por otra población
    //con la función codificar(), que sigue el formato:
    //"(dist10;dist20,dist21; ... ;distn0,distn1, ... ,distn(n-2),distn(n-1);)numCam:Cam1,Cam2, ... , CamN;"
    Poblacion(string data);

    //Destructor
    ~Poblacion();

    //Devuelve el número de ciudades en la población.
    int getNumCities();
    
    //Devuelve el número de caminantes en la población
    int getNumCam();

    //Devuelve el número de caminantes original de la población (porque si se ha ejecutado
    //cruzar el número de caminantes de la población sería el doble del numero de caminantes que tenía 
    //inicialmente, es decir, <numCamOrig>)
    int getNumCamOrig();

    //Iguala a el número de caminantes actuales <numCamOrig>
    void setNumCamOrig();

    //Se calcula el fit de todos los caminantes de la población y se actualizan con este.
    void calcFit();

    //Calcula el fit del caminante <caminate> con la matriz de distancias de la población.
    void calcFit(Caminante &caminate);

    //Devuelve el porcentaje de caminantes que son mejores que el fit que le introducimos,
    //tambien por mejorFit devuelve el fitness del mejor caminante y por media la
    //media de fitness de los caminantes
    //Si hay un caminante con mejor fit que <caminoMejorFit> se actualiza <caminoMejorFit> con este.
    float stats(float fit, float &mejorFit, float &media);

    //Actualiza el vector de Poblacion <pobs> de tamaño n la poblacion dividida en n subpoblaciones 
    //guardándolas en el array, además se guarda en cada subpoblación la matriz de distancias
    void dividir(int n, Poblacion pobs[]);

    //Fusiona n poblaciones almacenadas en <pobs> guardando todos sus caminantes en la población que invoca la función.
    void fusionar(int n, Poblacion pobs[]);

    //Devuelve un string que almacena la matriz de distancias de la Poblacion según el siguiente formato:
    // "(dist10;dist20,dist21; ... ;distn0,distn1, ... ,distn(n-2),distn(n-1);)"
    string codificarMatriz();

    //Guarda en dist la matriz de distancias de la Población almacenada en <MiMatriz> según el siguiente formato:
    // ""(dist10;dist20,dist21; ... ;distn0,distn1, ... ,distn(n-2),distn(n-1);)""
    //Y aumenta <avance> con el número de letras entre "(" y ")", ambos incluidos.
    void descodificarMatriz(const string MiMatriz, int &avance);

    ///Transforma los datos de la poblacion en un único string, en distintos formatos según el flag <flg>:
    //con UPGRADE_POB, unicamente el vector de caminantes
    //ALL_POB=> "(dist10;dist20,dist21; ... ;distn0,distn1, ... ,distn(n-2),distn(n-1);)numCam:Cam1,Cam2, ... , CamN;"
    //UPGRADE_POB=> "numCam:Cam1,Cam2, ... , CamN;"
    //MATRX => "(dist10;dist20,dist21; ... ;distn0,distn1, ... ,distn(n-2),distn(n-1);)"
    //NCIT => "numCities"
    //ALL_POB por defecto.
    string codificar(int flg = ALL_POB);

    //Actualiza los datos de la poblacion según el string <msg>, sigue distintos formatos según el flag <flg>:
    //con UPGRADE_POB, unicamente el vector de caminantes
    //ALL_POB=> "(dist10;dist20,dist21; ... ;distn0,distn1, ... ,distn(n-2),distn(n-1);)numCam:Cam1,Cam2, ... , CamN;"
    //UPGRADE_POB=> "numCam:Cam1,Cam2, ... , CamN;"
    //MATRX => "(dist10;dist20,dist21; ... ;distn0,distn1, ... ,distn(n-2),distn(n-1);)"
    //NCIT => "numCities"
    //ALL_POB por defecto.
    void descodificar(string mag, int flg = ALL_POB);

    //Guarda en la población que llama la función la matriz dist de la Poblacion <pob>.
    void getMatrixFrom(Poblacion pob);

    //Pre: id>=0 && id<Poblacion.getNumCams()
    //Devuelve el caminante número <id> de la población.
    Caminante getCaminante(int id);

    //Pre: <num>+Poblacion.getNumCams() < Población.maxCami()
    //Aumenta en num el numero de caminantes posibles en la población
    void addCams(int num);

    //muta el caminante de la pos num
    void mutar(const int numCities);

    void mutar_v2(const int numCities);

    //cruza los caminantes de la pos p1,p2 y coloca al hijo el ultimo de la
    //población, para que funcione la población no puede tener CAM_MAX caminantes
    void cruzar(int p1, int p2);

    //Selecciona numCamOrig caminantes por el metodo de la ruleta
    void seleccionar();

    //Selecciona numCamOrig caminantes por el metodo del torneo
    void seleccionar_v2();

    //Devuelve el mejor caminante que ha habido en la población.
    void mejCam();

    //Devuelve la media de los fitness de los caminantes almacenados en la población.
    float media();

    
};

#endif //!CAMINANTE_Y_POBLACION