#ifndef BUZON_H
#define BUZON_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define KEY 0xB60380 // Valor de la llave del recurso

class Buzon {
public:
    Buzon();
    ~Buzon();
    int Enviar(char * mensaje, int, long);
    int Recibir(char *mensaje, int len, int&, long); // len es el tamaño máximo que soporte la variable mensaje
private:
    int id; // Identificador del buzon
    
    struct msgbuf{
        long mtype;
        int veces;
        char mtext[32];
    };
};
#endif /* BUZON_H */

