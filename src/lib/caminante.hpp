#include <string>
using namespace std;
const int  CITY_MAX =500;
const int CAM_MAX = 100 ;

class Caminante
{
private:
    
    friend void Poblacion::calcFit();
    int camino[CITY_MAX];
    int fitness;
public:

    Caminante();
    ~Caminante();
    void ini(int inicio, int max);
    void desCodificar(const string MiCamino);
    string codificar();
    int MyFit();
    void Mutar();
    void Cruzar(const Caminante &O1, const Caminante &O2);
};

//Constructor del caminante
Caminante::Caminante()
{   
    camino[0]=-1;
    fitness = -1;
}

Caminante::~Caminante()
{
}

//Guarda en caminante el camino según la cadena <MiCamino>, que tendrá de formato:
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN;"
void Caminante::desCodificar(const string MiCamino)
{   
    int i=0;
    int j=0;
    char aux;
    while(MiCamino[i]!='\n'){
        camino[j]=0;
        while(MiCamino[i]!=',' && MiCamino[i]!='\n'){
            camino[j]=camino[j]*10+atoi((const char *)MiCamino[i]);
            j++;
            i++;
        }
        i++;
    }
}

//Devuelve el camino del caminante según la cadena <MiCamino>, que tendrá de formato:
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN;"
string Caminante::codificar()
{
    int inicio = camino[0];
    string MiCamino = to_string(inicio) + ',';
    int i = 1;
    while(camino[i]!=inicio){
        MiCamino += to_string(camino[i]) + ',';
    }
    MiCamino += to_string(inicio) + ';';
    return MiCamino;
}

//Inicializa el caminante de forma aleatoria partiendo de <inicio> con <max> número de ciudades.
void Caminante::ini(int inicio, int max)
{
    int aux;
    bool recorridos[max];
    for (int i=0; i<max; i++){
        recorridos[i]=false;
    }
    recorridos[inicio] = true;
    camino[0] = inicio;
    for (int i=1; i<max-1; i++){
        aux = rand()%max;
        while(recorridos[aux]){
            aux = (aux+1)%max;
        }
        recorridos[aux] = true;
        camino[i] = aux;
    }
    camino[max-1] = inicio;
}

//Devuelve el fitness del caminante.
int Caminante::MyFit()
{    
    return fitness;
}

//Función de mutar.
void Caminante::Mutar()
{    
}

//Modifica el camino del caminante con los genes cruzados de sus padres.
void Caminante::Cruzar(const Caminante &O1, const Caminante &O2)
{
}





class Poblacion
{
private:

        int dist[CITY_MAX][CITY_MAX];
        int numCam;
        Caminante caminantes[CAM_MAX];
        string cities[CITY_MAX];
        int numCities;
        

    public:
        Poblacion(int caminantes, string entrada = "entrada.txt");
        Poblacion(string data);
        ~Poblacion();
        void actualizar();
        void calcFit();
        void dividir(int n, Poblacion pobs[]);
        int mejorFit();
        int mediaFit();
        void serializar();
};
//le indicas cuantos caminantes va a haber y la entrada donde estan los datos
Poblacion::Poblacion(int numCam, string entrada = "entrada.txt")
{
    //inicializar numCam
    //rellenar la matriz
    //inicializar numCities
    for (int i = 0; i < numCam; i++)
    {
        caminantes[i].ini(/*numCities*/);  //Coflicto con mi función.
    }
    
}


Poblacion::Poblacion(string data)
{
}

Poblacion::~Poblacion()
{
}

//actualiza los caminnates de una poblacion a partir de un string, util para 
//el servidor
void actualizar(){


}

//calculas el fit de un caminante y se lo guardas
void Poblacion::calcFit()
{

}
//divide la poblacion en n subpoblaciones y las devuelve en array
void dividir(int n, Poblacion pobs[])
{

}

//obtiene el mejor fitnes de la poblacion
int mejorFit()
{

}

//obtiene la media de todos los fits de la poblacion
int Poblacion::mediaFit()
{
}

//transforma los datos de la poblacion en un único 
void serializar()
{

}


