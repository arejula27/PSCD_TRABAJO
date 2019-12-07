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
    void Cruzar(const Caminante &O1, &O2);
    string serializar();
};

Caminante::Caminante(string MiCamino)
{   
    int i=0;
    int j=0;
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
    /* data */
public:
    Poblacion(/* args */);
    ~Poblacion();
};

Poblacion::Poblacion(/* args */)
{
}

Poblacion::~Poblacion()
{
}

