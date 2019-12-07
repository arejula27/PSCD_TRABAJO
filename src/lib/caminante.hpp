#include <string>
using namespace std;
const int  CITY_MAX =500;
const int CAM_MAX = 100 ;

class Caminante
{
private:
    /* data */
public:
    Caminante(/* args */);
    ~Caminante();
};

Caminante::Caminante(/* args */)
{
}

Caminante::~Caminante()
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
