#include "Controlador.h"
#include <cstdlib>


int main(int argc, char *argv[]) {
    Controlador controller;
    
    if(argc > 1){
    controller.Synchronize(argv, argc);
    }else{
        cout << "Se necesitan más parametros" << endl;
    }
    return 0;
}



