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
    
    fitness = -1;
}

Caminante::~Caminante()
{
}

//Devuelve true si y sólo si <c> es un dígito ([0-9])
 bool isDigit(char c){
     bool res = true;
     if (c<'0') res =false;
     if (c> '9') res = false;
     return res;
 }


//Actualiza camino del caminante según la cadena <MiCamino>, que tendrá el formato:
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
//Y actualiza <avance> con el número de letras entre "NumCiud1" y "fitness;", todo incluido.
void Caminante::desCodificar(const string MiCamino, int &avance,int max)
{
    
    int i = 0;  //Iterador ciudades
    while (MiCamino[avance] != ':')         //Hasta que (Todas las ciudades guardadas) i=max
    {
        
        while (!isDigit(MiCamino[avance]))      //Saltamos ","
        {
            avance++;
        }
   
        camino[i] = stoi(&MiCamino[avance]);    //Guardamos la ciudad en camino[i]

        while (isDigit(MiCamino[avance]))       //Saltamos Ciud i
        {
            avance++;
        }
        i++;            //Una ciudad más guardada
        
    }   //Todas las ciudades guardadas.
    
    camino[i] = camino[0];      //Última ciudad recorrida siempre = a la primera.
    avance++;   //Saltando ":"
   
    fitness= stof(&MiCamino[avance]);   //Guardando el fitness del caminante
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
  
    int inicio = camino[0];     //Guardamos la primera ciudad.
  
    string MiCamino = to_string(inicio) + ',';      //String que se devolverá.
    int i = 1;
  
    while (camino[i] != inicio)         //Hasta llegar a la última ciudad recorrida por el caminante (igual a la de inicio).
    {
        
        MiCamino += to_string(camino[i]) + ',';
        i++;                            //Guardamos caminantes según el formato
    }
    MiCamino += to_string(inicio);
   
    MiCamino += ':' + to_string(fitness) + ';';     //":Fitness;"
    
    return MiCamino;
}

//Actualiza el caminante con un camino aleatorio partiendo de <inicio> con <max> número de ciudades.
void Caminante::ini(int inicio, int max)
{
    int aux;
  
    bool recorridos[max];               //Vector indicativo de si se ha recorrito la ciudad (iter)
    for (int i = 0; i < max; i++)
    {
        recorridos[i] = false;
    }
    recorridos[inicio] = true;          //La primera ciudad es <inicio>
    camino[0] = inicio;
    for (int i = 1; i < max; i++)
    {
        aux = rand() % max;         //Ciudad aleatoria.
        while (recorridos[aux])     
        {
            aux = (aux + 1) % max;      //Si ya se ha recorrido se intenta con la siguiente, así hasta encontrar una sin recorrer.
        }
        recorridos[aux] = true;
        camino[i] = aux;
    }
    camino[max] = inicio;       //Última ciudad a recorrer siempre es la primera.
}

int getValorMatriz(int **mtr,int i,int j){
    //solo podemos utilizar la mitad superior de la matriz
    return (i>j)?mtr[i][j]:mtr[j][i];
}

//Pre: dist es un puntero a una matriz con las distancias entre las ciudades que recorre el caminante.
//Post: Calcula el fitness del caminante según las ciudades que recorre.
//      fitness = 1000/(Distancia total recorrida)
void Caminante::calcMiFit(int **dist, int numCiuds)
{
    int recorrido = 0;

    for(int i = 0; i<numCiuds; i++)
    {                               //Obtiene la distancia que recorre de la matriz
        recorrido += getValorMatriz(dist,camino[i],camino[i+1]);
    }
    fitness = 1000.00000/recorrido;
}

//Devuelve el fitness del caminante.
float Caminante::MyFit()
{
    return fitness;
}

//Función de mutar.
void Caminante::mutar(const int numCities)
{

    //MODO1
    int genes[numCities - 1];
    bool cogidos[numCities - 1];
    //Almacenamos los genes intercambiables para no perder ninguno
    for (int i = 0; i < numCities - 1; i++)
    {
        genes[i] = camino[i + 1];
        cogidos[i] = false;
    }
    int j=0;
    int random;

    for(int i=1; i<numCities; i++){
        random=rand()%(numCities-1);
        while(cogidos[random]) random = (random + 1)%(numCities-1);
        camino[i]=genes[random];//Elige un gen entre todos los almacenados
        cogidos[random] = true;
    }
  
}
//Modifica el camino del caminante con los genes cruzados de sus padres.
void Caminante::cruzar(const Caminante &c1, const Caminante &c2, const int numCities)
{   
    bool elegido[numCities];
    camino[0]=c1.camino[0];
    for (int i = 0; i < numCities; i++)
    {   
        elegido[i]=false;
    }
    elegido[camino[0]]=true;
    for (int  i = 1; i < numCities; i++)
    {
        camino[i]=(c1.camino[i]+c2.camino[i])%numCities;
        while(elegido[camino[i]]){
           camino[i]=(camino[i]+1)%numCities;    
        }
        elegido[camino[i]]=true;   
    }
    camino[numCities]=camino[0];
}


//Devuelve true si y solo si el camino no tiene ciudades repetidas salvo el inicio y fin




/**********************************************
 * FUNCIONES POBLACIÓN
 * ********************************************/


Poblacion::Poblacion(){
    numCities = 0;
    numCam = 0;
    dist=nullptr;
    caminantes=nullptr;
    maxCami=0;
    
}

//Inicializa la población con <numCamis> caminantes, los cuales empiezan su camino en
//<ciudIni> y recorren <numCiuds> ciudades. 
//Y almacena en la población la matriz de distancias para <numCiuds> ciudades almacenada en el
//fichero <entrada>.
Poblacion::Poblacion(int numCamis, int ciudIni, int numCiuds, string entrada)
{
    maxCami=2*numCamis;                 //Se crearán numCam caminantes más en cruzar.
    caminantes=new Caminante[maxCami];  //Vector dinámico de caminantes
    numCam=numCamis;
    //rellenar la matriz
    //inicializar numCities
    dist= new int*[numCiuds]; //Guarda espacio para una matriz diagonal para <numCiuds> ciudades.
    for(int i =0;i<numCiuds;i++){
        dist[i]= new int[i];
        sizeMatrix = sizeof(int)*(i);
    }
    numCities = numCiuds;     //Guarda en la población el número de ciudades que hay.
    
    srand(time(NULL));
    
    for (int i = 0; i < numCam; i++)
    {
  
        caminantes[i].ini(ciudIni, numCiuds);       //Inicializa todos sus caminantes.
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
        exit(0);
    }
    f1.close();
}

//Inicializa una población y actualiza sus datos con el string <data> que ha sido devuelto por otra población
//con la función codificar(), que sigue el formato:
//"(dist10;dist20,dist21; ... ;distn0,distn1, ... ,distn(n-2),distn(n-1);)numCam:Cam1,Cam2, ... , CamN;"
Poblacion::Poblacion(string data)
{

    int inx = 0;            //Iterador para recorrer <data>
    numCities = stoi(&data[inx]);
    

    while (data[inx++] != ':');
    dist = new int *[numCities];
    for (int i = 0; i < numCities; i++)
    {
        dist[i] = new int[i];
        sizeMatrix = sizeof(int) * (i);
    }

    descodificarMatriz(data, inx);      //Descodifica y guarda la matriz.

    numCam = stoi(&data[inx]);
    //int extra = numCam * 20 / 100;
    maxCami = 2 * numCam;
    caminantes = new Caminante[maxCami];
    while (data[inx++] != ':');
    for (int i = 0; i < numCam; i++)
    {
        int avz = 0;                //Descodifica y guarda los caminantes.
        caminantes[i].desCodificar(&data[inx], avz,numCities);
        inx += avz;
    }
    

}

Poblacion::~Poblacion()
{
}

//Devuelve el número de ciudades en la población.
int Poblacion::getNumCities(){
    return numCities;
}

//Devuelve el número de caminantes en la población
int Poblacion::getNumCam(){
    return numCam;
}

//Devuelve el número de caminantes original de la población (porque si se ha ejecutado
//cruzar el número de caminantes de la población sería el doble del numero de caminantes que tenía 
//inicialmente, es decir, <numCamOrig>)
int Poblacion::getNumCamOrig(){
    return numCamOrig;
}

//Iguala a el número de caminantes actuales <numCamOrig>
void Poblacion::setNumCamOrig(){
    numCamOrig=numCam;
}
//Se calcula el fit de todos los caminantes de la población y se actualizan con este.
void Poblacion::calcFit(){
    for (int  i = 0; i < numCam ; i++)
    {
        if(numCities==0){
            exit(1);}               //<numCam> llamadas a calcMiFit con la matriz de distancias almacenada en la población.
        caminantes[i].calcMiFit(dist, numCities);
    }
    
   
}

//Calcula el fit del caminante <caminate> con la matriz de distancias de la población.
void Poblacion::calcFit(Caminante &caminate){
    caminate.calcMiFit(dist, numCities);
}

//Devuelve el porcentaje de caminantes que son mejores que el fit que le introducimos,
//tambien por mejorFit devuelve el fitness del mejor caminante y por media la
//media de fitness de los caminantes
////Si hay un caminante con mejor fit que <caminoMejorFit> se actualiza <caminoMejorFit> con este.
float Poblacion::stats(float fit,float &mejorFit,float &media){
    int cont = 0;
    int idMejor=0;
    for (int i = 0; i < numCam; i++){
        calcFit(caminantes[i]); // Calcula el fit
        if (caminantes[i].MyFit() >= fit){ //si el fit es mayor que el umbral suma
            cont++;
        }
        //Calculo del mejor fit
        if (caminantes[i].MyFit() >= mejorFit){
            mejorFit = caminantes[i].MyFit();
            idMejor=i;
        }
        //calculo de la media
        media += caminantes[i].MyFit();
    }
    if(mejorFit>mejorFitEver){
        mejorFitEver=mejorFit;
        caminoMejorFit=caminantes[idMejor].codificar();
    }
    
    media = media/numCam;
    //Devuelve el porcentaje
    return (cont*100)/numCam;
}

//Devuelve el mejor caminante que ha habido en la población.
void Poblacion::mejCam(){

    cout<<"El mejor camino logrado es:"<<endl;
    cout<<caminoMejorFit<<endl;
}

//Actualiza el vector de Poblacion <pobs> de tamaño n la poblacion dividida en n subpoblaciones 
//guardándolas en el array, además se guarda en cada subpoblación la matriz de distancias
void Poblacion::dividir(int n, Poblacion pobs[])
{
    
    int sobr = numCam % n;
    int indx = 0;
    for (int i = 0; i < n; i++)
    {
        pobs[i].numCities=numCities;        //Actualiza el valor de numCities de las tres poblaciones.

        pobs[i].getMatrixFrom((*this));     //Copia la matriz de distancias.
        int numSub = (numCam / n);          //Número de caminantes por subpoblación
        if (sobr > 0)
        {
            numSub++;
            sobr--;
        }
        pobs[i].numCam = numSub;
                                    //Da a cada subpoblación sus valores de numCam y maxCami.
        pobs[i].maxCami= pobs[i].numCam * 2;        

        if (pobs[i].caminantes == nullptr){
            pobs[i].caminantes = new Caminante[maxCami];    //Inicializa los vectores de los caminantes.
        }
        for (int j = 0; j < numSub; j++)
        {
    
            pobs[i].caminantes[j] = caminantes[j + indx];     //Iguala los caminantes de cada subpoblación con aquellos que le corresponden.
        }
        indx += numSub;
    }

}

void Poblacion::fusionar(int n, Poblacion pobs[])
{
    int idx = 0;
    int numC=0;
    for (int i = 0; i < n; i++)
    {
        numC += pobs[i].getNumCam();
    }
    maxCami = 2 * numC;

    

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
// "(dist10;dist20,dist21;, ... , ;distn0,distn1, ... , distn(n-2), distn(n-1);)"
//Y aumenta <avance> con el número de letras entre "(" y ")", ambos incluidos.
void Poblacion::descodificarMatriz(const string MiMatriz, int &avance)
{
    int fil = 1;
    
    int aux = 0;
  
    avance++;
    while (MiMatriz[avance] != ')')
    {
       
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
    string msg="";
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
            msg += caminantes[i].codificar();
         
        }

        break;
        case MATRX:
            msg = to_string(numCities) + ":";
            msg += codificarMatriz();
            break;

        case NCIT:
            msg = to_string(numCities);
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

        if (dist == nullptr)
        {
            dist = new int *[numCities];
            for (int i = 0; i < numCities; i++)
            {
                dist[i] = new int[i];
                sizeMatrix = sizeof(int) * (i);
            }
        }

        while (msg[inx++] != ':');
        dist = new int *[numCities];
        for (int i = 0; i < numCities; i++)
        {
            dist[i] = new int[i];
            sizeMatrix = sizeof(int) * (i);
        }
      
        descodificarMatriz(msg, inx);

        numCam = stoi(&msg[inx]);
        maxCami = 2 * numCam;
        
        assert(numCam<=maxCami);
        if (caminantes == nullptr)
           caminantes = new Caminante[maxCami];

        while (msg[inx++] != ':');
        //descodificar todos los viajeros
        for (int i = 0; i < numCam; i++)
        {

            caminantes[i].desCodificar(msg, inx,numCities);
        }
    }

    else if (flg == UPGRADE_POB)
    {
        static bool primer =true;
        numCam = stoi(msg);
        static int cnt=0;
        cnt++;
        if(primer){
            maxCami = 2 * numCam;
            primer=false;
            if (caminantes == nullptr)
                {
                    caminantes = new Caminante[maxCami];
                }
            
               
        }
        
        
        assert(numCam<=maxCami);
       
        while (msg[inx++] != ':');
        //descodificar todos los viajeros
        for (int i = 0; i < numCam; i++)
        {
            caminantes[i].desCodificar(msg, inx,numCities);
        }
    }
    else if (flg == MATRX)
    {

        numCities = stoi(msg);
        
        if(dist==nullptr){
            dist = new int *[numCities];
            for (int i = 0; i < numCities; i++)
            {
                dist[i] = new int[i];
                sizeMatrix = sizeof(int) * (i);
            }
        }
        int avz=0;
        descodificarMatriz(&msg[msg.find('(')+1],avz);
        
    }
     else if (flg == NCIT)
    {

        numCities = stoi(msg);
        
       
        
    }
}

void Poblacion::getMatrixFrom(Poblacion pob){
   
    numCities = pob.numCities;

    if (dist == nullptr)
    {
        dist = new int *[numCities];
        for (int i = 0; i < numCities; i++)
        {
            dist[i] = new int[i];
            sizeMatrix = sizeof(int) * (i);
        }
    }
    for (int i = 0; i < numCities; i++)
    {
        for (int j = 0; j < i; j++)
        {
            dist[i][j]=pob.dist[i][j];
        }
        
    }
    
}

Caminante Poblacion::getCaminante(int id) {
  
    assert(id>=0);
    assert(id<numCam);
    return caminantes[id];
}

//aumenta en num el numero de caminantes posibles en la población
void Poblacion::addCams(int num){
    assert(num+numCam <= maxCami);
    numCam+=num;
}


//muta el caminante de la pos num
void Poblacion::mutar(int num){
    
    caminantes[num].mutar(numCities);


}

//cruza los caminantes de la pos p1,p2 y coloca al hijo el ultimo de la
//población, para que funcione la población no puede tener CAM_MAX caminantes
void Poblacion::cruzar(int p1,int p2){
    
    assert(1 + numCam <= maxCami);

   
    caminantes[numCam].cruzar(caminantes[p1],caminantes[p2], numCities);
    numCam+=1;
}

void Poblacion::seleccionar(){
    Caminante selected[numCamOrig];
    double casillaCam[numCam]; //Almacena en prob[i] la longitud de su casilla
    double fit;
    double totalCasillas = 0; //"Unidades" o casillas acumuladas en la ruleta
    double bola;

    for (int i = 0; i < numCam; i++)
    {
  
        fit = caminantes[i].MyFit();
        casillaCam[i] = fit + totalCasillas; //La longitud/probabilidad de la casilla lo determina el fit
        totalCasillas = fit + totalCasillas; //Se aumenta el tamaño de la ruleta
    }
    int i;
    bool elegido;

    for (int tirada = 0; tirada < numCamOrig;)
    {

        bola = totalCasillas * drand48();
        //Recorrer para comprobar resultado
        elegido = false;
        i = 0;
        while (i < numCam && !elegido)
        {

            if (casillaCam[i] >= bola)
            {
                selected[tirada] = caminantes[i];
                tirada++;
                elegido = true;
            }
            i++;
        }
    }
    
    //BUCLE PARA COPIAR LOS ELEGIDOS DONDE CORRESPONDE
    
    
    
    
    for(int i=0; i<numCamOrig; i++){
        
        caminantes[i]= selected[i];

    }
    numCam=numCamOrig;
    
}

void Poblacion::seleccionar_v2(){

    Caminante selected[numCamOrig];

    int nVeces=4; //Numero de torneos
    int k=numCam/nVeces; //Numero de participantes en cada torneo
    int l=numCamOrig/nVeces; //elegidos en cada torneo

    
    int lextra=numCamOrig%nVeces;
    double fit;
    bool elegido[numCam];
    int posicion;
    int numElegTor; //numero de elegidos en el torneo actual
    int j;
     for(int i=0; i<numCam ; i++){
        elegido[i]=false;
    }
    int numElegidos=0;
   

    for(int i=0;i<nVeces;i++){ //Bucle para cada torneo
        numElegTor=0;
        j=k*i;
        while(numElegTor<l){   //Seleccion dentro de cada torneo
            if (!elegido[j])   //Coger uno no elegido
            {
            
            fit=caminantes[j].MyFit(); //Se compara su fit con el de todos
            for(int p=j;p<k*(i+1);p++){   //Elige un participante cuyo fit es el mayor de los no elegidos
                
                
                if(fit<=caminantes[p].MyFit() && !elegido[p]){
                    fit=caminantes[p].MyFit();
                    
                    posicion=p;
                }

            }
            elegido[posicion]=true;
            selected[numElegidos]=caminantes[posicion];
            numElegTor++;
            numElegidos++;

            }
            j++;               
        }

    }

    //Torneo con los restantes
    numElegTor=0;
    while(numElegTor<lextra){   //Seleccion dentro de cada torneo
            j=k*nVeces;
            if (!elegido[j] && j<numCam){ //Coger uno no elegido
                
                calcFit(caminantes[j]);
                fit=caminantes[j].MyFit(); //Se compara su fit con el de todos
                            
                for(int p=j;p<numCam;p++){   //Elige un participante cuyo fit es el mayor de los no elegidos
                    calcFit(caminantes[p]);
                    if(fit<=caminantes[p].MyFit() && !elegido[p]){
                        fit=caminantes[p].MyFit();
                        posicion=p;
                    }
                }
                
                elegido[posicion]=true;
                selected[numElegidos]=caminantes[posicion];
                numElegTor++;
                numElegidos++;

            }
            j++;               
        }
}