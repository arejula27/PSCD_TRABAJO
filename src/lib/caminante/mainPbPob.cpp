//*****************************************************************
// File:   mainPbPob.cpp
// Author: Iñigo Arejula & Sergio Cartiel
// Date:   Diciembre 2019/Enero 2020
// Coms:   Prueba funciones de las clases caminante y población
//*****************************************************************

#include "caminante.hpp"

#define E_NONE 0
#define ERROR 1

#define ANSI_COLOR_RESET "\x1b[0m" /**< Desactiva color del terminal */
#define ANSI_COLOR_RED "\x1b[31m"  /**< Pone terminal a rojo */
#define ANSI_COLOR_GREEN "\x1b[32m" /**< Pone terminal a verde */

using namespace std;



//si error == E_NONE devuelve correcto sino imprimira como error el msg
string  error(int error, string msg="")
{
	
	if(error ==E_NONE)
		return ANSI_COLOR_GREEN "todo correcto" ANSI_COLOR_RESET;
	else
		return ANSI_COLOR_RED "error: " ANSI_COLOR_RESET +msg;
}
/**************************************************
 * ************************************************/

bool isCamCodyDecod()
{
    Caminante a,b;
    a.ini(2, 8);
    string aS = a.codificar();
    int aux=0;
    b.desCodificar(aS, aux, 8);
    return aS == b.codificar();
}


void sacarPantIsCamCyD(){
    cout<<"------------"<<endl;
    cout<<"Prueba codifificar y decodificar Caminante:"<<endl;
    if(isCamCodyDecod()){
        cout<<error(E_NONE)<<endl;
    }
    else{
        cout<<error(ERROR,"Fallo en codificar y descodificar Caminante")<<endl;
    }
}
/**************************************************
 * ************************************************/

bool isPobCodyDecod(){
    Poblacion a(10, 4, 12, "./../entradas/uk12.txt");
  
    
    string aS = a.codificar(ALL_POB);
    

    Poblacion b;
    b.descodificar(aS);


    string bS = b.codificar(ALL_POB);

    return aS == bS;
}

void sacarPantIsPobCyD(){
    cout<<"------------"<<endl;
    cout<<"Prueba codifificar y decodificar Poblacion:"<<endl;
    bool res = false;
    try
    {

        res = isPobCodyDecod();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    if(res){
        cout<<error(E_NONE)<<endl;
    }
    else{
        cout<<error(ERROR,"Fallo en codificar y descodificar Poblacion")<<endl;
    }
}


/**************************************************
 * ************************************************/
bool isPobConstStr()
{
    Poblacion a(10, 4, 12, "./../entradas/uk12.txt");

    string aS = a.codificar(ALL_POB);

    Poblacion b(aS);

    string bS = b.codificar(ALL_POB);
    return aS == bS;
}

void sacarPantIsPCS()
{
    cout << "------------" << endl;
    cout << "Prueba codifificar y constructor Poblacion con string:" << endl;

    bool res= false;
    try
    {
        res = isPobConstStr();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    if (res)
    {
        cout << error(E_NONE) << endl;
    }
    else
    {
        cout << error(ERROR, "Fallo en codificar y descodificar Poblacion") << endl;
    }
}

void isFitCorr(){
    Poblacion a(10, 4, 12, "./../entradas/uk12.txt");
    float bestFit, media;
    a.stats(a, 0, bestFit, media);
    cout<<a.codificar()<<endl;
    cout<<bestFit<<endl;
    cout<<media<<endl;
}

bool isDivYFus(){
    Poblacion a(100, 4, 12, "./../entradas/uk12.txt");
    Poblacion Pobs[1];
    a.dividir(1,Pobs);
  
    string a1 = a.codificar();
    a.fusionar(1,Pobs);
    string a2 = a.codificar();
    return a1 == a2;
}

void sacarPantIsDivYFus(){
    cout<<"------------"<<endl;
    cout<<"Prueba dividir y fusionar Poblacion:"<<endl;
    if(isDivYFus()){
        cout<<error(E_NONE)<<endl;
    }
    else{
        cout<<error(ERROR,"Fallo en dividir y fusionar Poblacion")<<endl;
    }
}


/******************************************
 * Ahora comprobaremos caminante.ini()
 * *********************************************/
 bool isDigit(char c,string flags =""){
     bool res = true;
     if (c<'0') res =false;
     if (c> '9') res = false;
     //if(!res&&flags=="") cout<<"deberia ser digito pero es: "<<int(c)<< " "<< c<<endl;
     return res;
 }

 bool fitPosCorrect(string txt){

     int frs = txt.find_first_of(":");
     int lst = txt.find_first_of(";");
     if(!(stoi(&txt[++frs])==-1)){

    
        for(int i= 1+frs;i<lst;i++)
            if(!isDigit(txt[i])||!(txt[i]=='.') ){
            return false;
            }
        
        }

    return true;
}

 bool cityPosCorrect(string txt, int numCity){

     bool cities[numCity];
     for(int i=0;i<numCity;i++){
         cities[i]=false;
     }
     int coma=0;
     int c = stoi(&txt[0]);
      while(txt[++coma]!=',');
     int end =txt.find_first_of(":");
     for(int i =0;i<numCity;i++){
         int num=stoi(&txt[++coma]);
         if(!cities[num]){
             cities[num]= true;
         }
         else{
             cout<<error(ERROR,"se repiten ciudades en los caminantes")<< i<<endl;
             return false;
         }
         if(num == c){
            while(isDigit(txt[++coma],"h"))
            if(coma != end){
                 cout<<error(ERROR,"numero de ciudades incorrecto")<<endl;
                 return false;
            }
         }
         while(txt[++coma]!=',');
         

     }
    return true;

 }
 bool caminanteCorrecto(string txt,int numCity){

     if(!fitPosCorrect(txt)){
          
          cout<<error(ERROR, "fit no esta en la posición correcta")<<endl;
          return false;
     }
     else if(!cityPosCorrect(txt,numCity)){
          return false;

     }


    return true;
 }
 
 bool caminanteIni(){
     cout<<"------------"<<endl;
     cout<<"comprobando caminante.ini():"<<endl;
     int frs=0;
     int numCt= 12;
     string txt;
     Caminante cam;
     cam.ini(frs,numCt);
     txt = cam.codificar();
     if(caminanteCorrecto(txt,numCt)){
        cout<< error(E_NONE)<<endl;
         return true;

     }
     else{
         cout<<error(ERROR,"función incorrecta")<<endl;
         cout<<"Caminante: "<<txt<<endl;

         return false;
     }

     
     

 }

/******************************************
 * Ahora comprobaremos el cosntructor de la población
 * *********************************************/



bool comprobarMatr(string txt,int numCity,int first, int& idx)
{
    idx = 0;
    if(numCity!=stoi(&txt[idx])){
        cout<<error(ERROR,"Numero de ciudades erroeneo")<<endl;
    }
    while(txt[idx]!=':') idx++;
    idx++;
    ifstream f1;
    string entrada = "./../entradas/uk12.txt";
    bool iguales = false;
    int aux=0;
    f1.open(entrada);
    if (f1.is_open())
    {
        char c[500];
    
        //quitarse los comentarios
        do{
           f1.getline(c, 500, '\n');
        }while (c[0] == '#');

    
       
        int num;
        int miNum;
        int ind;
        iguales = true;
        bool cambioFila = false;
        int contMat = 0;
        int contFil = 1;
        int contCol = 0;
        do
        {   
            ind = 0;
            while(c[ind]!= '\0' && iguales){
                if (!cambioFila && contCol == 0) {cambioFila = true;}
                if(!cambioFila){
                    if(contCol == -1) contCol = 0;
                    while (!isDigit(txt[idx])) idx++;
                    miNum = stoi(&txt[idx]);
                    while (isDigit(txt[idx])) idx++;
                    if(txt[idx] == ',') contCol++;
                    if(txt[idx] == ';') {
                        contFil++; 
                        contCol=0;
                    } 
                }
                if(!cambioFila){
                    while (!isDigit(c[ind])) ind++;
                    num = stoi(&c[ind]);
                    contMat++;
                    while (isDigit(c[ind])) ind++;
                    if(miNum != num) iguales = false;
                    else aux++;
                }else{
                    while (!isDigit(c[ind])) ind++;
                    num = atoi(&c[idx]);
                    contMat++;
                    while (isDigit(c[ind])) ind++;
                }
                if(contMat == numCity){ 
                    contCol = -1; 
                    contMat = 0;
                    cambioFila = false;
                }
                
                
            }
            
        }while (f1.getline(c, 500, '\n') && iguales);
        f1.close();
    }
    else
    {
        cerr << "Fichero no encontrado" << endl;
        
    }
    idx+=2;
    return iguales;
}

bool comprobarCaminantes(Poblacion pob,int numCam,int numCit){

    int idx=0;
   for (int i = 0; i < numCam; i++)
   {
      if( !caminanteCorrecto( pob.getCaminante(i).codificar(),numCit )) return false;
    }
   
    return true;

} 

 bool contructorPobParam(){

    cout<<"------------"<<endl;
    cout<<"comprobando Poblacion(int numCam, int ciudIni, int numCiuds, string entrada):"<<endl;
    int idx = 0;
    int numCam =1;
    int ciudadIni =0;
    int numCiuds=12;
    string entrada = "./../entradas/uk12.txt";
    Poblacion pob(numCam,ciudadIni,numCiuds,entrada);
    string txt = pob.codificar();
    bool res =false;

   

        try
        {
            res = comprobarMatr(txt, numCiuds, ciudadIni, idx);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            cout << error(ERROR, "Matriz de población incorrecta") << endl;
        }

        try
        {
            res = comprobarCaminantes(pob, numCam, numCiuds);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            cout << error(ERROR, "Matriz de población incorrecta") << endl;
        }

    if (res){
        cout<<error(E_NONE)<<endl;
    }else{
        cout << error(ERROR,"constructor con varios parametros erróneo") << endl;
    }
    return res;


 }

 /******************************************
 * Ahora la función mutar
 * *********************************************/
bool trymutar(){
    int idx =0;
    int numCam = 100;
    int ciudadIni = 0;
    int numCiuds = 12;
    string entrada = "./../entradas/uk12.txt";
    Poblacion pob(numCam, ciudadIni, numCiuds, entrada);
    for(int i =0; i<numCam;i++){
        pob.mutar(i);
    }
    string txt = pob.codificar();
    bool res = false;

    try
    {
        res = comprobarMatr(txt, numCiuds, ciudadIni, idx);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        cout << error(ERROR, "Matriz de población incorrecta") << endl;
    }

    try
    {
        res = comprobarCaminantes(pob, numCam, numCiuds);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        cout << error(ERROR, "Caminantes") << endl;
    }

    if (!res)
    {
      
    
        cout << error(ERROR, "Población errónea") << endl;
    }
    return res;
}


void sacarPantMutar(){

    cout << "------------" << endl;
    cout << "comprobando mutar: " << endl;
    bool res= false;
    try
    {
        res = trymutar();
    }
    catch(const std::exception& e)
    {
        
        std::cerr << e.what() << '\n';
    }
    if(res){
        cout << error(E_NONE) << endl;
    }
    else
    {
        cout << error(ERROR, "función mutar incorrecta")<<endl;
    }
    
    
}

/******************************************
 * Ahora la función cruzar
 * *********************************************/
bool trycruzar()
{
    int idx = 0;
    int numCam = 100;
    int ciudadIni = 0;
    int numCiuds = 12;
    string entrada = "./../entradas/uk12.txt";
    Poblacion pob(numCam, ciudadIni, numCiuds, entrada);
    for (int i = 0; i < numCam; i++)
    {
        pob.mutar(i);
    }
    string txt = pob.codificar();
    bool res = false;

    try
    {
        res = comprobarMatr(txt, numCiuds, ciudadIni, idx);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        cout << error(ERROR, "Matriz de población incorrecta") << endl;
    }

    try
    {
        res = comprobarCaminantes(pob, numCam, numCiuds);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        cout << error(ERROR, "Caminantes") << endl;
    }

    if (!res)
    {

        cout << error(ERROR, "Población errónea") << endl;
    }
    return res;
}

void sacarPantCruzar()
{

    cout << "------------" << endl;
    cout << "comprobando cruzar: " << endl;
    bool res = false;
    try
    {
        res = trycruzar();
    }
    catch (const std::exception &e)
    {

        std::cerr << e.what() << '\n';
    }
    if (res)
    {
        cout << error(E_NONE) << endl;
    }
    else
    {
        cout << error(ERROR, "función cruzar incorrecta") << endl;
    }
}

/******************************************
 * Ahora la función selecion
 * *********************************************/
bool tryselec()
{
    int idx = 0;
    int numCam = 100;
    int ciudadIni = 0;
    int numCiuds = 12;
    string entrada = "./../entradas/uk12.txt";
    Poblacion pob(numCam, ciudadIni, numCiuds, entrada);
    for (int i = 0; i < numCam; i++)
    {
        pob.mutar(i);
    }
    string txt = pob.codificar();
    bool res = false;

    try
    {
        res = comprobarMatr(txt, numCiuds, ciudadIni, idx);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        cout << error(ERROR, "Matriz de población incorrecta") << endl;
    }

    try
    {
        res = comprobarCaminantes(pob, numCam, numCiuds);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        cout << error(ERROR, "Caminantes") << endl;
    }

    if (!res)
    {

        cout << error(ERROR, "Población errónea") << endl;
    }
    return res;
}

void sacarPantSeleccion()
{

    cout << "------------" << endl;
    cout << "comprobando selección: " << endl;
    bool res = false;
    try
    {
        res = tryselec();
    }
    catch (const std::exception &e)
    {

        std::cerr << e.what() << '\n';
    }
    if (res)
    {
        cout << error(E_NONE) << endl;
    }
    else
    {
        cout << error(ERROR, "función seleccion incorrecta") << endl;
    }
}


/******************************************
 * Ahora la función mutar (caminante)
 * *********************************************/

bool camMut(){

    int frs = 0;
    int numCt = 12;
    string txt;
    Caminante cam;
    cam.ini(frs, numCt);
    
    bool res = false;

    try
    {
        cam.mutar();
        txt = cam.codificar();
        res = caminanteCorrecto(txt, numCt);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
  
    return res;
    
}


void sacarPantCamMut()
{
    cout << "------------" << endl;
    cout << "Prueba mutar Caminante:" << endl;
    bool res = false;
    try
    {
        res =camMut();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    if (res)
    {
        cout << error(E_NONE) << endl;
    }
    else
    {
        cout << error(ERROR, "Fallo en mutar Caminante") << endl;
    }
}

/******************************************
 * Ahora la función cruzar (caminante)
 * *********************************************/

bool camCruz()
{

    int frs = 0;
    int numCt = 12;
    string txt;
    Caminante cam1,cam2,cam3;
    cam1.ini(frs, numCt);
    cam2.ini(frs, numCt);
    
    bool res = false;

    try
    {
      
        cam3.cruzar(cam1, cam2, numCt);
        
        txt = cam3.codificar();
        

        res = caminanteCorrecto(txt, numCt);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return res;
}

void sacarPantCamCruz()
{
    cout << "------------" << endl;
    cout << "Prueba cruzar Caminante:" << endl;
    bool res = false;
    try
    {
        res = camCruz();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    if (res)
    {
        cout << error(E_NONE) << endl;
    }
    else
    {
        cout << error(ERROR, "Fallo en cruzar Caminante") << endl;
    }
}
int main()
{

    caminanteIni();
    sacarPantIsCamCyD();
    sacarPantIsPobCyD();
    sacarPantIsPCS();
    contructorPobParam();
    sacarPantIsDivYFus();
    sacarPantCamMut();
    sacarPantCamCruz();
    sacarPantMutar();
    sacarPantCruzar();
    sacarPantSeleccion();

    return 0;
 }