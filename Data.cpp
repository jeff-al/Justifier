#include "Data.h"

Data::Data() {
}

Data::~Data() {
}

void Data::Add(int ind) {
    count[ind]++;
}

bool Data::Search(string word) { //Buca las palabras reservadas
    std::size_t index;
    std::size_t index2;
    index = 0;
    index2 = 0;
    index = word.find("//", index); //Busca si hay un comentario de la forma //
    if (index != string::npos) {
        index2 = word.find('\"', index2); //Busca si ese comentario no está dentro de llaves
        if (index2 != string::npos && index2 > index) {
        return false;
        }
    }
    for (int i = 0; i < 84; i++) { //Hace las busqueda de las palabras
        index = 0;
        do {
            index = word.find(resWords[i], index);
            if (index != string::npos) {
                if ((word[index - 1] >= 65 && word[index - 1] <= 87) || (word[index - 1] >= 97 && word[index - 1] <= 122)) {//Para saber que no hay mas letras detras de la palabra
                    index++;
                } else if ((word[index + resWords[i].length()] >= 65 && word[index + resWords[i].length()] <= 87) || (word[index + resWords[i].length()] >= 97 && word[index + resWords[i].length()] <= 122)) { //Para saber que no hay mas letras delante de la palabra
                    index++;
                } else {
                    if (resWords[i] == "break" || resWords[i] == "case" || resWords[i] == "for" || resWords[i] == "default") { //Pregunta si no es una "Especial"
                        indices.push(index);
                    }
                    index++;
                    Add(i);
                }
            }
        } while (index != string::npos);
    }
    return true;
}


