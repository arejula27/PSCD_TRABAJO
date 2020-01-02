#include <iostream>
#include "caminante.hpp"

using namespace std;




 

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