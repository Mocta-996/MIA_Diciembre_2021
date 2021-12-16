//
// Created by pilo-tuy on 5/12/21.
//

#ifndef PROYECTO_1_MKDISCO_H
#define PROYECTO_1_MKDISCO_H


#include <string> //string
#include <vector>  // Libreria que contiene type Vector.
#include <sstream> //stringstream
#include <iostream> //cout, cin, getline, entre otras.
#include "string.h"
using namespace std;
class MKdisco {
public:
    MKdisco();

    void Crear_Disco(int size, std::__cxx11::basic_string<char> fit, std::__cxx11::basic_string<char> unit,
                     std::__cxx11::basic_string<char> path);

    string Fecha_actual();

    string formato_fecha(int field);

    string Obtener_fecha();
};


#endif //PROYECTO_1_MKDISCO_H
