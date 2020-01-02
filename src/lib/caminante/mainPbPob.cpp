#include <iostream>
#include "caminante.hpp"

#define E_NONE 0;

#define ANSI_COLOR_RESET "\x1b[0m" /**< Desactiva color del terminal */
#define ANSI_COLOR_RED "\x1b[31m"  /**< Pone terminal a rojo */
#define ANSI_COLOR_GREEN "\x1b[32m" /**< Pone terminal a verde */

using namespace std;


bool isCodyDecod(){
    Caminante a,b;
    a.ini(2, 8);
    int aux=0;
    string aS = a.codificar();
    b.desCodificar(aS, aux);
    return aS == b.codificar();
}
string  strerror(int error, string msg)
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

 bool cityCorrect(){
     return true;

 }
 bool caminanteCorrecto(string txt){

     if(!fitPosCorrect(txt)){
          

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