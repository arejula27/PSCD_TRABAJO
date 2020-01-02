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

 bool isCodyDecod(){
    Caminante a,b;
    a.ini(2, 8);
    int aux=0;
    string aS = a.codificar();
    b.desCodificar(aS, aux);
    return aS == b.codificar();
}

void sacarPantIsCyD(){
    cout<<"Prueba codifificar y decodificar:"<<endl;
    if(isCodyDecod()){
        cout<<E_NONE;
    }
    else{
        cout<<ERROR<<"Fallo en codificar y descodificar"<<endl;
    }
}

 bool isDigit(char c){
     bool res = true;
     if (c<'0') res =false;
     if (c> '9') res = false;
     return res;
 }

 bool fitPosCorrect(string txt){

     int frs = txt.find_first_of(":");
     int lst = txt.find_first_of(";");

    for(int i= frs;i<lst;i++)
        if(!isDigit(txt[i]))return false;

    return true;
}

 bool cityPosCorrect(string txt, int numCity){

     bool cities[numCity];
     for(int i=0;i<numCity;i++){
         cities[i]=false;
     }
     int coma=0;
     int c = stoi(&txt[0]);
     coma = txt.find_first_of(",");
     int end =txt.find_first_of(":");
     for(int i =0;i<=numCity;i++){
         int num=stoi(&txt[++coma]);
         if(!cities[num]){
             cities[num ]= true;
         }
         else{
             error(ERROR,"se repiten ciudades en los caminantes");
             return false;
         }
         if(num == c){
            if(coma != end){
                 error(ERROR,"numero de ciudades incorrecto");
                 return false;
            }
         }
         while(txt[++coma]!=',');
         

     }
    return true;

 }
 bool caminanteCorrecto(string txt,int numCity){

     if(!fitPosCorrect(txt)){
          error(ERROR, "fit no esta en la posición correcta");
          return false;
     }
     else if(!cityPosCorrect(txt,numCity)){
          error(ERROR, "camino  no esta es correcto");

     }


    return true;
 }
 
 bool caminanteIni(){
     cout<<"------------"<<endl;
     cout<<"comprobando caminante.ini(): ";
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
         return false;
     }

     
     

 }
 
 
 
 
 int main(){
     caminanteIni();
     sacarPantIsCyD();

     return 0;
 }