#include <iostream>
#include "caminante.hpp"

using namespace std;

//Constructor del caminante
Caminante::Caminante()
{
    camino[0] = -1;
    fitness = -1;
}

Caminante::~Caminante()
{
}


 bool isDigit(char c){
     bool res = true;
     if (c<'0') res =false;
     if (c> '9') res = false;
     return res;
 }


//Guarda en caminante el camino según la cadena <MiCamino>, que tendrá de formato:
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
//Y actualiza <avance> con el número de letras entre "NumCiud1" y "fitness;", todo incluido.
void Caminante::desCodificar(const string MiCamino, int &avance)
{
    avance--;
    int i = 0;
    while (MiCamino[avance] != ':')
    {
        avance++;
        camino[i] = stoi(&MiCamino[avance]);
        while (isDigit(MiCamino[avance]))
        {
            avance++;
        }
        i++;
    }
    camino[i] = camino[0];
    avance++;
    stof(&MiCamino[avance]);
    while (MiCamino[avance] != ';')
    {
        avance++;
    }
    avance++;
}

//Devuelve el camino del caminante según la cadena <MiCamino>, que tendrá de formato:
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
string Caminante::codificar()
{
    int inicio = camino[0];
    string MiCamino = to_string(inicio) + ',';
    int i = 1;
    while (camino[i] != inicio)
    {
        MiCamino += to_string(camino[i]) + ',';
        i++;
    }
    MiCamino += to_string(inicio);
    MiCamino += ':' + to_string(fitness) + ';';
    return MiCamino;
}

//Inicializa el caminante de forma aleatoria partiendo de <inicio> con <max> número de ciudades.
void Caminante::ini(int inicio, int max)
{
    srand(time(0));
    int aux;
    bool recorridos[max];
    for (int i = 0; i < max; i++)
    {
        recorridos[i] = false;
    }
    recorridos[inicio] = true;
    camino[0] = inicio;
    for (int i = 1; i < max; i++)
    {
        aux = rand() % max;
        while (recorridos[aux])
        {
            aux = (aux + 1) % max;
        }
        recorridos[aux] = true;
        camino[i] = aux;
    }
    camino[max] = inicio;
}

void Caminante::calcMiFit(int dist[CITY_MAX][CITY_MAX], int numCiuds)
{
    int recorrido = 0;
    for(int i = 0; i<numCiuds; i++)
    {
        recorrido += dist[i][i+1];
    }
    fitness = 1/recorrido;
}

//Devuelve el fitness del caminante.
float Caminante::MyFit()
{
    return fitness;
}

//Función de mutar.
void Caminante::Mutar()
{
}
 
//Modifica el camino del caminante con los genes cruzados de sus padres.
void Caminante::Cruzar(const Caminante &O1, const Caminante &O2)
{
}





/**********************************************
 * FUNCIONES POBLACIÓN
 * ********************************************/

//le indicas cuantos caminantes va a haber y la entrada donde estan los datos
Poblacion::Poblacion(int numCam, int ciudIni, int numCiuds, string entrada)
{
    //inicializar numCam
    //rellenar la matriz
    //inicializar numCities
    numCities = numCiuds;
    for (int i = 0; i < numCam; i++)
    {
        caminantes[i].ini(ciudIni, numCiuds);
    }
    ifstream f1;
    f1.open(entrada);
    if(f1.is_open()){
        char h[1000000];
    h[0] = '0';
    while(h[0] == '#'){
        f1.getline(h, 1000000, '\n');
    }
    int i=0;
    int contFil = 0;
    int contCol = 0;
    int num;
   while(f1.getline(h, 1000000, '\n')){      
        i=0;
        while(h[i]!='\0'){
            while(h[i]==' '){
                i++;
            }
            num = atoi(&h[i]);
            while(h[i]!= ' ' && h[i]!='\0'){
                i++;
            }
            dist[contFil][contCol] = num;
            contCol++;
            if(contCol == numCiuds){
                contCol = 0;
                contFil++;
            }
        } 
    }

    }
    else{
        cerr<<"Fichero no encontrado"<<endl;
    }
    
    f1.close();
}

Poblacion::Poblacion(string data)
{

    int inx = 0;
    

    numCities = stoi(data);
    while (data[inx++] != ':');
    descodificarMatriz(data, inx);
    numCam = stoi(&data[inx]);
    while (data[inx++] != ':');
    //descodificar todos los viajeros
    for (int i = 0; i < numCam; i++)
    {
        int avz = 0;
        caminantes[i].desCodificar(&data[inx], avz);
        inx += avz;
    }
    

}

Poblacion::~Poblacion()
{
}

int Poblacion:: getNumCam(){
    return numCam;
}

//calculas el fit de un caminante y se lo guardas
void Poblacion::calcFit(Caminante &caminate){
    
}
//Devuelve el porcentaje de caminantes que son mejores que el fit que le introducimos,
//tambien por mejorFit devuelve el fitness del mejor caminante y por media la 
//media de fitness de los caminates
float Poblacion::stats(Poblacion &subPob,float fit,float &mejorFit,float &media){
    int cont = 0;
    for (int i = 0; i < numCam; i++){
        subPob.calcFit(caminantes[i]); // Calcula el fit
        if (caminantes[i].MyFit() >= fit){ //si el fit es mayor que el umbral suma
            cont++;
        }
        //Calculo del mejor fit
        if (caminantes[i].MyFit() >= mejorFit){
            mejorFit = caminantes[i].MyFit();
        }
        //calculo de la media
        media += caminantes[i].MyFit();
    }
    media = media/numCam;
    //Devuelve el porcentaje
    return (cont*100)/numCam;
}

//divide la poblacion en n subpoblaciones y las devuelve en array
void Poblacion::dividir(int n, Poblacion pobs[])
{
    int sobr = numCam % n;
    int indx = 0;

    for (int i = 0; i < n; i++)
    {
        pobs[i].numCities = numCities;

        memcpy(pobs[i].dist, dist, sizeof(dist));
        int numSub = (numCam / n) - 1;
        if (sobr > 0)
        {
            numSub++;
            sobr--;
        }
        pobs[i].numCam = numSub;
        //¿optimizar el bucle con memcopy?
        for (int j = 0; j < numSub; j++)
        {
            pobs[i].caminantes[j] = caminantes[j + indx];
        }
        indx += numSub;
    }
}

//Devuelve un string que almacena la matriz de distancias de la Poblacion según el siguiente formato:
// "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;) "
string Poblacion::codificarMatriz()
{
    string MiMat = "(";
    for (int i = 0; i < numCities; i++)
    {
        for (int j = 0; j < numCities - 1; j++)
        {
            MiMat += dist[i][j] + ',';
        }
        MiMat += dist[i][numCities - 1] + ';';
    }
    MiMat += ")";
    return MiMat;
}

//Guarda en dist la matriz de distancias de la Población almacenada en <MiMatriz> según el siguiente formato:
// "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;)"
//Y aumenta <avance> con el número de letras entre "(" y ")", ambos incluidos.
void Poblacion::descodificarMatriz(const string MiMatriz, int &avance)
{
    int fil = 0;
    int col = 0;
    int aux = 0;
    avance++;
    while (MiMatriz[avance] != ')')
    {
        while (MiMatriz[avance] != ';')
        {
            aux = stoi(&MiMatriz[avance]);
            while (MiMatriz[avance] != ',' && MiMatriz[avance] != ';')
            {
                avance++;
            }
            avance++;
            dist[fil][col] = aux;
            col++;
            aux = 0;
        }
        fil++;
        avance++;
    }
    avance++;
}

//transforma los datos de la poblacion en un único string, por defecto toda
//con UPGRADE_POB, unicamente el vector de caminantes
//ALL_POB=>
//UPGRADE_POB=> "numCam:[caminante]*"
string Poblacion::codificar(int flg)
{
    string msg;
    switch (flg)
    {
    case ALL_POB:
        msg = to_string(numCities) + ":";
        msg += codificarMatriz();
        msg += to_string(numCam) + ":";
        for (int i = 0; i < numCam; i++)
        {
            msg += caminantes[i].codificar();
        }

        break;
    case UPGRADE_POB:
        msg = to_string(numCam) + ":";
        for (int i = 0; i < numCam; i++)
        {
            msg += caminantes[i].codificar() + ";";
        }

        break;
    }

    return msg;
}

void Poblacion::descodificar(string msg, int flg)
{

    int inx = 0;
    if (flg == ALL_POB)
    {

        numCities = stoi(msg);
        while (msg[inx++] != ':')
            ;
        descodificarMatriz(msg, inx);
        numCam = stoi(&msg[inx]);
        while (msg[inx++] != ':')
            ;
        //descodificar todos los viajeros
        for (int i = 0; i < numCam; i++)
        {
            int avz = 0;
            caminantes[i].desCodificar(&msg[inx], avz);
            inx += avz;
        }
    }

    else if (flg == UPGRADE_POB)
    {

        numCam = stoi(msg);
        while (msg[inx++] != ':')
            ;
        //descodificar todos los viajeros
        for (int i = 0; i < numCam; i++)
        {
            int avz = 0;
            caminantes[i].desCodificar(&msg[inx], avz);
            inx += avz;
        }
    }
}

void Poblacion::getMatrixFrom(Poblacion pob){

    memcpy(dist,pob.dist,sizeof(dist));

}

Caminante Poblacion::getCaminante(int id) {
    return caminantes[id];
}

//aumenta en num el numero de caminantes posibles en la población
void Poblacion::addCams(int num){
    assert(num+numCam > CAM_MAX);
    numCam+=num;
}


//muta el caminante de la pos num
void Poblacion::mutar(int num){

    caminates[num].mutar();


}

//cruza los caminantes de la pos p1,p2 y coloca al hijo el ultimo de la
//población, para que funcione la población no puede tener CAM_MAX caminantes
void cruzar(int p1,int p2){
    assert(1+numCam > CAM_MAX);
    caminantes[numCam].cruzar(caminantes[p1],caminantes[p2])
    numCam+=1;

}