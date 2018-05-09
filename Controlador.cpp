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
    char *entradas[param]; //contienen los nombres de archivos de entrada
    char *salidas[param];//contienen los nombres de archivos de salida

    int cont = 0; //Se mueve por los indices de entrada y salida
    for (int i = 1; i < num; i++) {
        if ((string) vect[i] == "-e") { //Si el parametro es de espacios
            f.SetSpace(atoi(vect[i + 1]));
            i++;
        } else { //Si es un nombre
            entradas[cont] = vect[i];
            salidas[cont] = vect[i + 1];
            cont++;
            i++;
        }
    }

    int numProc = (param - 2) / 2; //Numero de procesos totales, ASUMIENDO QUE SIEMPRE VIENE EL -e ********************

    //Procesos
    int procNum; //Numero del proceso
    int id; // Para saber si es hijo o padre
    for (procNum = 1; procNum <= numProc; procNum++) {
        id = fork();
        if (0 == id) {
            break;
        }
    }

    if (0 == id) { //Si es un proceso hijo
        string entrada; // se definen variables para entrada y salida
        string salida;

        entrada = (string) entradas[procNum - 1]; // se les asigna un valor
        salida = (string) salidas[procNum - 1];

        f.InName(entrada); //se pasan al archivo
        f.OuName(salida);

        if (f.Read() == true) { //Si se pudo abrir el archivo
            for (int i = 0; i < 84; i++) {
                char* word = Convert(i); //Convierte un string a char* para despues enviarlo
                b.Enviar(word, f.data.count[i], (long) procNum);
                delete word; //Para liberar memoria que ya no se va a usar
            }
            b.Enviar((char *) "fin", -1, (long) numProc + 1);//Mensaje con tipo mayor al numero de procesos indica que está listo
            exit(0);
        } else { //Si el archivo no se pudo abrir se van a enviar 0's junto con la palabra
            for (int i = 0; i < 84; i++) {
                char* word = Convert(i); //Convierte un string a char* para despues enviarlo
                b.Enviar(word, 0, (long) procNum);
                delete word; //Para liberar memoria que ya no se va a usar
            }
            b.Enviar((char *) "fin", -1, (long) numProc + 1); //Mensaje con tipo mayor al numero de procesos indica que está listo
            exit(0);
        }

    } else {
        if (fork()) { //Se crea el hijo desplegador
            for (int i = 0; i < 20; i++) { //Se hace 20 veces porque es el total de letras diferentes
                sP.Wait();     //Wait al semaforo del padre
                Display(area); //Desplega lo que hay en la memoria compartida
                area->n = 0;
                sH.Signal();   //Signal del hijo al padre
            }
            exit(0);
        } else {
            WaitForAll(numProc); //Espera a que todos los procesos hayan enviado los mensajes

            char mensaje[32];        //Almacenará la palabra a recibir
            int veces = 0;           //Almacenara las veces que aparece una palabra en un proceso
            int sum = 0;             // Almacenara las veces que aparece una palabra en todos los procesos
            char flag = 'a'; //Bandera que indica si hay cambio de letra
            for (int i = 0; i < 84; i++) { //Se hace la suma para las 84 palabras reservadas
                sum = 0;
                for (int i = 1; i <= numProc; i++) {
                    b.Recibir(mensaje, 32, veces, (long) i);
                    if (mensaje[0] != flag) { //Pregunta si hay cambio de letra
                        sP.Signal();
                        sH.Wait();
                        flag = mensaje[0];
                    }
                    sum += veces;

                }
                CopyInfo(mensaje, sum, area); //Copia la informacion de una palabra y su suma a memoria compartida
            }
            sP.Signal(); //Signal de padre a hijo
            sH.Wait(); //Wait al semaforo del hijo
        }
    }
    shmdt(area);
    shmctl(idMem, IPC_RMID, NULL); //Se borra el area compartida
}

void Controlador::Display(AC *areaC) { //Despliega informacion en pantalla
    for (int i = 0; i < areaC->n; i++) {
        if (areaC->Etiquetas[i].veces > 0) {
            cout << areaC->Etiquetas[i].palabra;
            cout << ": " << areaC->Etiquetas[i].veces <<" veces" << endl;
        }
    }
};

void Controlador::WaitForAll(int numProcs) { //Espera por todo los procesos
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

char* Controlador::Convert(int index) { //Convierte de string a char* (el parametro indica el indice de la palabra a convertir)
    string temp = f.data.resWords[index];
    char * newWord = new char[temp.size() + 1];
    strcpy(newWord, temp.c_str());
    return newWord;
}

void Controlador::CopyInfo(char* word, int sum, AC* zone) { //Copia informacion a memoria compartida
    strcpy(zone->Etiquetas[zone->n].palabra, word);
    zone->Etiquetas[zone->n].veces = sum;
    zone->n++;
}