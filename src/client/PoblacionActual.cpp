#include "PoblacionActual.hpp"

//n = numero de personas
PobActual:: PobActual(int gen){
    historico= new int*[gen];
    for (int i = 0; i < gen; i++)
    {
        historico[i]=new int [2];
    }
    
    numGen = 0;
    sync = new bool[gen];
    for (int i = 0; i < gen; i++)
    {
        sync[i] = false;
    }
}

PobActual:: ~PobActual(){

}


int PobActual::numGeneracion(){
    return numGen;
}

void PobActual::agnadirDatos(int numGen, int mejorFit, int avgFit){
    unique_lock <mutex> lck(mtx);
    historico[numGen][0]=mejorFit;
    historico[numGen][1]=avgFit;
}
//añadir funcion bool que indique cuando acabar
bool PobActual::finEjec(Poblacion &personas){
    unique_lock <mutex> lck(mtx);
    #warning darle valor a fit para calcular el % IGUAL VAR GLOAL
    float fit,mejorFit,media;
    float porcentaje = personas.stats(fit,mejorFit,media);
    if (porcentaje <= 3.0){
        return true;
    }else{
        return false;
    }
}
//haced sleep de GA y despierta estadistico 
void PobActual::esperaEstadistico(){
    unique_lock <mutex> lck(mtx);
    dormir_estadistico.notify_all();
    dormir_GA.wait(lck);
}

// esperar GA control hace signal de ga xq ya ha cogido la poblacion y 
//wait de control estadistico
void PobActual::esperaGA(){
    unique_lock <mutex> lck(mtx);
    dormir_GA.notify_all();
    dormir_estadistico.wait(lck);
}
//Guarda los datos de la poblacion actual en el string datosCompartidos para 
//que luego tengan acceso a ellos otras funciones.
void PobActual::guardarDatos(string info){
    unique_lock <mutex> lck(mtx);
    datosCompartidos = info;
    dormirServ.notify_all();
}
//Lee los datos de la poblacion que hay almacenados en el string datosCompartidos 
//y los devuelve por referecia;
void PobActual::extraerDatos(string &datos){
    unique_lock <mutex> lck(mtx);
    datos = datosCompartidos;
}
//Duerme al proceso ServCliente
void PobActual::dormirSer(){
    unique_lock <mutex> lck(mtx);
    dormirServ.wait(lck);
}
//Si quieres calcular las estadisitcas de tu poblacion y todavia no se han calculado las
//de la poblacion anterior, te duermes hasta que la posicion del vector anterir a tu id este a true,
// es decir, se hayan calculados las estadisticas de la poblacion anterior
void PobActual::syncro(int id){
    unique_lock <mutex> lck(mtx);
    if (id !=1){
        while (!sync[id-2]){
            calcEstadistico.wait(lck);
        }
    }
}
//Cuando acaban de calcularse las estadisticas de la poblacion n, pone a true su poscion 
//en el vector de calculados y notifica a todos los procesos dormidos.
void PobActual::finProceso(int id){
    unique_lock <mutex> lck(mtx);
    sync[id-1] = true;
    calcEstadistico.notify_all();
}

void PobActual::despertarTodos(){
    unique_lock <mutex> lck(mtx);
    calcEstadistico.notify_all();
    dormir_estadistico.notify_all();
    dormir_GA.notify_all();
}
int PobActual::maxCli(){
	unique_lock<mutex> lck(mtx);
	return clientes_max;
}

void PobActual::dormir(){
	unique_lock<mutex> lck(mtx);
	//se dormira hasta que todos los clientes hayan sido notificados
	//que el avión esta lleno
	esperando.wait(lck);
}

void PobActual::nuevoCl(){ 
	unique_lock<mutex> lck(mtx);
	clientes_conect++;
	clientes_max++;
}

void PobActual::endPr(){
	unique_lock<mutex> lck(mtx);
	if(--clientes_conect ==0){
		//si se han cerrado todos los clientes
		//avisar al proceso finalizador
		esperando.notify_one();
	}