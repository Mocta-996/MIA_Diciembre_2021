//
// Created by pilo-tuy on 6/12/21.
//

#ifndef PROYECTO_1_FDISCO_H
#define PROYECTO_1_FDISCO_H

#include "Estructuras.h"
#include <string>
#include <sstream>
#include <iostream>
#include "string.h"
#include <vector>

class Fdisco {
public:
    Fdisco();
    typedef struct {
        int inicio;
        int size;
        int final;
        int libre;
    } Espacio_libre;

    void
    Crear_particion(int size, std::string fit, std::string unit, std::string path, std::string type, std::string name);


    bool Verficarnombre(Estructuras::MBRdisco disco, std::string type, std::string name);

    void Agregar_espacio(int add, std::string unit, std::string path, std::string name);

    static void Eliminar_particion(std::string eliminar, std::string path, std::string name);

    int Ajuste(int size, Estructuras::MBRdisco disco, std::string ajuste);

};


#endif //PROYECTO_1_FDISCO_H
