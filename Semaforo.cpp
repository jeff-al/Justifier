#include "Semaforo.h"

Semaforo::Semaforo(int valorInicial, int idf) {
    if(idf == 0){
    id = semget(KEY, 1, IPC_CREAT | 0600);
    }else{
    id = semget(KEY2, 1, IPC_CREAT | 0600);
    }
    if (-1 == id) {
        perror("Error constructor");
        exit(1);
    }
}

 Semaforo::~Semaforo() {
    int x = semctl(id, 0, IPC_RMID);
    if (-1 == x) {
        perror("Error en el destructor sem");
    }
}

int Semaforo::Signal() {
    struct sembuf n;
    n.sem_num = 0;
    n.sem_op = 1; //Suma al semaforo
    n.sem_flg = 0;

    int x;
    x = semop(id, &n, 1);

    if (-1 == x) {
        perror("Error en el signal");
        exit(1);
    }
}

int Semaforo::Wait() {
    struct sembuf n;
    n.sem_num = 0;
    n.sem_op = -1;  //Resta al semaforo
    n.sem_flg = 0;

    int x;
    x = semop(id, &n, 1);

    if (-1 == x) {
        perror("Error en el signal");
        exit(1);
    }
}

