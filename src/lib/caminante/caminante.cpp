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

    int i = 0;
    while (MiCamino[avance] != ':')
    {
        
        while (!isDigit(MiCamino[avance]))
        {
            avance++;
        }
   
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
double Caminante::MyFit()
{
    return fitness;
}

//Función de mutar.
void Caminante::mutar(const int numCities)
{

    //MODO1

    /*
    int genes[numCities-1];
    bool cogidos[numCities-1];
    //Almacenamos los genes intercambiables para no perder ninguno
    for(int i=0; i<numCities-1; i++){
        genes[i]=camino[i+1];
        cogidos[i]=false;
    }
    int j=0;
    int random;
  
    for(int i=1; i<numCities; i++){
        random=rand()%(numCities-1);
        while(cogidos[random]) random = (random + 1)%(numCities-1);
        camino[i]=genes[random];//Elige un gen entre todos los almacenados
        cogidos[random] = true;
    }
    */

   //MODO2

    int random=rand()%(numCities-1)+1;
  

  
    


    for(int i=1; i<numCities; i++){
      
        camino[i]=(camino[i]+random)%(numCities);
        
    }
    
    

}




//Modifica el camino del caminante con los genes cruzados de sus padres.
void Caminante::cruzar(const Caminante &c1, const Caminante &c2, const int numCities)
{
    //Modo 1: antes los genes anteriores a camino[corte] son los de c1 y los posteriores los de c2
    
    /*
    
    int corte = rand() % numCities; //Gen a partir del cual se va a intercambiar
    for(int i=0; i<corte; i++){
        camino[i] = c1.camino[i];
    }
    for (int i=corte; i<numCities; i++){
        camino[i] = c2.camino[i];
        if (!esValido(i)){
            camino[i] = c1.camino[i];
        }
    }
 
    */    

    //Modo 2: cada gen se elige aleatoriamente entre c1 y c2. En caso de estar repetido
    //se elige un gen cualquiera no repetido


    
   

   bool vect[numCities];
   for (int  i = 0; i < numCities; i++)
   {
       vect[i]=false;
   }
   vect[c2.camino[numCities]]=true;
   camino[0] = c2.camino[numCities];
   camino[numCities] = c2.camino[numCities];

   for (int i = 1; i < numCities; i++)
   {

       int num = (c1.camino[i] + c2.camino[i]) % (numCities);
       while (vect[num])
       {

           num++;
           num = num % numCities;
       }
       vect[num] = true;

       camino[i] = num;
      
       //camino[i] = (c1.camino[i] + c2.camino[i]) % (numCities);

       /* while(!esValido(i+1)){
            camino[i]=(camino[i]+1)%(numCities);
        }*/
    }
    
    




}

//Devuelve true si y scacaminominoolo si el camino no tiene ciudades repetidas salvo el inicio y fin
bool Caminante::esValido(int numCities){
    bool valido=true;
    int j=1;
    if(camino[1]==0) valido=false;
    while(j<numCities-1 && valido){
        for(int i = j+1; i<numCities ; i++){
            if(camino[j]==camino[i]) valido = false;
            if(camino[i]==0) valido = false;
        }
        j++;
    }
    return valido;
}




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

//le indicas cuantos caminantes va a haber y la entrada donde estan los datos
Poblacion::Poblacion(int numCamis, int ciudIni, int numCiuds, string entrada)
{
    //inicializar numCam
    int extra=numCamis*20/100;
    maxCami=extra+2*numCamis;
    caminantes=new Caminante[maxCami];
    numCam=numCamis;
    //rellenar la matriz
    //inicializar numCities
    dist= new int*[numCiuds];
    for(int i =0;i<numCiuds;i++){
        dist[i]= new int[i];
        sizeMatrix = sizeof(int)*(i);
    }
    numCities = numCiuds;
    
    
    
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
        exit(0);
    }
    f1.close();
}


Poblacion::Poblacion(string data)
{

    int inx = 0;
    numCities = stoi(&data[inx]);
    

    while (data[inx++] != ':');
    dist = new int *[numCities];
    for (int i = 0; i < numCities; i++)
    {
        dist[i] = new int[i];
        sizeMatrix = sizeof(int) * (i);
    }

    descodificarMatriz(data, inx);

    numCam = stoi(&data[inx]);
    int extra = numCam * 20 / 100;
    maxCami = extra + 2 * numCam;
    caminantes = new Caminante[maxCami];
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

int Poblacion::getNumCities(){
    return numCities;
}

int Poblacion::getNumCam(){
    return numCam;
}

int Poblacion::getNumCamOrig(){
    return numCamOrig;
}


//Iguala numCamOrig al valor actual de numCam
void Poblacion::setNumCamOrig(){
    numCamOrig=numCam;
}

//calculas el fit de un caminante y se lo guardas
void Poblacion::calcFit(Caminante &caminate){
    caminate.calcMiFit(dist, numCities);

}

//Devuelve el porcentaje de caminantes que son mejores que el fit que le introducimos,
//tambien por mejorFit devuelve el fitness del mejor caminante y por media la
//media de fitness de los caminates
double Poblacion::stats(double fit,double &mejorFit,double &media){
    int cont = 0;
    for (int i = 0; i < numCam; i++){
        calcFit(caminantes[i]); // Calcula el fit
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

        //getMatrixFrom(*this);
        int numSub = (numCam / n);
        if (sobr > 0)
        {
            numSub++;
            sobr--;
        }
        pobs[i].numCam = numSub;
        pobs[i].maxCami=maxCami;

        if (pobs[i].caminantes == nullptr){
            pobs[i].caminantes = new Caminante[maxCami];
        }
        for (int j = 0; j < numSub; j++)
        {
    
            pobs[i].caminantes[j] = caminantes[j + indx];
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
    int extra = numC * 20 / 100;
    maxCami = extra + 2 * numC;

    //cout << endl;
   //cout << "antes" << endl;
    //cout << (*this).codificar() << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < pobs[i].getNumCam(); j++)
        {  
            // cout<<caminantes[idx].codificar()<<"   "<<pobs[i].caminantes[j].codificar()<<endl;
            caminantes[idx] = pobs[i].caminantes[j];
           // cout << caminantes[idx].codificar() << endl;
            idx++;
        }
        }
        //cout  << endl;
        //cout << "despues"<< endl;
       // cout<<(*this).codificar()<<endl;
        //cout << pobs[i].getNumCam() << endl;
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
            msg += caminantes[i].codificar();
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
       
        if(dist!=nullptr){
            for (int i = 1; i < numCities; i++)
            {
                delete[] dist[i];
            }
        }
        
        
       
        delete [] dist;
      
        while (msg[inx++] != ':');
        dist = new int *[numCities];
        for (int i = 0; i < numCities; i++)
        {
            dist[i] = new int[i];
            sizeMatrix = sizeof(int) * (i);
        }
        descodificarMatriz(msg, inx);

        numCam = stoi(&msg[inx]);
        int extra = numCam * 20 / 100;
        maxCami = extra + 2 * numCam;
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

        numCam = stoi(msg);
      
        assert(numCam<=maxCami);
       
        while (msg[inx++] != ':');
        //descodificar todos los viajeros
  
      
        for (int i = 0; i < numCam; i++)
        {
         
            caminantes[i].desCodificar(msg, inx,numCities);
        }
      
    }
}

void Poblacion::getMatrixFrom(Poblacion pob){
   
    numCities = pob.numCities;

    if (dist != nullptr)
    {
        for (int i = 1; i < numCities; i++)
        {
            delete[] dist[i];
        }
    }
    delete [] dist;
   
    dist = new int *[numCities];
    for (int i = 0; i < numCities; i++)
    {
        
        dist[i] = new int[i];
        
        if(i>0)memcpy(dist[i],pob.dist[i],sizeof(dist[i]));
        
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
//Cruce con doble corte (Double Point Crossover)
/*void Poblacion::cruzar2(int p1, int p2){

    assert(1 + numCam <= maxCami);  

    caminantes[numCam].cruzar2(caminantes[p1],caminantes[p2], numCities);
    numCam+=1;
}*/
void Poblacion::seleccionar_v2()
{
    int random;
    //BUCLE DE ELEGIR
    Caminante selected[numCamOrig];
    for (int tirada = 0; tirada < numCamOrig; tirada++)
    {
       
        random = rand() % (numCam);
        selected[tirada] = caminantes[random];
        cout << "seleccionado--" << random;
    }
}

void Poblacion::seleccionar()
{

    Caminante selected[numCamOrig];
    

    //MODO1 (RULETA)
    /*
    double casillaCam[numCam]; //Almacena en prob[i] la longitud de su casilla
    double fit;
    double totalCasillas=0;  //"Unidades" o casillas acumuladas en la ruleta 
    double bola; 

    

    for(int i=0; i<numCam ; i++){
        calcFit(caminantes[i]);
        fit=caminantes[i].MyFit();
        //cout<<"fit"<<i<<"---"<<prob;
        casillaCam[i]=fit+totalCasillas; //La longitud/probabilidad de la casilla lo determina el fit
        totalCasillas=fit+totalCasillas; //Se aumenta el tamaño de la ruleta
        //cout<<"/////"<<"fit acumulado:"<<totalCasillas<<endl;
    }
    
    int i;
    bool elegido;


    for(int tirada = 0; tirada<n;){
        
        bola= totalCasillas*drand48();
        //Recorrer para comprobar resultado
        elegido=false;
        i=0;
        while(i<numCam && !elegido){

            if(casillaCam[i]>=bola){
                
                //cout<<"En el turno "<<tirada<<" ha caido en la casilla "<<casillaCam[i]<<"--caminante["<<i<<"]"<<endl;
                selected[tirada]=caminantes[i];
                tirada++;
                elegido=true;
            }
            i++;
        }
    }
    cout<<"Metodo de seleccion por ruleta(1)"<<endl;
    
    */
   
   //MODO2 (RANDOM)
    /*
    int random;
    //BUCLE DE ELEGIR 

    for(int tirada = 0; tirada<n;tirada++){
       
        random = rand()%(numCam);
        selected[tirada]=caminantes[random];
        cout<<"seleccionado--"<<random;
    }

    cout<<"Seleccionar version random(2)\n";
    */
   
   //MODO 3 (TORNEO)
    int nVeces=4; //Numero de torneos
    int k=numCam/nVeces; //Numero de participantes en cada torneo
    int l=numCamOrig/nVeces; //elegidos en cada torneo

    
    int lextra=numCamOrig%nVeces;

    cout<<"part="<<k<<" -- eleg="<<l<<endl;
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
        cout<<"----INICIO TORNEO "<<i<<"----["<<k*i<<"-"<<k*(i+1)<<"]----"<<l<<endl;
        j=k*i;
        while(numElegTor<l){   //Seleccion dentro de cada torneo
            
           // cout<<"hola"<<endl;
            if (!elegido[j])   //Coger uno no elegido
            {
            calcFit(caminantes[j]);
            fit=caminantes[j].MyFit(); //Se compara su fit con el de todos
           // cout << "hola" << endl;
            for(int p=j;p<k*(i+1);p++){   //Elige un participante cuyo fit es el mayor de los no elegidos
                calcFit(caminantes[p]);
                
                if(fit<=caminantes[p].MyFit() && !elegido[p]){
                    fit=caminantes[p].MyFit();
                    
                    posicion=p;
                }

            }
            //cout << "hola" << endl;
            elegido[posicion]=true;
            cout<<"Va a guardar el"<<posicion<<endl;
            selected[numElegidos]=caminantes[posicion];
            numElegTor++;
            numElegidos++;

            }
            cout<<j<<"-";
            j++;               
        }

    }

    //Torneo con los restantes
    numElegTor=0;
    while(numElegTor<lextra){   //Seleccion dentro de cada torneo
            j=0;
            cout<<"ENTRA EN EL TORNEO AUXILIAR"<<endl;
            if (!elegido[j+(k*nVeces)] && j+(k*nVeces)<numCam){ //Coger uno no elegido
                
                calcFit(caminantes[j+(k*nVeces)]);
                fit=caminantes[j+(k*nVeces)].MyFit(); //Se compara su fit con el de todos
                            
                for(int p=j;p<numCam;p++){   //Elige un participante cuyo fit es el mayor de los no elegidos
                    calcFit(caminantes[p]);
                    if(fit<caminantes[p].MyFit() && !elegido[p]){
                        fit=caminantes[p].MyFit();
                        posicion=p;
                    }
                }
                
                elegido[posicion]=true;
                cout<<"Va a guardar el"<<posicion<<endl;
                selected[numElegidos]=caminantes[posicion];
                numElegTor++;
                numElegidos++;

            }
            j++;               
        }
    
    //BUCLE PARA COPIAR LOS ELEGIDOS DONDE CORRESPONDE
    
    for(int i=0; i<numCamOrig; i++){
        
        caminantes[i]= selected[i];
        //caminantes[i].calcMiFit;

    }
    numCam=numCamOrig;
    
}
