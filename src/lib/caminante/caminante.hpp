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



class Caminante
{
    //friend void Poblacion::calcFit();

private:
    int camino[400];
    double fitness;
public:

    Caminante();
    ~Caminante();

    //Inicializa el caminante de forma aleatoria partiendo de <inicio> con <max> número de ciudades.
    void ini(int inicio, int max);

    //Guarda en caminante el camino según la cadena <MiCamino>, que tendrá de formato:
    // "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
    //Y actualiza <avance> con el número de letras entre "NumCiud1" y "fitness;", todo incluido.
    void desCodificar(const string MiCamino, int &avance, int max);

    //Devuelve el camino del caminante según la cadena <MiCamino>, que tendrá de formato:
    // "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
    string codificar();

    void calcMiFit(int **dist, int numCiuds);

    //Devuelve el fitness del caminante.
    double MyFit();

    bool esValido(int numCities);

    //Función de mutar.
    void mutar(const int numCities);

    //Modifica el camino del caminante con los genes cruzados de sus padres.
    void cruzar(const Caminante &c1, const Caminante &c2, const int numCities);
    //Cruce con doble corte (Double Point Crossover)
    void cruzar2(const Caminante &c1, const Caminante &c2, const int numCities);

    //Devuelve true si y solo si el camino no tiene ciudades repetidas salvo el inicio y fin
    bool esValido(int *camino, const int numCities);
};




class Poblacion
{
    friend Caminante;
  
private:
    int maxCami;
    int sizeMatrix;
    int numCities;
    int **dist;
    int numCam;
    Caminante *caminantes;
    

public:
    //constructo predeterminado
    Poblacion();
    //constructor para inicializar una poblacion a partir de un archivo de texto
    //con una matriz, se le debe de indicar el numero de caminantes de la pobaclion
    //con maximo CAM_MAX
    Poblacion(int numCamis, int ciudIni, int numCiuds, string entrada = "entrada.txt");

    //Tras recibir una poblacion de un cliente/servidor, usar este constructor para inicilizar
    Poblacion(string data);

    ~Poblacion();

    int getNumCities();
    
    int getNumCam();
    //calcula el fit de un caminante y cambia su propio fit
    void calcFit(Caminante &caminante);
    //Devuelve el porcentaje de caminantes que son mejores que el fit que le introducimos,
    //tambien por mejorFit devuelve el fitness del mejor caminante y por media la
    //media de fitness de los caminates
    double stats(double fit, double &mejorFit, double &media);
    //divide la poblacion en n subpoblaciones
    void dividir(int n, Poblacion pobs[]);

    //fuisona la poblacion en n subpoblaciones
    void fusionar(int n, Poblacion pobs[]);
    //Devuelve un string que almacena la matriz de distancias de la Poblacion según el siguiente formato:
    // "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;)"
    string codificarMatriz();

    //Guarda en dist la matriz de distancias de la Población almacenada en <MiMatriz> según el siguiente formato:
    // "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;)"
    //Y aumenta <avance> con el número de letras entre "(" y ")", ambos incluidos.
    void descodificarMatriz(const string MiMatriz, int &avance);

    //envia la poblacion, usar ALL_POB para codificar la poblacion con todos sus datos o
    //UPGRADE_POB para codificar ÚNICAMENTE los caminates
    string codificar(int flg = ALL_POB);

    void descodificar(string mag, int flg = ALL_POB);

    //toma la matriz de otra poblacion
    void getMatrixFrom(Poblacion pob);

    // Devuelve un caminante dado un id
    Caminante getCaminante(int id);

    //aumenta en num el numero de caminantes posibles en la población
    void addCams(int num);

    //muta el caminante de la pos num
    void mutar(const int numCities);

    //cruza los caminantes de la pos p1,p2 y coloca al hijo el ultimo de la
    //población, para que funcione la población no puede tener CAM_MAX caminantes
    void cruzar(int p1, int p2);
    //Cruce con doble corte (Double Point Crossover)
    //Se realizan dos cortes en el camino de los padres, para generar la descendencia 
    //se inserta la parte centra de un padre en el otro y viceversa
    void cruzar2(int p1, int p2);

    void seleccionar(int n);
};

#endif //!CAMINANTE_Y_POBLACION