//
// Created by pilo-tuy on 13/12/21.
//

#include "Comandos.h"
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
#define YLLW "\e[0;33m"
#include <string>
#include <iostream>
#include <vector>
#include "Estructuras.h"
#include "Globales.h"
#include <bits/stdc++.h>
using namespace std;
Comandos::Comandos() {

}
//======================================= CREACION DE CARPETAS ==================================
void Comandos::mkdir(string path, Estructuras::Particiones_Montadas particion_mont, bool c){

    // obteniendo super bloque
    Estructuras::Super_bloque super_b;
    int inicio_particion = particion_mont.start;
    FILE *disco_base = fopen(particion_mont.path.c_str(), "rb+");

    if (disco_base != NULL) {
        fseek(disco_base, inicio_particion, SEEK_SET);
        fread(&super_b, sizeof(Estructuras::Super_bloque), 1, disco_base);

        // ======================== BITMAP DE INODOS Y BLOQUES
        char bitinodes[super_b.s_inodes_count];
        char bitblocks[super_b.s_blocks_count];

        fseek(disco_base, super_b.s_bm_inode_start, SEEK_SET);
        fread(&bitinodes, sizeof(bitinodes), 1, disco_base);

        fseek(disco_base, super_b.s_bm_block_start, SEEK_SET);
        fread(&bitblocks, sizeof(bitblocks), 1, disco_base);

        //============================  BUSCAR PRIMER INODO Y BLOQUE LIBRE
        int indice_inodo = super_b.s_first_ino;
        int indice_bloque = 0;

        //========================== GLOBALES
        string fecha_actual = to_string(time(nullptr));
        stringstream ss(path);
        string item;
        vector<string> path_split;
        while (getline(ss, item, '/')){
            path_split.push_back(item);
        }
        string nuevo_path  = path.at(path_split.size() - 1) == '/'? path.substr(0, path.size() - 2): path;
        int path_cmp_size = path_split.size();
        path_split.at(0) = "/";

        //===================================== PRIMER INODO
        if (path_cmp_size == 1) {
            Estructuras::Inodo primer_inodo;
            strcpy(primer_inodo.i_atime, fecha_actual.c_str());
            strcpy(primer_inodo.i_ctime, fecha_actual.c_str());
            strcpy(primer_inodo.i_mtime, fecha_actual.c_str());

            for (int i = 0; i < 15; i++) {
                primer_inodo.i_block[i] = -1;
            }

            primer_inodo.i_type = '1';
            primer_inodo.i_size = 0;
            primer_inodo.i_perm = 664;
            primer_inodo.i_uid = (int)stoi(user_g.uid);
            primer_inodo.i_gid = (int)stoi(user_g.gid);

            fseek(disco_base, super_b.s_inode_start, SEEK_SET);
            fwrite(&primer_inodo, sizeof(Estructuras::Inodo), 1, disco_base);

            bitblocks[0] = '1';
            bitinodes[0] = '1';
        } else {
            bool recursivo = 0;
            Estructuras::Inodo inodo_ac;
            int inode_start = super_b.s_inode_start;
            fseek(disco_base, inode_start, SEEK_SET);
            fread(&inodo_ac, sizeof(Estructuras::Inodo), 1, disco_base);

            int ruta_contador = 1;
            int inodo_siguiente = indice_inodo;

            // CREAR CARPETAS SI NO EXISTEN
            string aux_dir;
            vector<string> b_dir;
            Estructuras::Inodo aux_inodo = inodo_ac;
            for (int i = 1; i < path_split.size() - 1; i++) {
                bool existe_dir = 0;
                aux_dir += "/" + path_split.at(i);
                b_dir.push_back(aux_dir);

                for (int j = 0; j < 15; j++) {
                    if (aux_inodo.i_block[j] != -1) {
                        Estructuras::Bloque_carpeta bloque_dir;
                        fseek(disco_base, aux_inodo.i_block[j], SEEK_SET);
                        fread(&bloque_dir, sizeof(Estructuras::Bloque_carpeta), 1, disco_base);

                        // BUSCAR CONTENIDO
                        for (int k = 0; k < 4; k++) {
                            if (bloque_dir.b_content[k].b_inodo != -1) {
                                string block_name(bloque_dir.b_content[k].b_name);
                                if (block_name == path_split.at(i)) {
                                    fseek(disco_base, bloque_dir.b_content[k].b_inodo,
                                          SEEK_SET);
                                    fread(&aux_inodo, sizeof(Estructuras::Inodo), 1, disco_base);
                                    existe_dir = 1;
                                }
                            }
                        }
                    }
                }

                //============  NO EXISTE
                if (!existe_dir) {
                    recursivo = 1;
                }
            }

            // CREAR CARPETAS FALTANTES
            if (recursivo) {
                if (c) {
                    for (int i = 0; i < b_dir.size();i++) {
                         mkdir(b_dir.at(i), particion_mont, 0);
                    }
                    mkdir(nuevo_path, particion_mont, 0);
                } else {
                    cout <<RED "NO EXISTE LA RUTA! " << CYN""<< endl;
                }
                return;
            }

            //========================== buscar inodos
            for (int i = 0; i < path_split.size(); i++) {
                for (int j = 0; j < 15; j++) {
                    if (inodo_ac.i_block[j] != -1) {
                        Estructuras::Bloque_carpeta bloque_d;
                        fseek(disco_base, inodo_ac.i_block[j], SEEK_SET);
                        fread(&bloque_d, sizeof(Estructuras::Bloque_carpeta ), 1, disco_base);

                        //================================== buscar contenido en inodos
                        for (int k = 0; k < 4; k++) {
                            if (bloque_d.b_content[k].b_inodo != -1) {
                                string nombre_b(bloque_d.b_content[k].b_name);
                                if (nombre_b == path_split.at(i)) {
                                    ruta_contador++;
                                    inode_start = bloque_d.b_content[k].b_inodo;
                                    fseek(disco_base, inode_start, SEEK_SET);
                                    fread(&inodo_ac, sizeof(Estructuras::Inodo ), 1, disco_base);
                                    inodo_siguiente =(int)(((super_b.s_inode_start - inode_start) * -1) /sizeof(Estructuras::Inodo));
                                }
                            }
                        }
                    }
                }
            }

            //============================ USAR INODO===============
            bool direc_repetido = ruta_contador == path_split.size();
            indice_inodo = direc_repetido ? inodo_siguiente : indice_inodo;
            bitinodes[indice_inodo] = '1';

            if (!direc_repetido)
                super_b.s_free_inodes_count = super_b.s_free_inodes_count - 1;

            bool nuevo_blo = 0;
            bool bloque_l = 0;
            int posicion_bloque[2] = {0, 1};

            //==================== BUSQUEDA EN APUNTADORES DIRECTOS
            for (int i = 0; i < 15; i++) {
                // BLOQUE OCUPADO
                if (inodo_ac.i_block[i] != -1) {
                    // BLOQUE ACTUAL
                    Estructuras::Bloque_carpeta block_c;
                    posicion_bloque[0] = i;
                    fseek(disco_base, inodo_ac.i_block[i], SEEK_SET);
                    fread(&block_c, sizeof(Estructuras::Bloque_carpeta), 1, disco_base);

                    // BUSCAR ESPACIO EN CONTENIDO DEL BLOQUE
                    for (int j = 0; j < 4; j++) {
                        posicion_bloque[1] = j;
                        Estructuras::Content content = block_c.b_content[j];
                        if (content.b_inodo == -1) {
                            bloque_l = 1;
                            break;
                        }
                    }

                    // PRIMER BLOQUE LIBRE
                    if (bloque_l) {
                        break;
                    }
                }
                    // SINO CREAR UN NUEVO BLOQUE
                else {
                    posicion_bloque[0] = i;
                    posicion_bloque[1] = 0;
                    bloque_l = 1;
                    nuevo_blo = 1;
                    break;
                }
            }

            // SE ENCONTRO UN BLOQUE DISPONIBLE
            if (bloque_l && !direc_repetido) {
                // CREAR UN CONTENIDO
                Estructuras::Content nuevo_bloque;
                strcpy(nuevo_bloque.b_name,path_split.at(path_split.size() - 1).c_str());

                // CREAR UN NUEVO INODO
                Estructuras::Inodo nuevo_inodo;
                strcpy(nuevo_inodo.i_atime, fecha_actual.c_str());
                strcpy(nuevo_inodo.i_ctime, fecha_actual.c_str());
                strcpy(nuevo_inodo.i_mtime, fecha_actual.c_str());
                nuevo_inodo.i_type = '1';
                nuevo_inodo.i_size = 0;
                nuevo_inodo.i_perm = 664;
                nuevo_inodo.i_uid = (int)stoi(user_g.uid);
                nuevo_inodo.i_gid = (int)stoi(user_g.gid);
                for (int i = 0; i < 15; i++) {
                    nuevo_inodo.i_block[i] = -1;
                }
                nuevo_bloque.b_inodo =super_b.s_inode_start + (sizeof(Estructuras::Inodo) * indice_inodo);
                fseek(disco_base, nuevo_bloque.b_inodo, SEEK_SET);
                fwrite(&nuevo_inodo, sizeof(Estructuras::Inodo), 1, disco_base);

                Estructuras::Bloque_carpeta  current_block;
                if (!nuevo_blo) {
                    fseek(disco_base, inodo_ac.i_block[posicion_bloque[0]], SEEK_SET);
                    fread(&current_block, sizeof(Estructuras::Bloque_carpeta), 1, disco_base);
                    indice_bloque =(int)(((super_b.s_block_start -inodo_ac.i_block[posicion_bloque[0]]) * -1) / sizeof(Estructuras::Bloque_carpeta ));

                } else {
                    for (int j = 0; j < 4; j++) {
                        current_block.b_content[j] = (Estructuras::Content){{}, -1};
                    }

                    // OCUPAR UN NUEVO BLOQUE
                    super_b.s_free_blocks_count = super_b.s_free_blocks_count - 1;
                    indice_bloque = super_b.s_first_blo;
                    bitblocks[indice_bloque] = '1';
                }

                current_block.b_content[posicion_bloque[1]] = nuevo_bloque;
                inodo_ac.i_block[posicion_bloque[0]] =
                        super_b.s_block_start + (sizeof(Estructuras::Bloque_archivos) * indice_bloque);

                // GUARDAR BLOQUE DE INODO PADRE
                fseek(disco_base, inodo_ac.i_block[posicion_bloque[0]], SEEK_SET);
                fwrite(&current_block, sizeof(Estructuras::Bloque_carpeta), 1, disco_base);

                // CAMBIOS EN INODO PADRE
                strcpy(inodo_ac.i_atime, fecha_actual.c_str());
                strcpy(inodo_ac.i_mtime, fecha_actual.c_str());

                fseek(disco_base, inode_start, SEEK_SET);
                fwrite(&inodo_ac, sizeof(Estructuras::Inodo), 1, disco_base);
            }
        }

        // ASIGNAR A SUPERBLOQUE
        int p_inodo = 0;
        int p_block = 0;
        for (int i = 0; i < sizeof(bitinodes); i++) {
            if (bitinodes[i] == '0') {
                p_inodo = i;
                break;
            }
        }
        for (int j = 0; j < sizeof(bitblocks); j++) {
            if (bitblocks[j] == '0') {
                p_block = j;
                break;
            }
        }

        // GUARDAR CAMBIOS EN SUPERBLOQUE Y EN BITNODES
        super_b.s_first_ino = p_inodo;
        super_b.s_first_blo = p_block;

        fseek(disco_base, inicio_particion, SEEK_SET);
        fwrite(&super_b, sizeof(Estructuras::Super_bloque), 1, disco_base);

        fseek(disco_base, super_b.s_bm_inode_start, SEEK_SET);
        fwrite(&bitinodes, sizeof(bitinodes), 1, disco_base);

        fseek(disco_base, super_b.s_bm_block_start, SEEK_SET);
        fwrite(&bitblocks, sizeof(bitblocks), 1, disco_base);

        if (super_b.s_filesystem_type == 3) {

            // ===== AGREGAR NUEVO JOURNAL
            Estructuras::Journal nuevo_jorunal;
            string operation = "mkdir";
            nuevo_jorunal.permiso = 664;
            nuevo_jorunal.name[0] = '0';
            nuevo_jorunal.tipo = '1';
            strcpy(nuevo_jorunal.tipo_operacion, operation.c_str());
            strcpy(nuevo_jorunal.contenido, nuevo_path.c_str());
            strcpy(nuevo_jorunal.propietario, user_g.user.c_str());
            strcpy(nuevo_jorunal.fecha, fecha_actual.c_str());
            //=========Agregando nuevo journal

            int journal_inicio = super_b.s_bm_inode_start - (sizeof(Estructuras::Journal) * 100);
            fseek(disco_base, journal_inicio, SEEK_SET);

            // BUSCAR SIGUIENTE JOURNAL
            int siguiente_journal = 0;
            for (int j = 0; j < 100; j++) {
                Estructuras::Journal aux_journal;
                fseek(disco_base, journal_inicio + (sizeof(Estructuras::Journal) * j),
                      SEEK_SET);
                if (aux_journal.tipo_operacion[0] == '-') {
                    siguiente_journal = j;
                    break;
                }
            }
            // GUARDAR
            fseek(disco_base, journal_inicio + (sizeof(Estructuras::Journal) * siguiente_journal),
                  SEEK_SET);
            fwrite(&nuevo_jorunal, sizeof(Estructuras::Journal), 1, disco_base);
        }
        fclose(disco_base);
    } else{
        cout <<RED "NO SE ENCOTRO EL DISCO! " << CYN""<< endl;
    }

}
//======================================= CREACION DE ARCHIVOS ==================================
void Comandos::mkfile(string contenido, Estructuras::Particiones_Montadas particion_mont, string path, bool c) {
     //======================= LEER SUPERBLOQUE
    Estructuras::Super_bloque super_b;
    int inicio_part = particion_mont.start;
    FILE *disco_base = fopen(particion_mont.path.c_str(), "rb+");

    //======================= EXISTE
    if (disco_base != NULL) {
        //======================= RUTA
        //======================= GLOBALES
        stringstream ss(path);
        string item;
        vector<string> path_split;
        while (getline(ss, item, '/')){
            path_split.push_back(item);
        }
        string fecha_actual = to_string(time(nullptr));
        int path_tam = path_split.size();
        path_split.at(0) = "/";

        string nombre_archivo = path_split.at(path_split.size() - 1).c_str();

        //======================= CREAR CARPETAS
        if (path_tam > 2 && c) {
            string dir_siguente;
            for (int i = 1; i < path_tam - 1; i++) {
                dir_siguente += "/" + path_split.at(i);
            }
            mkdir(dir_siguente, particion_mont, 1);
        }

        fseek(disco_base, inicio_part, SEEK_SET);
        fread(&super_b, sizeof(Estructuras::Super_bloque), 1, disco_base);

        //======================= BITMAP DE INODOS Y BLOQUES
        char bitinodes[super_b.s_inodes_count];
        char bitblocks[super_b.s_blocks_count];

        fseek(disco_base, super_b.s_bm_inode_start, SEEK_SET);
        fread(&bitinodes, sizeof(bitinodes), 1, disco_base);

        fseek(disco_base, super_b.s_bm_block_start, SEEK_SET);
        fread(&bitblocks, sizeof(bitblocks), 1, disco_base);

        //================ GLOBALES
        int indice_inodo = super_b.s_first_ino;
        int indice_bloque = 0;

        Estructuras::Inodo inodo_act;
        int inodo_inicio = super_b.s_inode_start;

        fseek(disco_base, inodo_inicio, SEEK_SET);
        fread(&inodo_act, sizeof(Estructuras::Inodo), 1, disco_base);

        //============== BUSCAR INODO
        for (int i = 0; i < path_tam; i++) {
            for (int j = 0; j < 15; j++) {
                if (inodo_act.i_block[j] != -1) {
                    Estructuras::Bloque_carpeta  directorio_bloque;
                    fseek(disco_base, inodo_act.i_block[j], SEEK_SET);
                    fread(&directorio_bloque, sizeof(Estructuras::Bloque_carpeta ), 1, disco_base);

                    // ===================BUSCAR CONTENIDO
                    for (int k = 0; k < 4; k++) {
                        if (directorio_bloque.b_content[k].b_inodo != -1) {
                            string nombre_blo(directorio_bloque.b_content[k].b_name);

                            if (nombre_blo == path_split.at(i)) {
                                Estructuras::Inodo aux_inodo;

                                fseek(disco_base, directorio_bloque.b_content[k].b_inodo,
                                      SEEK_SET);
                                fread(&aux_inodo, sizeof(Estructuras::Inodo), 1, disco_base);

                                // ES UN INODO DE CARPETA
                                if (aux_inodo.i_type == '1') {
                                    inodo_inicio = directorio_bloque.b_content[k].b_inodo;
                                    inodo_act = aux_inodo;
                                }
                            }
                        }
                    }
                }
            }
        }

        //======================= VERIFICAR PERMISOS

        if (!Verificar_permiso(inodo_act, 1)) {
            return;
        }

        bool archivo_i = 0;
        bool bandera_bloquenuevo = 0;
        bool bandera_bloquelibre = 0;
        int posicion_bloque[2] = {0, 1};

        //======================= BUSQUEDA EN APUNTADORES DIRECTOS
        for (int i = 0; i < 15; i++) {
            // BLOQUE OCUPADO
            if (inodo_act.i_block[i] != -1) {
                //======================= BLOQUE ACTUAL
                Estructuras::Bloque_carpeta bloque_ac;
                posicion_bloque[0] = i;
                fseek(disco_base, inodo_act.i_block[i], SEEK_SET);
                fread(&bloque_ac, sizeof(Estructuras::Bloque_carpeta), 1, disco_base);

                //======================= BUSCAR ESPACIO EN CONTENIDO DEL BLOQUE
                for (int j = 0; j < 4; j++) {
                    posicion_bloque[1] = j;
                    Estructuras::Content content = bloque_ac.b_content[j];

                    if (content.b_inodo == -1) {
                        bandera_bloquelibre = 1;
                        break;
                    } else {
                        string nobmre_contenido(content.b_name);
                        if (nobmre_contenido == nombre_archivo) {
                            bandera_bloquelibre = 1;
                            archivo_i = 1;
                            break;
                        }
                    }
                }

                // PRIMER BLOQUE LIBRE
                if (bandera_bloquelibre) {
                    break;
                }
            }
                // SINO CREAR UN NUEVO BLOQUE
            else {
                posicion_bloque[0] = i;
                posicion_bloque[1] = 0;
                bandera_bloquelibre = 1;
                bandera_bloquenuevo = 1;
                break;
            }
        }
        // SE ENCONTRO UN BLOQUE DISPONIBLE
        if (bandera_bloquelibre) {
            // CREAR UN CONTENIDO
            Estructuras::Content contenido_nuevo_bloque;
            strcpy(contenido_nuevo_bloque.b_name, nombre_archivo.c_str());

            // CREAR UN NUEVO INODO
            Estructuras::Inodo nuevo_inodo_;
            strcpy(nuevo_inodo_.i_atime, fecha_actual.c_str());
            strcpy(nuevo_inodo_.i_ctime, fecha_actual.c_str());
            strcpy(nuevo_inodo_.i_mtime, fecha_actual.c_str());
            nuevo_inodo_.i_type = '0';
            nuevo_inodo_.i_size = contenido.length();
            nuevo_inodo_.i_perm = 664;
            nuevo_inodo_.i_uid = (int)stoi(user_g.uid);
            nuevo_inodo_.i_gid = (int)stoi(user_g.gid);
            for (int j = 0; j < 15; j++) {
                nuevo_inodo_.i_block[j] = -1;
            }

            // NUEVO BLOQUE
            if (bandera_bloquenuevo) {
                // OCUPAR UN NUEVO BLOQUE
                super_b.s_free_blocks_count = super_b.s_free_blocks_count - 1;
                indice_bloque = super_b.s_first_blo;
                bitblocks[indice_bloque] = '1';
            }

            //======================= DISTRIBUIR CONTENIDO EN NUEVO INODO
            int tam_contenido = contenido.length();
            float tam_ = (float)tam_contenido / (float)(sizeof(Estructuras::Bloque_archivos ) - 1);
            int contador_bloque = ceil(tam_);

            //======================= LIMPIAR BLOQUES ANTERIORES Y RESCRIBIR
            if (archivo_i) {
                //======================= BLOQUE ANTERIOR
                Estructuras::Bloque_carpeta  bloque_ant;
                fseek(disco_base, inodo_act.i_block[posicion_bloque[0]], SEEK_SET);
                fread(&bloque_ant, sizeof(Estructuras::Bloque_carpeta), 1, disco_base);

                //======================= INODO ANTERIOR
                Estructuras::Inodo nodo_ante;
                int prev_inode_start = bloque_ant.b_content[posicion_bloque[1]].b_inodo;
                indice_inodo =(int)(((super_b.s_inode_start - prev_inode_start) * -1) /sizeof(Estructuras::Inodo));

                //=======================  CAMBIAR FECHA DE MODIFICACION
                strcpy(nodo_ante.i_atime, fecha_actual.c_str());
                strcpy(nodo_ante.i_mtime, fecha_actual.c_str());

                //======================= GUARDAR CAMBIOS
                fseek(disco_base, prev_inode_start, SEEK_SET);
                fread(&nodo_ante, sizeof(Estructuras::Inodo), 1, disco_base);

                //======================= BORRAR INODO
                for (int i = 0; i < 15; i++) {
                    if (nodo_ante.i_block[i] != -1) {
                        int bloque_add = (int)(((super_b.s_block_start -nodo_ante.i_block[i]) * -1) /sizeof(Estructuras::Bloque_archivos));
                        bitblocks[bloque_add] = '0';
                        super_b.s_free_blocks_count = super_b.s_free_blocks_count + 1;
                    }
                }
            }
            //======================= CORTAR BLOQUES ANTES
            string contenido_siguiente = contenido;
            char content_block[contador_bloque][sizeof(Estructuras::Bloque_archivos)];

            for (int i = 0; i < contador_bloque; i++) {
                //======================= DIVIDIR BLOQUES EN 63 BYTES
                int tam_max =(sizeof(Estructuras::Bloque_archivos) - 1) * (i + 1) < contenido_siguiente.size()? (sizeof(Estructuras::Bloque_archivos) - 1) * (i + 1) : contenido_siguiente.size();
                int inicial = (sizeof(Estructuras::Bloque_archivos) - 1) * i;
                string aux_blo = contenido_siguiente.substr(inicial, tam_max);
                //======================= AGREGAR
                snprintf(content_block[i], sizeof(Estructuras::Bloque_archivos), "%s",aux_blo.c_str());
            }

            //=======================RECORRER BLOQUES
            for (int i = 0; i < contador_bloque;i++) {
                Estructuras::Bloque_archivos bloque_arch;
                snprintf(bloque_arch.content, sizeof(Estructuras::Bloque_archivos), "%s",content_block[i]);

                //======================= BUSCAR NUEVO BLOQUE LIBRE Y OCUPARLO
                int bloque_lib = 0;
                for (int j = 0;j < super_b.s_blocks_count; j++) {
                    if (bitblocks[j] == '0') {
                        bitblocks[j] = '1';
                        super_b.s_free_blocks_count = super_b.s_free_blocks_count - 1;
                        bloque_lib = j;
                        break;
                    }
                }
                //======================= ACTUALIZAR INODO
                int sig_ =super_b.s_block_start + (sizeof(Estructuras::Bloque_archivos) * bloque_lib);
                nuevo_inodo_.i_block[i] = sig_;
                //======================= GAURDAR NUEVO BLOQUE
                fseek(disco_base, sig_, SEEK_SET);
                fwrite(&bloque_arch, sizeof(Estructuras::Bloque_archivos), 1, disco_base);
            }

            //======================= GUARDAR NUEVO INODO SI NO ES EL MISMO
            bitinodes[indice_inodo] = '1';
            contenido_nuevo_bloque.b_inodo = super_b.s_inode_start + (sizeof(Estructuras::Inodo) * indice_inodo);

            fseek(disco_base, contenido_nuevo_bloque.b_inodo, SEEK_SET);
            fwrite(&nuevo_inodo_, sizeof(Estructuras::Inodo), 1, disco_base);

            Estructuras::Bloque_carpeta  bloque_ac;
            if (!bandera_bloquenuevo) {
                fseek(disco_base, inodo_act.i_block[posicion_bloque[0]], SEEK_SET);
                fread(&bloque_ac, sizeof(Estructuras::Bloque_carpeta), 1, disco_base);
                indice_bloque = (int)(((super_b.s_block_start -inodo_act.i_block[posicion_bloque[0]]) *-1) /sizeof(Estructuras::Bloque_carpeta));

            } else {
                for (int content_index = 0; content_index < 4; content_index++) {
                    bloque_ac.b_content[content_index] = (Estructuras::Content){{}, -1};
                }
            }
            bloque_ac.b_content[posicion_bloque[1]] = contenido_nuevo_bloque;
            inodo_act.i_block[posicion_bloque[0]] = super_b.s_block_start + (sizeof(Estructuras::Bloque_archivos) * indice_bloque);
            //======================= GUARDAR BLOQUE DE INODO PADRE
            fseek(disco_base, inodo_act.i_block[posicion_bloque[0]], SEEK_SET);
            fwrite(&bloque_ac, sizeof(Estructuras::Bloque_archivos), 1, disco_base);

            //======================= CAMBIOS EN INODO PADRE
            strcpy(inodo_act.i_atime, fecha_actual.c_str());
            strcpy(inodo_act.i_mtime, fecha_actual.c_str());

            fseek(disco_base, inodo_inicio, SEEK_SET);
            fwrite(&inodo_act, sizeof(Estructuras::Inodo), 1, disco_base);
        }

        //======================= ASIGNAR A SUPERBLOQUE
        int p_inodo = 0;
        int p_bloque = 0;
        for (int i = 0; i < sizeof(bitinodes); i++) {
            if (bitinodes[i] == '0') {
                p_inodo = i;
                break;
            }
        }

        for (int i = 0; i < sizeof(bitblocks); i++) {
            if (bitblocks[i] == '0') {
                p_bloque = i;
                break;
            }
        }

        //======================= GUARDAR CAMBIOS EN SUPERBLOQUE Y EN BITNODES
        super_b.s_first_ino = p_inodo;
        super_b.s_first_blo = p_bloque;

        fseek(disco_base, inicio_part, SEEK_SET);
        fwrite(&super_b, sizeof(Estructuras::Super_bloque), 1, disco_base);

        fseek(disco_base, super_b.s_bm_inode_start, SEEK_SET);
        fwrite(&bitinodes, sizeof(bitinodes), 1, disco_base);

        fseek(disco_base, super_b.s_bm_block_start, SEEK_SET);
        fwrite(&bitblocks, sizeof(bitblocks), 1, disco_base);

        if (super_b.s_filesystem_type == 3) {
            //======================= AGREGAR NUEVO JOURNAL
            Estructuras::Journal nuev_journal;
            nuev_journal.permiso = 664;
            nuev_journal.name[0] = '0';
            nuev_journal.tipo = '1';
            strcpy(nuev_journal.tipo_operacion, "touch");
            strcpy(nuev_journal.contenido, contenido.c_str());
            strcpy(nuev_journal.propietario, user_g.user.c_str());
            strcpy(nuev_journal.fecha, fecha_actual.c_str());

            int inicio_journal = super_b.s_bm_inode_start - (sizeof(Estructuras::Journal) * 100);
            fseek(disco_base, inicio_journal, SEEK_SET);

            // BUSCAR SIGUIENTE JOURNAL
            int journal_siguiente = 0;
            for (int i = 0; i < 100; i++) {
                Estructuras::Journal aux_journal;
                fseek(disco_base, inicio_journal + (sizeof(Estructuras::Journal) * i),
                      SEEK_SET);
                if (aux_journal.tipo_operacion[0] == '-') {
                    journal_siguiente = i;
                    break;
                }
            }
            //=================== GUARDAR
            fseek(disco_base, inicio_journal + (sizeof(Estructuras::Journal) * journal_siguiente),
                  SEEK_SET);
            fwrite(&nuev_journal, sizeof(Estructuras::Journal), 1, disco_base);
        }

        fclose(disco_base);
    } else
        cout <<RED " NO SE ENCONTRO EL DISCO " << CYN""<< endl;
}
//======================================== VERIFICANDO PERMISOS ================================
bool Comandos::Verificar_permiso(Estructuras::Inodo inodo, int per) {
    // VERIFICAR PERMISOS
    string perms = to_string(inodo.i_perm);
    string user_ = bitset<3>((int)(perms.at(0) - '0')).to_string();
    string grp = bitset<3>((int)(perms.at(1) - '0')).to_string();
    string otros = bitset<3>((int)(perms.at(2) - '0')).to_string();

    //=================0  USUARIO PROPIETARIO
    bool bandera = 0;
    if (user_g.group == "root" ||((user_.at(per) == '1' && (inodo.i_uid == (int)stoi(user_g.uid))) ||otros.at(per) == '1' ||
         (grp.at(per) == '1' && (inodo.i_gid == (int)stoi(user_g.gid))))) {
        bandera = 1;
    }

    if (!bandera) {
        string msg;
        if (per == 0){
            cout <<RED "EL USUARIO NO TIENE PERMISOS DE LECTURA " << CYN""<< endl;
        }
        else if (per == 1){
            cout <<RED "EL USUARIO NO TIENE PERMISOS DE SCRITURA! " << CYN""<< endl;
        }
        else if (per == 2){
            cout <<RED "EL USUARIO NO TIENE PERMISOS DE EJECUCION " << CYN""<< endl;
        }

    }

    return bandera;
}

//========================================= COMANDO LOGIN =======================================
/*
void Comandos::login(string id, string pwd, string user) {
    // VERIFICAR SI EXISTE UNA SESION INICIADA
    if(user_g.logged==false){
        // STREAM
        bool login = 0;
        bool deleted_user = 0;

        // BUSCAR BLOQUE DE USUARIOS
        stringstream ss(obtener_usuarios(id));
        string item;
        vector<string> user_lines;
        while (getline(ss, item, '\n')){
            user_lines.push_back(item);
        }
        //vector<string> user_lines = get_users(id);

        // LEER
        for (int line_index = 0; line_index < user_lines.size();
             line_index++) {
            UserOrGroup user_info =
                    get_user_line_info(user_lines.at(line_index));

            // LOGIN
            if (user_info.type == "U") {
                if (user_info.name == props.user && user_info.pwd == props.pwd) {
                    if (user_info.uid != "0") {
                        global_user.id = props.id;
                        global_user.user = user_info.name;
                        global_user.logged = 1;
                        global_user.pwd = user_info.pwd;
                        global_user.grp = user_info.grp;
                        global_user.uid = user_info.uid;
                        global_user.gid = user_info.uid;
                    } else
                        deleted_user = 1;

                    break;
                }
            }
        }

        // BUSCAR GRUPO
        for (int line_index = 0; line_index < user_lines.size();
             line_index++) {
            UserOrGroup user_info =
                    get_user_line_info(user_lines.at(line_index));

            if (user_info.type == "G") {
                if (user_info.grp == global_user.grp) {
                    if (user_info.uid != "0") {
                        global_user.gid = user_info.uid;
                    } else
                        deleted_user = 1;

                    break;
                }
            }
        }

        // NO SE ENCONTRO NINGUN USUARIO
        if (!deleted_user) {
            if (!global_user.logged)
                print_err("USERS_ERR", "No se encontro ningun usuario \"" +
                                       props.user + "\".");
        } else
            print_err("USER_ERR", "El usuario esta eliminado.");


    }
    else{
        cout <<RED " EXISTE UNA SESION INICIADA, ES NECESARIO CERRAR SESIÓN " << CYN""<< endl;
    }





}*/

//================================== FUNCIONES AUXILIARES==========================
string Comandos::obtener_usuarios(string id) {
    // PARTICION
    // BUSCAR PARTICION MONTADA
    bool bandera=false;
    Estructuras::Particiones_Montadas particion_montada;
    for (int i = 0; i <Lista_part_montadas.size();i++) {
        if (Lista_part_montadas.at(i).id == id) {
            particion_montada = Lista_part_montadas.at(i);
            bandera = true;
        }
    }
    string users_content;

    if (true) {
        // STREAM
        bool login = 0;
        bool deleted_user = 0;
        FILE *users_file = fopen(particion_montada.path.c_str(), "rb+");

        if (users_file != NULL) {
            // BUSCAR BLOQUE DE USUARIOS
            Estructuras::Super_bloque superblock;
            fseek(users_file, particion_montada.start, SEEK_SET);
            fread(&superblock, sizeof(Estructuras::Super_bloque ), 1, users_file);

            Estructuras::Inodo first_inode;
            fseek(users_file, superblock.s_inode_start, SEEK_SET);
            fread(&first_inode, sizeof(Estructuras::Inodo), 1, users_file);

            Estructuras::Bloque_carpeta root_block;
            fseek(users_file, first_inode.i_block[0], SEEK_SET);
            fread(&root_block, sizeof(Estructuras::Bloque_carpeta), 1, users_file);

            Estructuras::Inodo users_inode;
            fseek(users_file, root_block.b_content[0].b_inodo, SEEK_SET);
            fread(&users_inode, sizeof(Estructuras::Inodo), 1, users_file);

            string users_text;
            for (int block_index = 0; block_index < 15; block_index++) {
                if (users_inode.i_block[block_index] != -1) {
                    Estructuras::Bloque_archivos  users_block;
                    fseek(users_file, users_inode.i_block[block_index], SEEK_SET);
                    fread(&users_block, sizeof(Estructuras::Bloque_archivos), 1, users_file);

                    // CONTENT
                    string file_content = users_block.content;
                    users_text += file_content;
                }
            }

            users_content = users_text;
            fclose(users_file);
        }
    } else {
        cout<<RED"PARTICION NO MONTADA  "<<YLLW ""<< id<<CYN""<<endl;
    }

    return users_content;
}