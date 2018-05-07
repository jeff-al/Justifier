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
    Semaforo(int valorInicial = 0, int idf = 0);
     ~Semaforo();
    
    int Signal();         //Puede ser V
    int Wait();           //Puede ser P
    
private:   
    int id;
    

};

#endif /* SEMAFORO_H */

