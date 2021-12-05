//
// Created by pilo-tuy on 4/12/21.
// Manejo e Implementación de Archivos Vacaciones 2021
// CLASE ANALIZADOR PARA INTERPRETAR LOS COMANDOS DE ENTRADA

#ifndef PROYECTO_1_ANALIZADOR_H
#define PROYECTO_1_ANALIZADOR_H
#include <string> //string
#include <vector>  // Libreria que contiene type Vector.
#include <sstream> //stringstream
#include <iostream> //cout, cin, getline, entre otras.
#include <algorithm>
#include <cctype>
#include <fstream>
using namespace std;


class Analizador {
public:
    Analizador();
    void Analizar();


    void Analizar_Comando(std::__cxx11::basic_string<char> _entrada);


    string StringToLower(string comando);

    string RutaCompleta(vector<string> ruta, int pos);

    void verificar_mkdisk(vector<string> lineainstruccion);

    string AtributoComando(string comando);

    int RutaCompletaSaltos(vector<string> ruta, int pos);

    void verificar_rmdisk(vector<string> linecomand);
};

#endif //PROYECTO_1_ANALIZADOR_H
