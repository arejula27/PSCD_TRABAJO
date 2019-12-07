
#include <iostream> 

using namespace std;

class Caminante
{
private:
    // ATRIBUTOS
    // Almacena el número de coudades que visita
    // y dichas ciudades en el orden que las recorre
    int N;
    
    string ruta[N]; //puntero a la lista de ciudades

public:
    Caminante(/* args */);
    ~Caminante();
};
//CONSTRUCTORES
//Constructor de la clase a partir de un
//único string, conocienda el numero  de ciudades
Caminante(string listCitys, int N)
{
    //debe divir las ciudades separadas por comas
    //guardandolos en un vector, Si el valor de N es negativo
    //esto indiacará que el caminante es erróneo
    this->N = N;
    ruta = new string[N];
    #warning INTRODUCIR LAS CIUDADES EN EL VECTOR

}
//Constructor de clase a partir de una única cadena de char
//esta se utilizará para crear un individuo al recivirlo
//de otro nodo
Caminante(const char* data)
{

}
~Caminante()
{
    delete ruta;
}
serializar(string msg) {
    msg = to_string(N)+;
    for (int i=0i<N<i++){
        *ruta[i]
    }

}

bool mutarMitad(){

}

bool reproducirMitad(Caminante padre, Caminante madre){


}


