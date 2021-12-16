//
// Created by pilo-tuy on 13/12/21.
//

#ifndef PROYECTO_1_GLOBALES_H
#define PROYECTO_1_GLOBALES_H

#include "Estructuras.h"
#include <deque>
 struct Usuario_global  {
    bool logged;
    string uid;
    string gid;
    string user;
    string pwd;
    string group;
    string id;
} ;

extern deque<Estructuras::Particiones_Montadas> Lista_part_montadas;
extern struct  Usuario_global user_g ;
#endif //PROYECTO_1_GLOBALES_H
