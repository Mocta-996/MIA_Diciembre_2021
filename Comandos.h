//
// Created by pilo-tuy on 13/12/21.
//

#ifndef PROYECTO_1_COMANDOS_H
#define PROYECTO_1_COMANDOS_H

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include "Estructuras.h"
#include "Mount.h"
using namespace std;
using namespace std;
class Comandos {
public:
    Comandos();


    void mkdir(string path, Estructuras::Particiones_Montadas particion_mont, bool c);


    void mkfile(string contenido, Estructuras::Particiones_Montadas particion_mont,string path, bool c);
    bool Verificar_permiso(Estructuras::Inodo inodo, int per);

    string obtener_usuarios(string id);
};


#endif //PROYECTO_1_COMANDOS_H
