#ifndef DATA_H
#define DATA_H

#include <queue>
#include <string>

using namespace std;

class Data {
public:

    Data();
    ~Data();
    queue<int> indices; //Cola de las instrucciones especiales
  
    string resWords[84] = { //Palabras reservadas de c++
        "alignas", "alignof", "and", "and_eq", "asm", "auto", //0-5
        "bitand", "bitor", "bool", "break", //6-9
        "case", "catch", "char", "char16_t", "char32_t", "class", "compl", "const", "constexpr", "const_cast", "continue", //10-20
        "decltype", "default", "delete", "do", "double", "dynamic_cast", //21-26
        "else", "enum", "explicit", "export", "extern", //27-31
        "false", "float", "for", "friend", //32-35
        "goto", //36
        "if", "inline", "int", //37-39
        "long", //40
        "mutable", //41
        "namespace", "new", "noexcept", "not", "not_eq", "nullptr", //42-47
        "operator", "or", "or_eq", //48-50
        "private", "protected", "public", //51-53
        "register", "reinterpret_cast", "return", //54-56
        "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", //57-64
        "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", //65-73
        "union", "unsigned", "using", //74-76
        "virtual", "void", "volatile", //77-79
        "wchar_t", "while", // 80-81
        "xor", "xor_eq" //82-83
    };

    int count[84] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Contador de palabras reservadas

    bool Search(string); //Busca si  una palabra está reservada

    void Add(int); //Suma al contador de palabras reservadas

private:

};

#endif /* DATA_H */

