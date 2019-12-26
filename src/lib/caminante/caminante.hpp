#include <string>
#include <fstream>
using namespace std;
const int  CITY_MAX =500;
const int CAM_MAX = 100 ;
const int UPGRADE_POB =0;
const int ALL_POB =1;



class Caminante
{
    //friend void Poblacion::calcFit();

private:
    int camino[CITY_MAX];
    float fitness;
public:

    Caminante();
    ~Caminante();

    //Inicializa el caminante de forma aleatoria partiendo de <inicio> con <max> número de ciudades.
    void ini(int inicio, int max);

    //Guarda en caminante el camino según la cadena <MiCamino>, que tendrá de formato:
    // "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
    //Y actualiza <avance> con el número de letras entre "NumCiud1" y "fitness;", todo incluido.
    void desCodificar(const string MiCamino, int &avance);

    //Devuelve el camino del caminante según la cadena <MiCamino>, que tendrá de formato:
    // "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
    string codificar();

    void calcMiFit(int dist[CITY_MAX][CITY_MAX], int numCiuds);

    //Devuelve el fitness del caminante.
    float MyFit();

    //Función de mutar.
    void Mutar();

    //Modifica el camino del caminante con los genes cruzados de sus padres.
    void Cruzar(const Caminante &O1, const Caminante &O2);
};




class Poblacion
{
    friend Caminante;
  
private:

        int dist[CITY_MAX][CITY_MAX];
        int numCam;
        Caminante caminantes[CAM_MAX];
        string cities[CITY_MAX];
        int numCities;

        

    public:
        //constructo predeterminado
        Poblacion();
        //constructo para inicializar una poblacion a partir de un archivo de texto
        //con una matriz, se le debe de indicar el numero de caminantes de la pobaclion 
        //con maximo CAM_MAX
        Poblacion(int numCam,int ciudIni, int numCiuds, string entrada = "entrada.txt");

        
        //Tras recibir una poblacion de un cliente/servidor, usar este constructor para inicilizar
        Poblacion(string data);

        ~Poblacion();

        //calcula el fit de un caminante y cambia su propio fit 
        void calcFit(Caminante &caminante);
        //Devuelve el porcentaje de caminantes que son mejores que el fit que le introducimos,
        //tambien por mejorFit devuelve el fitness del mejor caminante y por media la 
        //media de fitness de los caminates
        float stats(Poblacion &subPob, float fit, float &mejorFit, float &media);
        //divide la poblacion en n subpoblaciones
        void dividir(int n, Poblacion pobs[]);

        //fuisona la poblacion en n subpoblaciones
        void fusionar(int n, Poblacion pobs[]);
        //Devuelve un string que almacena la matriz de distancias de la Poblacion según el siguiente formato:
        // "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;)"
        string codificarMatriz();

        //Guarda en dist la matriz de distancias de la Población almacenada en <MiMatriz> según el siguiente formato:
        // "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;)"
        //Y aumenta <avance> con el número de letras entre "(" y ")", ambos incluidos.
        void descodificarMatriz(const string MiMatriz, int &avance);

        //envia la poblacion, usar ALL_POB para codificar la poblacion con todos sus datos o
        //UPGRADE_POB para codificar ÚNICAMENTE los caminates 
        string codificar(int flg = ALL_POB);

        void descodificar(string mag, int flg = ALL_POB);

        //toma la matriz de otra poblacion
        void getMatrixFrom(Poblacion pob);
};