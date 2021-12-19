//
// Created by pilo-tuy on 12/12/21.
//

#ifndef PROYECTO_1_MKFS_H
#define PROYECTO_1_MKFS_H
#include <string>
#include <iostream>
#include <algorithm>
#include "Estructuras.h"
#include "Mount.h"
using namespace std;


class Mkfs {
public:
    Mkfs();

    void formateo_particion(string id, string type, string fs);
};


#endif //PROYECTO_1_MKFS_H
