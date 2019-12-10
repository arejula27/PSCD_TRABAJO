#include <string>
using namespace std;
const int  CITY_MAX =500;
const int CAM_MAX = 100 ;
const int UPGRADE_POB =0;
const int ALL_POB =1;

class Caminante
{
private:
    
    friend void Poblacion::calcFit(Caminante &caminante);
    int camino[CITY_MAX];
    int fitness;
public:

    Caminante();
    void ini(string MiCamino);
    ~Caminante();
    int MyFit();
    void Mutar();
    void Cruzar(const Caminante &O1, const Caminante &O2);
    string serializar();
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

//Guarda el camino del caminante según la cadena <MiCamino>, que tendrá de formato:
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN;"
void Caminante::ini(string MiCamino)
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

//Devuelve en una cadena el camino seguido por el Caminante según el formato: 
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN;"
string Caminante::serializar()
{
    string aux = "";
    string ciudad;
    for (int i=0; i<CITY_MAX; i++){
        ciudad = to_string(camino[i]);
        aux += ciudad +',';
    }
    return aux;
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
        //constructo predeterminado
        Poblacion();
        //constructo para inicializar una poblacion a partir de un archivo de texto
        //con una matriz, se le debe de undicar el numero de caminantes de la pobaclion 
        //con maximo CAM_MAX
        Poblacion(int caminantes, string entrada = "entrada.txt");
        //Tras recibir una poblacion de un cliente/servidor, usar este constructor para inicilizar
        Poblacion(string data);
        ~Poblacion();
        //recibe los caminantes de un cliente/servidor, esta funcion sustituye los actuales por 
        //los nuevos
        void actualizar();
        //recibe la poblacion de un cliente/servidor, copia todos los datos en tu poblacion,
        //utilizar para inicializarla copiando otra
        void ini();
        //calcula el fin de un caminante y cambia su propio fit 
        void calcFit(Caminante &caminante);
        //divide la poblacion en n subpoblaciones
        void dividir(int n, Poblacion pobs[]);
        //obtiene el mejor fit de la poblacion
        int mejorFit();
        //obtiene el fites medio de la poblacion
        int mediaFit();
        //envia la poblacion, usar ALL_POB para codificar la poblacion con todos sus datos o
        //UPGRADE_POB para codificar ÚNICAMENTE los caminates 
        void codificar(int flg = ALL_POB);

        void descodificar(int flg = ALL_POB);
};
//le indicas cuantos caminantes va a haber y la entrada donde estan los datos
Poblacion::Poblacion(int numCam, string entrada = "entrada.txt")
{
    //inicializar numCam
    //rellenar la matriz
    //inicializar numCities
    for (int i = 0; i < numCam; i++)
    {
        caminantes[i].ini("numCities");
        #warning PARAMETRO INCORRECTO "caminante.ini(string)"  //Coflicto con mi función.
        
    
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
void Poblacion::calcFit(Caminante &caminate)
{

}


    //divide la poblacion en n subpoblaciones y las devuelve en array
    void
    Poblacion::dividir(int n, Poblacion pobs[])
{
    int sobr= numCam%n;
    int indx=0;
    
    for (int  i = 0; i < n; i++)
    {
        pobs[i].numCities=numCities;

       
        #warning copiar matriz  // ¿memcopy con el el tamaño?
        int numSub = (numCam / n) - 1;
        if(sobr>0){
           numSub++;
            sobr--;
        }
        pobs[i].numCam =numSub;
        //¿optimizar el bucle con memcopy?
        for (int j = 0; j < numSub; j++)
        {
            pobs[i].caminantes[j] = caminantes[j+indx];
        }
        indx += numSub;
    }
    


}

//obtiene el mejor fitnes de la poblacion
int Poblacion::mejorFit()
{

}

//obtiene la media de todos los fits de la poblacion
int Poblacion::mediaFit()
{

}

//transforma los datos de la poblacion en un único string, por defecto toda
//con UPGRADE_POB, unicamente el vector de caminantes
//ALL_POB=>
//UPGRADE_POB=> "numCam:[caminante]*"
void Poblacion::codificar(int flg = ALL_POB)
{
    string msg;
    switch (flg)
    {
        case ALL_POB:
            msg = to_string(numCities) + ":";
            #warning serializar matriz
            msg += to_string(numCam) + ":";
            for (int i = 0; i < numCam; i++)
            {
                msg += caminantes[i].codificar();
            }
            

            break;
        case UPGRADE_POB:
        msg=to_string(numCam)+":";
            for (int i = 0; i < numCam ; i++)
            {
                msg+=caminantes[i].codificar()+";";
            }
            
            break;

        default:
            break;
    } 
}

void descodificar(string mag, int flg = ALL_POB)
{
    switch (flg)
    {
    case ALL_POB:
        to_string(numCities) + ":";
        numCities = strtok() 
        #warning serializar matriz
        msg += to_string(numCam) + ":";
        for (int i = 0; i < numCam; i++)
        {
            msg += caminantes[i].codificar();
        }

        break;
    case UPGRADE_POB:
        msg = to_string(numCam) + ":";
        for (int i = 0; i < numCam; i++)
        {
            msg += caminantes[i].codificar() + ";";
        }

        break;

    default:
        break;
    }
}
