#ifndef SEMAFORO_H
#define SEMAFORO_H
#include <iostream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define KEY 0xB60380
#define KEY2 0xB60381

using namespace std;

class Semaforo {
public:
    Semaforo(int valorInicial = 0, int idf = 0); //Recibe valor inicial e indentificador (0 padre y 1 hijo)
     ~Semaforo();
    
    int Signal();         //Pone señal
    int Wait();           //Quita señal
    
private:   
    int id; //ID del semaforo
    

};

#endif /* SEMAFORO_H */

