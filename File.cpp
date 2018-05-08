#include "File.h"

File::File() {
}

File::~File() {
}

bool File::Read() { //Lee el archivo
    ifstream file(inName);
    if (file.is_open()) {
        ofstream salida;
        if (ouName != "") { //Si no hay nombre de salida no abre un archivo
            salida.open(ouName);
        }
        while (!file.eof()) {
            string line;
            getline(file, line);
            data.Search(line);
            NewLine(line);
        }
        if (ouName == "") {//Si no hay nombre de salida imprime en salida estandar
            cout << archivo;
        } else { //Sino finaliza la creacion del archivo
            salida << archivo;
            salida.close();
        }
        cout << "Lectura exitosa\n" << endl;
        return true;
    } else {
        cout << "No se pudo abrir el archivo / No existe" << endl;
        return false;
    }
}

void File::NewLine(string line) { //Procesa una linea y la agrega al archivo
    char charac = ' '; //obtiene caracter actual
    int index = -1;
    if (!data.indices.empty()) { //La pila index contiene las direcciones de las instrucciones "especiales"
        index = data.indices.front();
        data.indices.pop();
    }
    for (int i = 0; i < line.length(); i++) {
        if (i == index) {
            i = Exceptions(index, line, archivo); //Si se encuentra una especial hace otro trabajo
            if (!data.indices.empty()) {
                index = data.indices.front();
                data.indices.pop();
            } else {
                index = -1;
            }
        } else { //Si no, entonces analiza el caracter 
            charac = line[i];
            switch (charac) {//Acciones diferentes para caracteres "Especiales"
                case ';':
                    if (*--archivo.end() == '\n') {
                        archivo.erase(archivo.length() - 1);
                    }
                    archivo += ";\n";

                    archivo.append(openKeys * space, ' ');
                    break;
                case '{':
                    if (*--archivo.end() == '\n') {
                        archivo.erase(archivo.length() - 1);
                        archivo += " {\n";
                    } else if (*--archivo.end() == ' ') {
                        archivo += "{\n";
                    } else {
                        archivo += " {\n";
                    }
                    openKeys++;
                    archivo.append(openKeys * space, ' ');
                    break;

                case'}':
                    if (*--archivo.end() == '\n') {
                        archivo.append((openKeys - 1) * space, ' ');
                        if (line[i + 1] == ';') {
                            archivo += "};\n";
                            i++;
                        } else {
                            archivo += "}\n";
                        }
                        archivo.append((openKeys - 1) * space, ' ');
                    } else {
                        archivo.erase(archivo.length() - space, space);
                        if (line[i + 1] == ';') {
                            archivo += "};\n";
                            i++;
                        } else {
                            archivo += "}\n";
                        }
                        archivo.append((openKeys - 1) * space, ' ');
                    }
                    openKeys--;
                    if (openKeys == 0 && archivo[archivo.length() - 2] != '\n') {
                        archivo += "\n";
                    }
                    break;
                case'[':
                case'(':
                    if (*--archivo.end() == ' ') {
                        archivo.erase(--archivo.end());
                    }
                    archivo += charac;
                    break;
                case']':
                case')':
                    if (*--archivo.end() == ' ') {
                        archivo.erase(--archivo.end());
                    }
                    archivo += charac;

                    break;
                case'\"':
                    archivo += charac;
                    bool comment;
                    comment = true;
                    int cont;
                    cont = i + 1;
                    while (comment) {
                        if (cont == index) {

                        }
                        if ((line[cont] == '\"') && (*--archivo.end() != ('\\'))) {
                            archivo += line[cont];
                            comment = false;
                        } else {
                            archivo += line[cont];
                        }
                        cont++;
                    }
                    i = cont - 1;
                    break;
                case' ':
                    if (*--archivo.end() != ' ' && *--archivo.end() != '(') {
                        archivo += " ";
                    }
                    break;
                case'/':
                    archivo.erase((archivo.length() - (space * (openKeys))), archivo.length());
                    archivo += line;
                    archivo += '\n';
                    archivo.append(openKeys * space, ' ');
                    i = i = line.length() + 1;
                    break;

                case'\'':
                    if (line[i + 1] == '\\') {
                        archivo += '\'';
                        archivo += line[i + 1];
                        archivo += line[i + 2];
                        archivo += '\'';
                        i = i + 3;
                    } else {
                        archivo += '\'';
                        archivo += line[i + 1];
                        archivo += '\'';
                        i = i + 2;
                    }
                    break;
                case'\n':
                    if (*--archivo.end() != '\n') {
                        archivo += "\n";
                    }
                    break;
                case'\t':
                    break;
                default:
                    archivo += charac;
                    break;
            }
        }
    }
    if ((*--archivo.end() != '\n' && *--archivo.end() != ' ') || line == "") {

        archivo += "\n";
        archivo.append(openKeys * space, ' ');
    }
}

void File::Apparitions() { //guarda las apariciones de las palabras reservadas en el contador de cada una
    for (int i = 0; i < 84; i++) {
        if (data.count [i] != 0) {
            //name << data.resWords [i] + ": ";
            //name << data.count [i];
            //name << "\n";
        }
    }

}

int File::Exceptions(int index, string line, string & file) { //Metodo para instrucciones "especiales"
    char ex = line[index];
    int cont;
    switch (ex) {
        case 'b': //Break
            openKeys--;
            archivo += "break";
            return index + 4;
            break;
        case 'c': //Case
            file += "case";
            cont = index + 4;
            while (line[cont] != ':') {
                if (*--file.end() == ' ' && line[cont] == ' ') {
                    cont++;
                } else {
                    file += line[cont];
                    cont++;
                }
            }
            openKeys++;
            archivo += ":\n";
            archivo.append(openKeys * space, ' ');
            return cont;
            break;

        case 'f': //For
            file += "for";
            cont = index + 3;
            while (line[cont] != ')') {
                if (*--file.end() == ' ' && line[cont] == ' ') {
                    cont++;
                } else {
                    file += line[cont];
                    cont++;
                }
            }
            archivo += ")";
            return cont;
            break;

        case 'd': //Default
            file += "default";
            cont = index + 4;
            while (line[cont] != ':') {
                if (*--file.end() == ' ' && line[cont] == ' ') {
                    cont++;
                } else {
                    file += line[cont];
                    cont++;
                }
            }
            openKeys++;
            archivo += ":\n";
            archivo.append(openKeys * space, ' ');
            return cont;
            break;
    }
}

void File::InName(string name) { //Recibe un nombre de entrada y lo asigna
    inName = name;
}

void File::OuName(string name) { //Recibe un nombre de salida y lo asigna
    ouName = name;
}

void File::SetSpace(int num) {  //Recibe los espacios para identar y lo asigna
    space = num;
}