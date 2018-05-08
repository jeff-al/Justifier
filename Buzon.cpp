#include "Buzon.h"
#include <iostream>

using namespace std;

Buzon::Buzon() {
    id = msgget(KEY, IPC_CREAT | 0600);
    if (-1 == id) {
        cout << "Error en el constructor" << endl;
    }

}

Buzon::~Buzon() {
    int x;
    x = msgctl(id, IPC_RMID, 0);
    if (-1 == x) {
        cout << "Error en el destructor buzon" << endl;
    }
}

int Buzon::Enviar(char* mensaje, int veces, long tipo) {
    struct msgbuf a;
    strcpy(a.mtext, mensaje);
    a.mtype = tipo;
    a.veces = veces;
    
    int x;
    x = msgsnd(id, (void*) &a, sizeof (a), IPC_NOWAIT);
    if (-1 == x) {
        cout << "Error en el enviar" << endl;
    }
}

int Buzon::Recibir(char* mensaje, int len, int &veces, long tipo) {
    struct msgbuf a;
    int x;
    x = msgrcv(id, (void*) &a, sizeof (a), tipo, 0);
    if (-1 == x) {
        cout << "Error en el recibr" << endl;
    } else {
        strncpy(mensaje, a.mtext, len);
        veces = a.veces;
    }
}

