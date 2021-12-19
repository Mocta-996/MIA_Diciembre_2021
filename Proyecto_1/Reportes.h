//
// Created by pilo-tuy on 14/12/21.
//

#ifndef PROYECTO_1_REPORTES_H
#define PROYECTO_1_REPORTES_H

#include "Globales.h"
#include "Estructuras.h"
#include <iostream>
#include <string>
using namespace std;
class Reportes {
public:
    Reportes();

    void Reporte_mbr(std::__cxx11::basic_string<char> id, std::__cxx11::basic_string<char> path);
    string Obtner_ebr_dot(Estructuras::MBRdisco mbr_rep, FILE *disco_b);

    string get_tablaebr(Estructuras::MBRdisco mbr_data, FILE *disco_base, int ext_size);

    void Reporte_disk(string id, string path);

    void Reporte_tree(string id, string path, string name);
};


#endif //PROYECTO_1_REPORTES_H
