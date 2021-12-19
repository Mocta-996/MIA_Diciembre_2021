//
// Created by pilo-tuy on 5/12/21.
//

#ifndef PROYECTO_1_ESTRUCTURAS_H
#define PROYECTO_1_ESTRUCTURAS_H
#include <ctime>
#include <string>
#include <deque>
using namespace std;
class Estructuras {
public:
    Estructuras();

    // ============================== ESTRUCTURA PARA PARTICIONES  ===================================
    typedef struct
    {
        char part_status ;             // indica estado de particion/ '0' no usado /'1' usado
        char part_type ;                // tipo de particion primaria: p,e,l
        char part_fit ;                // tipo de ajuste de la particion: wf,ff,bf
        int part_start ;                  // indica en que parte del disco inicia la particion -1
        int part_size ;                  // contiene el tamaño total de la particion 0
        char part_name[16];            // nombre de la particion
    } Particion; //typedef

    // ================================== ESTRUCTURA DEL MBR PARA CREACION DE DISCOS ===================
    typedef struct
    {
        int mbr_tamano ;                     // tamaño del  disco
        char mbr_fecha_creacion[17];          // fecha de creacion del disco
        int disk_signature ;                 // identificacion unica del disco
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
    // ===================================== MONTAR PARTICIONES ===========================================
    typedef  struct  {
        string name;
        string path;
        char type;
        string id;
        int start;
        int size;
        int letra;
        int disco;
    } Particiones_Montadas;
    //================================ USUARIOS ===================
    typedef  struct  {
        bool logged;
        string uid;
        string gid;
        string user;
        string pwd;
        string group;
        string id;
    } Usuario_global;

    //================================ SUPER BLOQUE ===================
    typedef  struct  {
        int s_filesystem_type;
        int s_inodes_count;
        int s_blocks_count;
        int s_free_inodes_count;
        int s_free_blocks_count;
        time_t s_umtime;
        time_t s_mtime;
        int s_mnt_count;
        int s_magic;
        int s_inode_size;
        int s_block_size;
        int s_first_ino;
        int s_first_blo;
        int s_bm_inode_start;
        int s_bm_block_start;
        int s_inode_start;
        int s_block_start;
    } Super_bloque;

    typedef struct  {
        char tipo_operacion[10];    //operation[10]
        char tipo;                  //type
        char name[200];             // name
        char contenido[200];        //content
        int  permiso;            //permission
        char fecha[17];             //date
        char propietario[10];             //owner
    }Journal;

    typedef struct {
        int i_uid;
        int i_gid;
        int i_size;
        char i_atime[17];
        char i_ctime[17];
        char i_mtime[17];
        int i_block[15];
        char i_type;
        int i_perm;
    }Inodo;

    // ================== BLOQUE DE CONTENIDO ==================0
    typedef struct  {
        char b_name[12];
        int b_inodo;
    }Content;

    typedef struct  {
        Content b_content[4];
    }Bloque_carpeta; //dirBlock
    //==============================  bloque de archivos=================
    typedef struct  {
        char content[64];
    }Bloque_archivos;
};


#endif //PROYECTO_1_ESTRUCTURAS_H
