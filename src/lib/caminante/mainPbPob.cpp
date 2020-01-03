#include <iostream>
#include <string>
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

 bool isCamCodyDecod(){
    Caminante a,b;
    a.ini(2, 8);
    string aS = a.codificar();
    int aux=0;
    b.desCodificar(aS, aux,8);
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

bool isPobCodyDecod(){
    Poblacion a(10, 4, 12, "./../entradas/uk12.txt");
    
    string aS = a.codificar(ALL_POB);
    
    
    Poblacion b(aS);
   
    string bS = b.codificar(ALL_POB);
    return true;
}

void sacarPantIsPobCyD(){
    cout<<"------------"<<endl;
    cout<<"Prueba codifificar y decodificar Poblacion:"<<endl;
    if(isPobCodyDecod()){
        cout<<error(E_NONE)<<endl;
    }
    else{
        cout<<error(ERROR,"Fallo en codificar y descodificar Poblacion")<<endl;
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
    Poblacion Pobs[4];
    a.dividir(4,Pobs);
    string a1 = a.codificar();
    a.fusionar(4,Pobs);
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
     if(!res&&flags=="") cout<<"deberia ser digio pero es: "<<int(c)<< " "<< c<<endl;
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
 * Ahora comprobaremos el cisntructor de la población
 * *********************************************/



bool comprobarMatr(string txt,int numCity,int first){

    int idx=0;
    if(numCity!=stoi(txt)){
        cout<<error(ERROR,"Numero de ciudades erroeneo")<<endl;
    }
    while(txt[idx++]!=':');
    ifstream f1;
    string entrada = "./../entradas/uk12.txt";
    f1.open(entrada);
    if (f1.is_open())
    {
        char c[200];
    
        //quitarse los comentarios
        do{
           f1.getline(c, 200, '\n');
        }while (c[0] == '#');

    
       
        int num;
        int miNum;

        
            
        while (f1.getline(c, 200, ' '))
        {
            miNum = stoi(&txt[idx]);
            while ((txt[idx] != ';') || (txt[idx++] != ','));

            
        }
        f1.close();
    }
    else
    {
        cerr << "Fichero no encontrado" << endl;
    }

    return true;
}

bool comprobarCaminantes(string txt,int numCam,int numCit){

    int idx=0;
    while(txt[idx++]!=')');

    for (int i = 0; i < numCam; i++)
    {
        if(!caminanteCorrecto(&txt[idx],numCit)){
            cout<<"cam: "<<idx<<endl;
        }
        while(txt[idx++]!=';');
    }
    

    return true;

} 
 bool contructorPobParam(){
    cout<<"------------"<<endl;
    cout<<"comprobando Poblacion(int numCam, int ciudIni, int numCiuds, string entrada):"<<endl;
    int numCam =1;
    int ciudadIni =0;
    int numCiuds=12;
    string entrada = "./../entradas/uk12.txt";
    Poblacion pob(numCam,ciudadIni,numCiuds,entrada);
    string txt = pob.codificar();

    if(!comprobarMatr(txt,numCiuds,ciudadIni)){

        cout<<error(ERROR,"Matriz de población incorrecta")<<endl;
        cout<<"Población: "<<txt<<endl;

    }
    if(!comprobarCaminantes(txt,numCam,numCiuds)){

    }
    



    return true;


 }
 
 
 
 int main(){
    
     caminanteIni();
     sacarPantIsCamCyD();
     sacarPantIsPobCyD();
     contructorPobParam();
 

     return 0;
 }