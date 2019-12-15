#ifndef MONITORIZACION
#define MONITORIZACION

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "caminante.hpp"

using namespace std;

class Monitorizacion{
    friend Caminante;
public:
    Monitorizacion();
    ~Monitorizacion();
    void calculoFit(Caminante &caminante);
    int mejorFit();
    int avgFit();

private:
    mutex mtx;
    
};
#endif