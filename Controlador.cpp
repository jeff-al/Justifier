#include "Controlador.h"

Controlador::Controlador() {
}

Controlador::~Controlador() {
}

bool Controlador::Synchronize(char *vect[], int num) {
    //Memoria compartida
    int idMem = shmget(60380, sizeof (AC), 0700 | IPC_CREAT);
    AC *area = (AC *) shmat(idMem, NULL, 0);
    //

    int param = num - 1; //parametros de entrada
    char *entradas[num - 1];
    char *salidas[num - 1];

    int cont = 0; //Se mueve por los indices de entrada y salida
    for (int i = 1; i < num; i++) {
        if ((string) vect[i] == "-e") {
            f.SetSpace(atoi(vect[i + 1]));
            i++;
        } else {
            entradas[cont] = vect[i];
            salidas[cont] = vect[i + 1];
            cont++;
            i++;
        }
    }

    int numProc = (param - 2) / 2; //ASUMIENDO QUE SIEMPRE VIENE EL -e ********************

    //Procesos
    int procNum; //Numero del proceso
    int id; // Para saber si es hijo o padre
    for (procNum = 1; procNum <= numProc; procNum++) {
        id = fork();
        if (-1 == id) {
            perror("Error en la creacion de un proceso");
            break;
        } else if (0 == id) {
            break;
        }
    }

    if (id == 0) {
        string entrada; // se definen variables para entrada y salida
        string salida;

        entrada = (string) entradas[procNum - 1]; // se les asigna un valor
        salida = (string) salidas[procNum - 1];

        f.InName(entrada); //se pasan al archivo
        f.OuName(salida);

        if (f.Read() == true) {
            for (int i = 0; i < 84; i++) {
                char* word = Convert(i); //Convierte un string a char* para despues enviarlo
                b.Enviar(word, f.data.count[i], (long) procNum);
                delete word; //Para liberar memoria que ya no se va a usar
            }
            b.Enviar((char *) "fin", -1, (long) numProc + 1);
            exit(0);
        } else {
            b.Enviar((char *) "fin", -1, (long) numProc + 1);
            exit(0);
        }

    } else {
        if (fork()) {
            for (int i = 0; i < 20; i++) {
                sP.Wait();
                Display(area); //Desplega lo que hay en la memoria compartida
                area->n = 0;
                cout << endl;
                sH.Signal();
            }
            exit(0);
        } else {
            WaitForAll(numProc); //Espera a que todos los procesos hayan enviado los mensajes

            char mensaje[32];
            int veces = 0;
            int sum = 0;
            char flag = 'a';
            for (int i = 0; i < 84; i++) {
                sum = 0;
                for (int i = 1; i <= numProc; i++) {
                    b.Recibir(mensaje, 32, veces, (long) i);
                    if (mensaje[0] != flag) {
                        sP.Signal();
                        sH.Wait();
                        flag = mensaje[0];
                    }
                    sum += veces;

                }
                CopyInfo(mensaje, sum, area);
            }
            sP.Signal();
            sH.Wait();
        }
    }
    shmdt(area);
    shmctl(idMem, IPC_RMID, NULL);
}

void Controlador::Display(AC *areaC) {
    for (int i = 0; i < areaC->n; i++) {
        if (areaC->Etiquetas[i].veces >= 0) {
            printf("Palabra: %s ", areaC->Etiquetas[i].palabra);
            cout <<"Total: "<< areaC->Etiquetas[i].veces << endl;
        }
    }
};

void Controlador::WaitForAll(int numProcs) {
    int cont = 0;
    int veces = 0;
    char mensaje[32];
    while (cont < numProcs) {
        b.Recibir(mensaje, 32, veces, (long) numProcs + 1);
        if (veces == -1) {
            cont++;
            veces = 0;
        }
    }
}

char* Controlador::Convert(int index) {

    string temp = f.data.resWords[index];
    char * newWord = new char[temp.size() + 1];
    strcpy(newWord, temp.c_str());

    return newWord;
}

void Controlador::CopyInfo(char* word, int sum, AC* zone) {
    strcpy(zone->Etiquetas[zone->n].palabra, word);
    zone->Etiquetas[zone->n].veces = sum;
    zone->n++;
}