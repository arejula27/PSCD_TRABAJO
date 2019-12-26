#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    ifstream f1;
    f1.open("usca312_dist.txt");
    char h[1000000];
    int ciudades[312][312];
    for(int i=0;i<7;i++){
        f1.getline(h, 1000000, '\n');
    }
    int i=0;
    int contFil = 0;
    int contCol = 0;
    int aux, num;
    char aux1;
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
            ciudades[contFil][contCol] = num;
            contCol++;
            if(contCol == 312){
                contCol = 0;
                contFil++;
            }
        } 
    }
    for(int i = 0; i<312;i++){
        for(int j = 0;j<312;j++){
            cout<<ciudades[i][j]<<" ";
        }
        cout<<"CAMBIO DE FILA\n";
    }
    
    f1.close();
    return 0;
}