#include <iostream>
#include "caminante.hpp"

#define E_NONE 0;
#define ERROR 1

#define ANSI_COLOR_RESET "\x1b[0m" /**< Desactiva color del terminal */
#define ANSI_COLOR_RED "\x1b[31m"  /**< Pone terminal a rojo */
#define ANSI_COLOR_GREEN "\x1b[32m" /**< Pone terminal a verde */

using namespace std;


//si error == E_NONE devuelve correcto sino imprimira como error el msg
string  strerror(int error, string msg="")
{
	switch (error)
	{
	case E_NONE:
		return ANSI_COLOR_GREEN "todo correcto" ANSI_COLOR_RESET;
	default:
		return ANSI_COLOR_RED "error: " ANSI_COLOR_RESET +msg;
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
     
     return true;

 }
 bool caminanteCorrecto(string txt){

     if(!fitPosCorrect(txt)){
          strerror(ERROR, "fit no esta en la posición correcta");
          return false;
     }
     else if(!cityPosCorrect(txt)){
          strerror(ERROR, "camino  no esta es correcto");

     }


    return true;
 }
 
 bool caminanteIni(){
     int frs=0;
     int numCt= 12;
     string txt;
     Caminante cam;
     cam.ini(frs,numCt);
     txt = cam.codificar();
     return true;
     

 }
 
 
 
 
 int main(){

     return 0;
 }