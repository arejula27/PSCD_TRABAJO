#include <string>
using namespace std;
const int  CITY_MAX =500;
const int CAM_MAX = 100 ;

class Caminante
{
private:
    /* data */
    friend class Poblacion;
    int camino[CITY_MAX];
    int fitness;
public:
    Caminante(string MiCamino);
    ~Caminante();
    int MyFit();
    void Mutar();
    void Cruzar(const Caminante& O1, const Caminante& O2);
    string serializar();
};

Caminante::Caminante(string MiCamino)
{   
    int i=0;
    int j=0;
    char aux;
    while(MiCamino[i]!='\n'){
        camino[j]=0;
        while(MiCamino[i]!=',' && MiCamino[i]!='\n'){
            camino[j]=camino[j]*10+atoi(MiCamino[i]);
            j++;
            i++;
        }
    }
}

Caminante::~Caminante()
{
}

Caminante::MyFit()
{    
    return Caminante.fitness;
}

Caminante::Mutar()
{    
}

Caminante::Cruzar()
{
}

class Poblacion
{
private:

        int dist[CITY_MAX][CITY_MAX];
        Caminante caminantes[CAM_MAX];
        string cities[CITY_MAX];
        

    public:
        Poblacion(int caminantes, string entrada = "entrada.txt");
        Poblacion(string data);
        ~Poblacion();
        void calcFit();
        void dividir(int n, Poblacion pobs[]);
        int mejorFit();
        int mediaFit();
        void serializar();
};

Poblacion::Poblacion(int caminantes, string entrada = "entrada.txt")
{
}
Poblacion::Poblacion(string data)
{

}

Poblacion::~Poblacion()
{
}
void calcFit()
{

}
void dividir(int n, Poblacion pobs[])
{

}
int mejorFit()
{

}
int mediaFit()
{

}
void serializar()
{

}
