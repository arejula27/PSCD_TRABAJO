//*****************************************************************
// File:   caminante.cpp
// Author: Iñigo Arejula & Sergio Cartiel
// Date:   Diciembre 2019/Enero 2020
// Coms:   Implementación de las clases caminante y población
//*****************************************************************



#include "caminante.hpp"
using namespace std;

//Constructor del caminante
Caminante::Caminante()
{
    camino = nullptr;
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
void Caminante::desCodificar(const string MiCamino, int &avance,int max)
{
    avance--;
    delete camino;
    camino = new int[max + 1];
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
    int aux;
    camino = new int[max+1];
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

int getValorMatriz(int **mtr,int i,int j){
    //solo podemos utilizar la mitad superior de la matriz
    return (i>j)?mtr[i][j]:mtr[j][i];
}

void Caminante::calcMiFit(int **dist, int numCiuds)
{
    int recorrido = 0;
    for(int i = 0; i<numCiuds; i++)
    {
        recorrido += getValorMatriz(dist,camino[i],camino[i+1]);
    }
    fitness = 1.00000/recorrido;
}

//Devuelve el fitness del caminante.
float Caminante::MyFit()
{
    return fitness;
}

//Función de mutar.
void Caminante::mutar()
{
}
 
//Modifica el camino del caminante con los genes cruzados de sus padres.
<<<<<<< HEAD
void Caminante::cruzar(const Caminante &O1, const Caminante &O2, const int &numCities)
{
    //Modo 1
    Caminante hijo;
    
    int corte = rand() % numCities; //Gen a partir del cual se va a intercambiar 
    for(int i=0; i<corte; i++){
        
        hijo.camino[i] = O1.camino[i];
    }

    for (int i=corte; i<numCities; i++){
        hijo.camino[i] = O2.camino[i];
    }

    hijo.camino[numCities]=hijo.camino[0];

    /***Modo 2 (habria que implementar una variable para elegir el modo)

    Caminante hijo;
    int aux;
    for(int i=0; i<numCities; i++){
        srand (time(NULL));
        if(rand() % 2 >0.5){ //Genera aleatoriamente 1 ó 0
            hijo.camino[i] = O1.camino[i];
        }
        else{
            hijo.camino[i] = O2.camino[i];
        }
    }

    hijo.camino[numCities]=hijo.camino[0];
    */
=======
void Caminante::cruzar(const Caminante &c1, const Caminante &c2)
{
    camino = c1.camino;
>>>>>>> 2730dc1c650405389869ed8c0a99f98748f97600
}





/**********************************************
 * FUNCIONES POBLACIÓN
 * ********************************************/


Poblacion::Poblacion(){
    numCities = 0;
    numCam = 0;
}

//le indicas cuantos caminantes va a haber y la entrada donde estan los datos
Poblacion::Poblacion(int numCamis, int ciudIni, int numCiuds, string entrada)
{
    //inicializar numCam
    //rellenar la matriz
    //inicializar numCities
    dist= new int*[numCiuds];
    for(int i =0;i<numCiuds;i++){
        dist[i]= new int[i];
        sizeMatrix = sizeof(int)*(i);
    }
    numCities = numCiuds;
    numCam = numCamis;
    srand(time(NULL));
    for (int i = 0; i < numCam; i++)
    {
        caminantes[i].ini(ciudIni, numCiuds);
    }
    
    ifstream f1;
    f1.open(entrada);
    if(f1.is_open()){
        char h[10000];
        do{
            f1.getline(h, 10000);
        }while(h[0] == '#');

        int i=0;
        int contFil = 0;
        int contCol = 0;
        int num;
        int fila =0;
        do{  
            i=0;
            while(h[i]!='\0'){
                //nos saltamos hasta llegar a un numero
                while(!isDigit(h[i])){
                    i++;
                }
                //tomamos el numero
                num = atoi(&h[i]);
                //nos saltamos el numero
                while(isDigit(h[i])){
                    i++;
                }
                //solo guardamos si corresponde con la mitad superior
                if (contFil>contCol){
                    dist[contFil][contCol] = num;
                }
                    
                contCol++;
                if(contCol == numCiuds){
                    contCol = 0;
                    contFil++;
                }
            }
            
        }while(f1.getline(h, 10000));

    }
    else{
        cerr<<"Fichero no encontrado"<<endl;
    }
    f1.close();
}

Poblacion::Poblacion(string data)
{

    int inx = 0;
    numCities = stoi(&data[inx]);
    //delete dist;
    while (data[inx++] != ':');
    dist = new int *[numCities];
    for (int i = 0; i < numCities; i++)
    {
        dist[i] = new int[i];
        sizeMatrix = sizeof(int) * (i);
    }

    descodificarMatriz(data, inx);
    
    numCam = stoi(&data[inx]);
    while (data[inx++] != ':');
    //descodificar todos los viajeros
    for (int i = 0; i < numCam; i++)
    {
        int avz = 0;
        caminantes[i].desCodificar(&data[inx], avz,numCities);
        inx += avz;
    }
    

}

Poblacion::~Poblacion()
{
}

int Poblacion::getNumCam(){
    return numCam;
}

//calculas el fit de un caminante y se lo guardas
void Poblacion::calcFit(Caminante &caminate){
    caminate.calcMiFit(dist, numCities);
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
        getMatrixFrom(*this);
        int numSub = (numCam / n);
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


void Poblacion::fusionar(int n, Poblacion pobs[]){
    int idx = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < pobs[i].getNumCam(); j++)
        {
            caminantes[idx] = pobs[i].caminantes[j];
            idx++;
        }
    }
}

//Devuelve un string que almacena la matriz de distancias de la Poblacion según el siguiente formato:
// "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;) "
string Poblacion::codificarMatriz()
{
    string MiMat = "(";
    for (int i = 0; i < numCities; i++)
    {
        for (int j = 0; (i > j) ; j++)
        {
            
                MiMat += to_string(dist[i][j]) + ((i-1!=j)?',':';');
            
            
        }
      
    }
    MiMat += ")";
  
    return MiMat;
}

//Guarda en dist la matriz de distancias de la Población almacenada en <MiMatriz> según el siguiente formato:
// "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;)"
//Y aumenta <avance> con el número de letras entre "(" y ")", ambos incluidos.
void Poblacion::descodificarMatriz(const string MiMatriz, int &avance)
{
    int fil = 1;
    
    int aux = 0;
  
    avance++;
    while (MiMatriz[avance] != ')')
    {
        //cout << "beep" << endl;
        int col = 0;
        while (MiMatriz[avance] != ';')
        {
            
            while (!isDigit(MiMatriz[avance]))
            {
                avance++;
            }
            aux = stoi(&MiMatriz[avance]);
            while (isDigit(MiMatriz[avance]))
            {
                avance++;
            }
            
            dist[fil][col] = aux;

            //cout << "boop" << endl;
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
//ALL_POB=> "numcities:(matriz)numCam:[caminante]*"
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
       
        //delete dist;
        
        while (msg[inx++] != ':');
        dist = new int *[numCities];
        for (int i = 0; i < numCities; i++)
        {
            dist[i] = new int[i];
            sizeMatrix = sizeof(int) * (i);
        }

        descodificarMatriz(msg, inx);
      
        numCam = stoi(&msg[inx]);
        while (msg[inx++] != ':');
        //descodificar todos los viajeros
        for (int i = 0; i < numCam; i++)
        {
            int avz = 0;
            caminantes[i].desCodificar(&msg[inx], avz,numCities);
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
            caminantes[i].desCodificar(&msg[inx], avz,numCities);
            inx += avz;
        }
    }
}

void Poblacion::getMatrixFrom(Poblacion pob){
   
    numCities = pob.numCities;
    delete dist;
    dist = new int *[numCities];
    for (int i = 0; i < numCities; i++)
    {
        dist[i] = new int[i];
        memcpy(dist[i],pob.dist[i],sizeof(dist[1]));
         sizeMatrix = sizeof(int) * (i);
    }
    
}

Caminante Poblacion::getCaminante(int id) {
  
    assert(id>=0);
    assert(id<numCam);
    return caminantes[id];
}

//aumenta en num el numero de caminantes posibles en la población
void Poblacion::addCams(int num){
    assert(num+numCam <= CAM_MAX);
    numCam+=num;
}


//muta el caminante de la pos num
void Poblacion::mutar(int num){

    caminantes[num].mutar();


}

//cruza los caminantes de la pos p1,p2 y coloca al hijo el ultimo de la
//población, para que funcione la población no puede tener CAM_MAX caminantes
void Poblacion::cruzar(int p1,int p2){
<<<<<<< HEAD
    assert(1+numCam > CAM_MAX);
    caminantes[numCam].cruzar(caminantes[p1],caminantes[p2], numCities);
=======
    assert(1+numCam <= CAM_MAX);
    caminantes[numCam].cruzar(caminantes[p1],caminantes[p2]);
>>>>>>> 2730dc1c650405389869ed8c0a99f98748f97600
    numCam+=1;

}

void Poblacion::seleccionar(){
    
}