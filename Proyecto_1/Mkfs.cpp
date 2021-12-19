//
// Created by pilo-tuy on 12/12/21.
//

#include "Mkfs.h"
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
#define YLLW "\e[0;33m"
#include <string>
#include <iostream>
#include <algorithm>
#include "Estructuras.h"
#include "Comandos.h"
#include "Globales.h"
#include "Fdisco.h"
using namespace std;
Mkfs::Mkfs() {
}
void Mkfs::formateo_particion(string id, string type,string fs){
    bool bandera = false;
    if(fs=="3fs"){
        // BUSCAR PARTICION MONTADA
        Estructuras::Particiones_Montadas particion_montada;
        for (int i = 0; i <Lista_part_montadas.size();i++) {
            if (Lista_part_montadas.at(i).id == id) {
                particion_montada = Lista_part_montadas.at(i);
                bandera=true;
            }
        }

        // INICIAR SESION
        //Estructuras::Usuario_global  user_g;
        user_g.logged = 1;
        user_g.uid = "1";
        user_g.user = "root";
        user_g.pwd = "123";
        user_g.group = "root";
        user_g.id = particion_montada.id;
        user_g.gid = "1";

        // SE ENCONTRO
        if (bandera) {

            Estructuras::MBRdisco  disco_mbr;
            FILE *disco_base = fopen(particion_montada.path.c_str(), "rb+");
            if (disco_base != NULL) {
                // OBTENER MBR
                fseek(disco_base, 0, SEEK_SET);
                fread(&disco_mbr, sizeof(Estructuras::MBRdisco ), 1, disco_base);

                // CALCULO NUMERO DE INODOS Y BLOQUES
                int n = ((particion_montada.size - sizeof(Estructuras::Super_bloque )) /
                         (4 + sizeof(Estructuras::Journal) + sizeof(Estructuras::Inodo) + 3 * sizeof(Estructuras::Bloque_archivos)));

               /* if (type == "full") {
                    string name = particion_montada.name;
                    string path = particion_montada.path;
                    string del = type;
                    Fdisco::Eliminar_particion(del,path,name);
                }*/

                // CREAR SUPERBLOQUE
                Estructuras::Super_bloque super_block;
                int fs_type = (int)(fs.at(0) - '0');

                super_block.s_mnt_count = 1;
                super_block.s_magic = 0xEF53;
                super_block.s_first_ino = 0;
                super_block.s_first_blo = 0;
                super_block.s_inodes_count = n;
                super_block.s_blocks_count = 3 * n;
                super_block.s_free_inodes_count = n - 2;
                super_block.s_free_blocks_count = (3 * n) - 2;
                super_block.s_inode_size = sizeof(Estructuras::Inodo );
                super_block.s_block_size = sizeof(Estructuras::Bloque_archivos);
                super_block.s_bm_inode_start = particion_montada.start + sizeof(Estructuras::Super_bloque ) +(fs_type == 3 ? 100 * sizeof(Estructuras::Journal) : 0);
                super_block.s_filesystem_type = fs_type;
                super_block.s_bm_block_start = super_block.s_bm_inode_start + n;
                super_block.s_inode_start = super_block.s_bm_block_start + 3 * n;
                super_block.s_block_start = super_block.s_inode_start + n * sizeof(Estructuras::Inodo);

                // Obteniendo fecha
                string def_time = "";
                /*string current_date = time(nullptr);
                strcpy(super_block.s_umtime, def_time.c_str());*/

                if (particion_montada.type != 'e') {
                    fseek(disco_base, particion_montada.start, SEEK_SET);
                    fwrite(&super_block, sizeof(Estructuras::Super_bloque ), 1, disco_base);

                    // CREAR INODOS Y BLOQUES
                    char bit_inodos[n];
                    char bit_bloques[3 * n];
                    for (int i = 0; i < n; i++)
                    {
                        bit_inodos[i] = '0';
                    }

                    fseek(disco_base, super_block.s_bm_inode_start, SEEK_SET);
                    fwrite(&bit_inodos, n, 1, disco_base);

                    // BLOQUES
                    for (int i = 0; i < 3 * n; i++){
                        bit_bloques[i] = '0';
                    }


                    fseek(disco_base, super_block.s_bm_block_start, SEEK_SET);
                    fwrite(&bit_bloques, 3 * n, 1, disco_base);

                    // JOURNALING
                    if (fs_type == 3) {
                        int journalstart = (particion_montada.start + sizeof(Estructuras::Super_bloque ));
                        Estructuras::Journal journal_data;
                        journal_data.tipo_operacion[0] = '-';
                        journal_data.contenido[0] = '\0';
                        journal_data.permiso = -1;
                        journal_data.propietario[0] = '\0';
                        journal_data.name[0] = '\0';
                        journal_data.fecha[0] = '\0';
                        journal_data.tipo = '-';

                        // ESCRIBO TODOS LOS JOURNAL
                        for (int i = 0; i < 100; i++) {
                            fseek(disco_base, (journalstart + (i * sizeof(Estructuras::Journal))),
                                  SEEK_SET);
                            fwrite(&journal_data, sizeof(Estructuras::Journal), 1, disco_base);
                        }
                    }

                    fclose(disco_base);

                    //== CREANDO RUTA
                   // particion_montada, path , bool
                    string def_users = "1,G,root\n1,U,root,root,123\n";
                    Comandos m;
                    m.mkdir("/",particion_montada,0);
                    m.mkfile(def_users,particion_montada, "/users.txt",1);
                    // CERRAR SESION
                    user_g.logged = 0;
                    user_g.uid = "";
                    user_g.user = "";
                    user_g.pwd = "";
                    user_g.group = "";
                    user_g.id = "";
                    user_g.gid = "";
                }

            } else{
                cout<<RED"EL DISCO NO EXISTE  "<<CYN""<<endl;
            }
        } else{
            cout<<RED"PARTICION NO MONTADA  "<<YLLW ""<< id<<CYN""<<endl;
        }

    }else {
        // BUSCAR PARTICION MONTADA
        Estructuras::Particiones_Montadas particion_montada;
        for (int i = 0; i <Lista_part_montadas.size();i++) {
            if (Lista_part_montadas.at(i).id == id) {
                particion_montada = Lista_part_montadas.at(i);
                bandera=true;
            }
        }

        // INICIAR SESION
        Estructuras::Usuario_global  user_g;
        user_g.logged = 1;
        user_g.uid = "1";
        user_g.user = "root";
        user_g.pwd = "123";
        user_g.group = "root";
        user_g.id = particion_montada.id;
        user_g.gid = "1";

        // SE ENCONTRO
        if (bandera) {

            Estructuras::MBRdisco  disco_mbr;
            FILE *disco_base = fopen(particion_montada.path.c_str(), "rb+");
            if (disco_base != NULL) {
                // OBTENER MBR
                fseek(disco_base, 0, SEEK_SET);
                fread(&disco_mbr, sizeof(Estructuras::MBRdisco ), 1, disco_base);

                // CALCULO NUMERO DE INODOS Y BLOQUES
                int n = ((particion_montada.size - sizeof(Estructuras::Super_bloque )) /
                         (4 + sizeof(Estructuras::Journal) + sizeof(Estructuras::Inodo) + 3 * sizeof(Estructuras::Bloque_archivos)));

                // BORRAR CON FDISK
                if (type == "full") {
                    string name = particion_montada.name;
                    string path = particion_montada.path;
                    string del = type;
                    Fdisco::Eliminar_particion(del,path,name);
                }

                // CREAR SUPERBLOQUE
                Estructuras::Super_bloque super_block;
                int fs_type = (int)(fs.at(0) - '0');

                super_block.s_mnt_count = 1;
                super_block.s_magic = 0xEF53;
                super_block.s_first_ino = 0;
                super_block.s_first_blo = 0;
                super_block.s_inodes_count = n;
                super_block.s_blocks_count = 3 * n;
                super_block.s_free_inodes_count = n - 2;
                super_block.s_free_blocks_count = (3 * n) - 2;
                super_block.s_inode_size = sizeof(Estructuras::Inodo );
                super_block.s_block_size = sizeof(Estructuras::Bloque_archivos);
                super_block.s_bm_inode_start = particion_montada.start + sizeof(Estructuras::Super_bloque ) +(fs_type == 3 ? 100 * sizeof(Estructuras::Journal) : 0);
                super_block.s_filesystem_type = fs_type;
                super_block.s_bm_block_start = super_block.s_bm_inode_start + n;
                super_block.s_inode_start = super_block.s_bm_block_start + 3 * n;
                super_block.s_block_start = super_block.s_inode_start + n * sizeof(Estructuras::Inodo);

                // Obteniendo fecha
                string def_time = "";
                /*string current_date = time(nullptr);
                strcpy(super_block.s_umtime, def_time.c_str());*/

                if (particion_montada.type != 'e') {
                    fseek(disco_base, particion_montada.start, SEEK_SET);
                    fwrite(&super_block, sizeof(Estructuras::Super_bloque ), 1, disco_base);

                    // CREAR INODOS Y BLOQUES
                    char bit_inodos[n];
                    char bit_bloques[3 * n];
                    for (int i = 0; i < n; i++)
                    {
                        bit_inodos[i] = '0';
                    }

                    fseek(disco_base, super_block.s_bm_inode_start, SEEK_SET);
                    fwrite(&bit_inodos, n, 1, disco_base);

                    // BLOQUES
                    for (int i = 0; i < 3 * n; i++){
                        bit_bloques[i] = '0';
                    }


                    fseek(disco_base, super_block.s_bm_block_start, SEEK_SET);
                    fwrite(&bit_bloques, 3 * n, 1, disco_base);
                    fclose(disco_base);

                    //== CREANDO RUTA
                    // particion_montada, path , bool
                    string def_users = "1,G,root\n1,U,root,root,123\n";
                    Comandos m;
                    m.mkdir("/",particion_montada,0);
                    m.mkfile(def_users,particion_montada, "/users.txt",1);
                    // CERRAR SESION
                    user_g.logged = 0;
                    user_g.uid = "";
                    user_g.user = "";
                    user_g.pwd = "";
                    user_g.group = "";
                    user_g.id = "";
                    user_g.gid = "";
                }

            } else{
                cout<<RED"EL DISCO NO EXISTE  "<<CYN""<<endl;
            }
        } else{
            cout<<RED"PARTICION NO MONTADA  "<<YLLW ""<< id<<CYN""<<endl;
        }

    }

}