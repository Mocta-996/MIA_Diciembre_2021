//
// Created by pilo-tuy on 5/12/21.
//

#ifndef PROYECTO_1_ESTRUCTURAS_H
#define PROYECTO_1_ESTRUCTURAS_H
#include <ctime>

class Estructuras {
public:
    Estructuras();
    // ============================== ESTRUCTURA PARA PARTICIONES  ===================================
    typedef struct
    {
        char part_status;             // indica estado de particion
        char part_type;                // tipo de particion primaria
        char part_fit ;                // tipo de ajuste de la particion
        int part_start;                  // indica en que parte del disco inicia la particion
        int part_size ;                  // contiene el tamaño total de la particion
        char part_name[16];            // nombre de la particion
    } Particion; //typedef

    // ================================== ESTRUCTURA DEL MBR PARA CREACION DE DISCOS ===================
    typedef struct
    {
        int mbr_tamano;                     // tamaño del  disco
        time_t mbr_fecha_creacion;          // fecha de creacion del disco
        int disk_signature;                 // identificacion unica del disco
        char disk_fit;                      // tipo de ajuste del disco
        Particion disco_particion[4];       // particiones del disco

    } MBRdisco;  //typedef

    // ====================================== ESTRUCTURA DEL EBR PARA PARTICI0NES ==================================
    typedef struct
    {
        char part_status;                   // indica si la particion esta activa o inactiva
        char part_fit;                      // tipo de ajuste de la particion
        int part_start;                     // posicion en byte de la particion
        int part_size;                      // contiene tamaño de la particion
        int part_next;                      // indica donde esta  la siguiente particion
        char disk_fit;                      // tipo de ajuste del disco
        char part_name[16];                 // contiene el nombre de la particion

    } EBRparticion;  //typedef

};


#endif //PROYECTO_1_ESTRUCTURAS_H
