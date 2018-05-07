
#ifndef CONTROLADOR_H
#define CONTROLADOR_H
#include "Buzon.h"
#include "Semaforo.h"
#include "File.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/shm.h>

class Controlador {
public:
    Controlador();
    virtual ~Controlador();
    bool Synchronize(char*[], int);
    File f;
private:
    Buzon b;
    Semaforo sP = Semaforo(0,0);
    Semaforo sH = Semaforo(0,1);

    struct AreaCompartida {
        int n = 0;

        struct Etiqueta {
            char palabra[32];
            int veces;
        } Etiquetas[84];
    };
    typedef struct AreaCompartida AC;
    
    void WaitForAll(int);
    char* Convert(int);
    void Display(AC*);
    void CopyInfo(char*, int, AC*);

};

#endif /* CONTROLADOR_H */

