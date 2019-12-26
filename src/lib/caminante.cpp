#include "caminante.hpp"

//Constructor del caminante
Caminante::Caminante()
{
    camino[0] = -1;
    fitness = -1;
}

Caminante::~Caminante()
{
}

//Guarda en caminante el camino según la cadena <MiCamino>, que tendrá de formato:
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
//Y actualiza <avance> con el número de letras entre "NumCiud1" y "fitness;", todo incluido.
void Caminante::desCodificar(const string MiCamino, int &avance)
{
    avance = 0;
    int i = 0;
    while (MiCamino[avance] != ':')
    {
        camino[i] = 0;
        camino[i] = stoi(&MiCamino[avance]);
        while (MiCamino[avance] != ',' || MiCamino[avance] != ':')
        {
            avance++;
        }
        avance++;
        i++;
    }
    camino[i] = camino[0];
    avance++;
    stof(&MiCamino[avance]);
    while (MiCamino[avance] != ';')
    {
        avance++;
    }
    avance++;
}

//Devuelve el camino del caminante según la cadena <MiCamino>, que tendrá de formato:
// "NumCiud1,NumCiud2,NumCiud3, ...., NumCiudN:fitness;"
string Caminante::codificar()
{
    int inicio = camino[0];
    string MiCamino = to_string(inicio) + ',';
    int i = 0;

    while (camino[++i] != inicio)
    {
        MiCamino += to_string(camino[i]) + ',';
    }
    MiCamino += to_string(fitness);
    MiCamino += to_string(inicio) + ';';
    return MiCamino;
}

//Inicializa el caminante de forma aleatoria partiendo de <inicio> con <max> número de ciudades.
void Caminante::ini(int inicio, int max)
{
    srand(time(0));
    int aux;
    bool recorridos[max];
    for (int i = 0; i < max; i++)
    {
        recorridos[i] = false;
    }
    recorridos[inicio] = true;
    camino[0] = inicio;
    for (int i = 1; i < max; i++)
    {
        aux = rand() % max;
        while (recorridos[aux])
        {
            aux = (aux + 1) % max;
        }
        recorridos[aux] = true;
        camino[i] = aux;
    }
    camino[max] = inicio;
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





/**********************************************
 * FUNCIONES POBLACIÓN
 * ********************************************/

//le indicas cuantos caminantes va a haber y la entrada donde estan los datos
Poblacion::Poblacion(int numCam, int ciudIni, int numCiuds, string entrada)
{
    //inicializar numCam
    //rellenar la matriz
    //inicializar numCities
    for (int i = 0; i < numCam; i++)
    {
        caminantes[i].ini(ciudIni, numCiuds);
    }
    ifstream f1;
    f1.open(entrada);
    char h[1000000];
    for(int i=0;i<7;i++){
        f1.getline(h, 1000000, '\n');
    }
    int i=0;
    int contFil = 0;
    int contCol = 0;
    int num;
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
            dist[contFil][contCol] = num;
            contCol++;
            if(contCol == numCiuds){
                contCol = 0;
                contFil++;
            }
        } 
    }
    f1.close();
}
Poblacion::Poblacion(string data)
{
}

Poblacion::~Poblacion()
{
}

//calculas el fit de un caminante y se lo guardas
void Poblacion::calcFit(Caminante &caminate)
{
}

//divide la poblacion en n subpoblaciones y las devuelve en array
void Poblacion::dividir(int n, Poblacion pobs[])
{
    int sobr = numCam % n;
    int indx = 0;

    for (int i = 0; i < n; i++)
    {
        pobs[i].numCities = numCities;

#warning copiar matriz  // ¿memcopy con el el tamaño?
        int numSub = (numCam / n) - 1;
        if (sobr > 0)
        {
            numSub++;
            sobr--;
        }
        pobs[i].numCam = numSub;
        //¿optimizar el bucle con memcopy?
        for (int j = 0; j < numSub; j++)
        {
            pobs[i].caminantes[j] = caminantes[j + indx];
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

//Devuelve un string que almacena la matriz de distancias de la Poblacion según el siguiente formato:
// "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;) "
string Poblacion::codificarMatriz()
{
    string MiMat = "(";
    for (int i = 0; i < numCities; i++)
    {
        for (int j = 0; j < numCities - 1; j++)
        {
            MiMat += dist[i][j] + ',';
        }
        MiMat += dist[i][numCities - 1] + ';';
    }
    MiMat += ")";
    return MiMat;
}

//Guarda en dist la matriz de distancias de la Población almacenada en <MiMatriz> según el siguiente formato:
// "(dist11,dist12, ... , dist1n;dist21,dist22, ... , dist2n; ... ;distn1,distn2, ..., distnn;)"
//Y aumenta <avance> con el número de letras entre "(" y ")", ambos incluidos.
void Poblacion::descodificarMatriz(const string MiMatriz, int &avance)
{
    int fil = 0;
    int col = 0;
    int aux = 0;
    avance++;
    while (MiMatriz[avance] != ')')
    {
        while (MiMatriz[avance] != ';')
        {
            aux = stoi(&MiMatriz[avance]);
            while (MiMatriz[avance] != ',' || MiMatriz[avance] != ';')
            {
                avance++;
            }
            avance++;
            dist[fil][col] = aux;
            col++;
            aux = 0;
        }
        fil++;
        avance++;
    }
    avance++;
}

//transforma los datos de la poblacion en un único string, por defecto toda
//con UPGRADE_POB, unicamente el vector de caminantes
//ALL_POB=>
//UPGRADE_POB=> "numCam:[caminante]*"
string Poblacion::codificar(int flg)
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
        msg = to_string(numCam) + ":";
        for (int i = 0; i < numCam; i++)
        {
            msg += caminantes[i].codificar() + ";";
        }

        break;
    }
}

void Poblacion::descodificar(string msg, int flg)
{

    int inx = 0;
    if (flg == ALL_POB)
    {

        numCities = stoi(msg);
        while (msg[inx++] != ':')
            ;
        descodificarMatriz(msg, inx);
        numCam = stoi(&msg[inx]);
        while (msg[inx++] != ':')
            ;
        //descodificar todos los viajeros
        for (int i = 0; i < numCam; i++)
        {
            int avz = 0;
            caminantes[i].desCodificar(&msg[inx], avz);
            inx += avz;
        }
    }

    else if (flg == UPGRADE_POB)
    {

        numCam = stoi(msg);
        while (msg[inx++] != ':')
            ;
        //descodificar todos los viajeros
        for (int i = 0; i < numCam; i++)
        {
            int avz = 0;
            caminantes[i].desCodificar(&msg[inx], avz);
            inx += avz;
        }
    }
}
