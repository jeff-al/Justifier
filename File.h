#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include <iostream>
#include "Data.h"

using namespace std;

class File {
public:
    int openKeys = 0;
    string archivo = "";
    int space = 5;
    string inName = "";
    string ouName = "";

    File();

    ~File();

    void InName(string); //Da un nombre de entrada

    void OuName(string); //Da un nombre de salida

    void SetSpace(int); //Da espacios de identacion

    bool Read(); //Leer un archivo de entrada
    
     Data data;

private:
    
    int Exceptions(int, string, string&); //Maneja instrucciones especiales
    
    void NewLine(string); //Actualiza una linea analizada y la escribe en el archivo de salida
    
    void Apparitions(ofstream&); //Guarda las apariciones de las palabras reservadas en un archivo  

};

#endif /* FILE_H */

