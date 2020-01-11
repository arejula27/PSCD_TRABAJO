#ifndef PoblacionActual
#define PoblacionActual

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "caminante.hpp"

using namespace std;
//Numero maximo de generaciones que crearemos

class PobActual
{
public:
    //Construye el monitor
    PobActual(int gen);
    //Destructor
    ~PobActual();
    //Crea poblaciones según el parámetro n (llama a uno de los 3 constructores)
    Poblacion crearPob(int n, int caminantes, string entrada);
    //Divide la poblacion en n subpoblaciones
    void dividir(Poblacion pob,int n, Poblacion pobs[]);
    //Devuelve una poblacionn fruto de fusionar pobs[]
    Poblacion fusionar(Poblacion pobs[]);
    //Prepara los datos para ser enviados por el socket
    void codificar(Poblacion pob,int flg = ALL_POB);
    //flg: 1=ALL_POB 2=UPGRADE_POB
    void descodificar(Poblacion pob,string mag, int flg);
    //Devuelve el numero de generacion hasta el momento
    int numGeneracion();
    //Inserta los datos en la matriz del estadistico
    void agnadirDatos(int numGen, int mejorFit, int avgFit);
    //añadir funcion bool que indique cuando acabar
    bool finEjec(Poblacion &personas);
    //haced sleep de GA y despierta estadistico 
    void esperaEstadistico();
    // esperar GA control hace signal de ga xq ya ha cogido la poblacion y 
    //wait de control estadistico
    void esperaGA();
    //Si quieres calcular las estadisitcas de tu poblacion y todavia no se han calculado las
    //de la poblacion anterior, te duermes hasta que la posicion del vector anterir a tu id este a true,
    // es decir, se hayan calculados las estadisticas de la poblacion anterior
    void syncro(int id);
    //Cuando acaban de calcularse las estadisticas de la poblacion n, pone a true su poscion 
    //en el vector de calculados y notifica a todos los procesos dormidos.
    void finProceso(int id);
    //Guarda los datos de la poblacion actual en el string datosCompartidos para 
    //que luego tengan acceso a ellos otras funciones.
    void guardarDatos(string info);
    //Lee los datos de la poblacion que hay almacenados en el string datosCompartidos 
    //y los devuelve por referecia;
    void extraerDatos(string &datos);
    //Duerme al proceso ServCliente
    void dormirSer();

    void dormir();
    int maxCli();
    void nuevoCl();
    void endPr();
    void despertarTodos();
    
private:
    int **historico;
    mutex mtx;
    string datosCompartidos;
    int numGen; //Número de poblaciones generadas hasta el momento
    //Variables condicion para sincronizar estadisitico y GA
    condition_variable dormir_estadistico,dormir_GA,dormirServ;
    condition_variable calcEstadistico,esperando;
    bool sync[MAX_GENS];
    int clientes_max;
    int clientes_conect;
    
};
#endif






