//
// Created by pilo-tuy on 12/12/21.
//

#ifndef PROYECTO_1_MOUNT_H
#define PROYECTO_1_MOUNT_H
#include <string>
#include <iostream>
#include <algorithm>
#include "Estructuras.h"
using namespace std;

class Mount {
public:
    Mount();
    static deque<Estructuras::Particiones_Montadas> Lista_part_montadas;
    void Montar(string path, string name);

    void UnMontar(string id);
};


#endif //PROYECTO_1_MOUNT_H
