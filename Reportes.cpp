//
// Created by pilo-tuy on 14/12/21.
//

#include "Reportes.h"
#include "MKdisco.h"
#include "RMdisco.h"
#include "Globales.h"
#include "Estructuras.h"
#include <iostream>
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
#define YLLW "\e[0;33m"
#include <math.h>
using namespace std;

Reportes::Reportes() {
}

//======================================================== REPORTE MBR =======================================
void Reportes::Reporte_mbr(string id, string path){

    Estructuras::Particiones_Montadas particion_reporte;
    bool existe = false;
    for(int i=0;i< Lista_part_montadas.size();i++){
            if(Lista_part_montadas.at(i).id == id){
                particion_reporte =Lista_part_montadas.at(i);
                existe = true;
            }
    }
    if(existe){
        Estructuras::MBRdisco  mbr_reporte;
        FILE *disco_base = fopen(particion_reporte.path.c_str(), "rb+");

        // EXISTE
        if (disco_base != NULL) {
            string dot_p = "/home/archivos/reportes/";
            string nombre_dot = dot_p + "mbr.dot";

            // OBTENER MBR
            fseek(disco_base, 0, SEEK_SET);
            fread(&mbr_reporte, sizeof(Estructuras::MBRdisco), 1, disco_base);
            FILE *archivo_dot = fopen(nombre_dot.c_str(), "w+");
            if (archivo_dot != NULL) {
                string dot = "digraph G {\n"
                             "graph[rankdir=LR, overlap=false,splines=true];\n"
                             "node[shape = record, fontsize = 9, fontname = Verdana];\n"
                             "MBR [shape=none, margin=0, \n"
                             "label=<\n"
                             "<table>\n"
                             "<tr>\n"
                             "<td>Nombre</td> <td>Valor</td></tr> \n"
                             "<tr><td>MBR Tamaño</td><td> " + to_string(mbr_reporte.mbr_tamano) +" </td></tr>  \n"
                             "<tr><td>MBR fecha creacion </td><td> " + mbr_reporte.mbr_fecha_creacion +"</td></tr>  \n"
                             "<tr><td>MBR_Signature</td><td> " + to_string(mbr_reporte.disk_signature) + "</td></tr> \n" +
                             "<tr><td>Disk Fit</td><td>"+mbr_reporte.disk_fit+"</td></tr>\n";

                // PARTICIONES
                for (int i = 0; i < 4; i++) {
                    Estructuras::Particion  part = mbr_reporte.disco_particion[i];
                    string id = part.part_type == 'e' ? "EXT" : "Part" + to_string(i);
                    string s="Status";
                    //string nm(16, part.part_name);
                    //string ax(1, part.part_status);
                    dot += "<tr><td BGCOLOR=\"gray\"></td> <td BGCOLOR=\"gray\"></td> </tr>\n"
                           "<tr><td>" + s + "</td><td>" + part.part_status + " </td></tr> \n" +
                           "<tr><td>Type</td><td> " + part.part_type + "</td></tr> \n " +
                           "<tr><td>Fit</td><td> " + part.part_fit + "</td></tr> \n" +
                           "<tr><td>Start</td><td> " + to_string(part.part_start) + " </td></tr>\n" +
                           "<tr><td>Size</td><td> " + to_string(part.part_size) + "</td></tr>\n" +
                           "<tr><td>Name</td><td> " + part.part_name + "</td></tr>\n";
                }
                dot +="</table>>];\n";
                dot += Obtner_ebr_dot(mbr_reporte, disco_base);
                dot += "}";

                fwrite(dot.c_str(), dot.length(), 1, archivo_dot);
                fclose(archivo_dot);

                // GENERAR IMAGEN

                //Creacion de la carpeta
                string comando2 = "rmdir \"" + path + "\"";
                string crear = "mkdir -p \"" + path + "\"" ;
                system(crear.c_str());
                system(comando2.c_str());

                string svg_r = "dot -Tsvg " + nombre_dot + " -o " + path;
                system(svg_r.c_str());
            }
            fclose(disco_base);
        }
        else{
            cout <<RED""<<"REPORTE NO GENERADO!,DISCO NO EXISTE! :  " <<YLLW<<id<<CYN""<<endl;
        }
    }else{
        cout <<RED""<<"REPORTE NO GENERADO!, NO SE ENCONTRO:  " <<YLLW<<id<<CYN""<<endl;
    }
}

//====================================== OBTENER GRAFICA DE EBR  PARA EL REPORTE MBR =======================
string Reportes::Obtner_ebr_dot(Estructuras::MBRdisco  mbr_rep, FILE *disco_b) {
    string contenido_dot;
    int inicio_extendida = -1;
    if (disco_b != NULL) {
        //======================  BUSCAR EXTENDIDA
        for (int i = 0; i < 4; i++) {
            if (mbr_rep.disco_particion[i].part_status != '0')
                if (mbr_rep.disco_particion[i].part_type == 'e') {
                    inicio_extendida = mbr_rep.disco_particion[i].part_start;
                    break;
                }
        }

        // POSICIONES DE EBRS
        if (inicio_extendida != -1) {
            int pos[23] = {-1};
            for (int i = 0; i < 23; i++)
                pos[i] = -1;
            pos[0] = inicio_extendida;

            for (int e = 0; e < 23 - 1; e++) {
                if (pos[e] != -1) {
                    // BUSCAR EBR
                    Estructuras::EBRparticion data_e;
                    fseek(disco_b, pos[e], SEEK_SET);
                    fread(&data_e, sizeof(Estructuras::EBRparticion), 1, disco_b);
                    string ebr_name(data_e.part_name);

                    if (data_e.part_status == '1') {
                        if (e > 0 &&
                            ebr_name.find("Footer_") == std::string::npos) {

                            contenido_dot +=
                                    "EBR" + to_string(e) +
                                    " [shape=none, margin=0, label=<<table "
                                    "border=\"0\" "
                                    "cellborder=\"1\" cellspacing=\"0\" "
                                    "cellpadding=\"4\"><tr><td>Nombre</td><td>Valor</td></"
                                    "tr><tr><td>Status</td><td>" +
                                    data_e.part_status + "</td></tr><tr><td>Fit</td><td>" +
                                    data_e.part_fit + "</td></tr><tr><td>Start</td><td>" +
                                    to_string(data_e.part_start) +
                                    "</td></tr><tr><td>Size</td><td>" + to_string(data_e.part_size) +
                                    "</td></tr><tr><td>Next</td><td>" + to_string(data_e.part_next) +
                                    "</td></tr><tr><td>Name</td><td>" + data_e.part_name +
                                    "</td></tr></table>>]";

                            if (e == 1) {
                                contenido_dot += "EXT -> EBR1;";
                            } else {
                                contenido_dot += "EBR" + to_string(e - 1) + " -> EBR" +
                                                 to_string(e);
                            }
                        }
                        pos[e + 1] = data_e.part_next;
                    }
                } else
                    break;
            }
        }
    }

    return contenido_dot;
}

//====================================== OBTENER GRAFICA DE EBR  PARA EL REPORTE MBR =======================
void Reportes::Reporte_disk(string id, string path) {
    Estructuras::Particiones_Montadas particion_reporte;
    bool existe = false;
    for(int i=0;i< Lista_part_montadas.size();i++){
        if(Lista_part_montadas.at(i).id == id){
            particion_reporte =Lista_part_montadas.at(i);
            existe = true;
        }
    }
    if(existe){
        Estructuras::MBRdisco mbr_data;
        FILE *disco_base = fopen(particion_reporte.path.c_str(), "rb+");
        if (disco_base != NULL) {
            string path_d = "/home/archivos/reportes/";
            string dot_nombre = path_d + "disk.dot";
            fseek(disco_base, 0, SEEK_SET);
            fread(&mbr_data, sizeof(Estructuras::MBRdisco), 1, disco_base);
            FILE *archivo_dot = fopen(dot_nombre.c_str(), "w+");

            if (archivo_dot != NULL) {
                // GENERAR DOT
                string grafica = "digraph G {graph[rankdir=LR, overlap=false, splines=true];\n"
                             "    node[shape = record, fontsize = 9, fontname= Verdana]; \n"
                             "    DISK [shape=none, margin=0, label=<<table border='0' cellborder='1' cellpadding='10' cellspacing='0' >\n"
                             " <tr >\n"
                             " <td rowspan='2'> \n"
                             "MBR <br/> inicio:0  <br/>fin:" + to_string(sizeof(Estructuras::MBRdisco)) + "\n"
                             "</td>";

                //========================  ESPACIOS ENTRE PARTICIONES
                int inicio_libre = 161;
                int fin_libre = 161;
                for (int part_index = 0; part_index < 4; part_index++) {

                    fin_libre = mbr_data.disco_particion[part_index].part_start;

                    if ((fin_libre - inicio_libre) > 0) {
                        float free_per =(((fin_libre - inicio_libre) / mbr_data.mbr_tamano) * 100);
                        grafica += "<td rowspan='2'> Libre  <br/> inicio: " + to_string(inicio_libre) + " <br/>fin: " + to_string(fin_libre ) + " <br/>Porcentaje: " + to_string(free_per) + " % </td>";
                    }

                    if (mbr_data.disco_particion[part_index].part_status == '1') {
                        float part_per =(mbr_data.disco_particion[part_index].part_size / mbr_data.mbr_tamano) *100;
                        //string pType(1, mbr_data.disco_particion[part_index].part_type);
                        if (mbr_data.disco_particion[part_index].part_type != 'e') {
                            string name = mbr_data.disco_particion[part_index].part_name;
                            int inicio = mbr_data.disco_particion[part_index].part_start;
                            int fin = mbr_data.disco_particion[part_index].part_start + mbr_data.disco_particion[part_index].part_size;
                            grafica += "<td rowspan='2' > \n"
                                  "Primaria <br/> Nombre: " + name + "  <br/>inicio: " + to_string(inicio) + " <br/>fin: " + to_string(fin) +
                                       "<br/>Porcentaje: " + to_string(part_per) +" \n"
                                  "</td>";
                        } else {
                            string name = mbr_data.disco_particion[part_index].part_name;
                            grafica += "<td><table><tr><td>Particion "
                                   "Extendida</td></tr><tr>" +
                                       get_tablaebr(mbr_data, disco_base,mbr_data.disco_particion[part_index].part_size) +
                                       "</tr></table></td>";
                            grafica += "<td> \n"
                                  "<table border='0' cellborder='1' cellpadding='10' cellspacing='0'>\n"
                                  "<tr><td colspan=\"23\">Extendida: " + name +"</td></tr>\n"
                                  "<tr>" + get_tablaebr(mbr_data, disco_base,mbr_data.disco_particion[part_index].part_size) +
                                  "</tr>\n"
                                  "</table>\n"
                                  "</td>";
                        }

                    } else {
                        grafica += "<td rowspan='2' > \n"
                               "Libre \n"
                               "</td>";
                    }

                    inicio_libre =
                            fin_libre + mbr_data.disco_particion[part_index].part_size + 1;
                }

                grafica += "</tr></table>>];}";

                fwrite(grafica.c_str(), grafica.length(), 1, archivo_dot);
                fclose(archivo_dot);

                // GENERAR IMAGEN
                string dot_svg = "dot -Tsvg " + dot_nombre + " -o " + path;
                system(dot_svg.c_str());
            }

            // CERRAR
            fclose(disco_base);
        }
        else{
            cout <<RED""<<"REPORTE NO GENERADO!,DISCO NO EXISTE! :  " <<YLLW<<id<<CYN""<<endl;
        }

    }else{
        cout <<RED""<<"REPORTE NO GENERADO!, NO SE ENCONTRO:  " <<YLLW<<id<<CYN""<<endl;
    }

}

string Reportes::get_tablaebr(Estructuras::MBRdisco mbr_data, FILE *disco_base, int ext_size) {
    string contenido_grafica;
    int inicio_extendida = -1;
    // EXISTE
    if (disco_base != NULL) {
        //===============================  BUSCAR EXTENDIDA
        for (int i = 0; i < 4; i++) {
            if (mbr_data.disco_particion[i].part_status != '0')
                if (mbr_data.disco_particion[i].part_type == 'e') {
                    inicio_extendida = mbr_data.disco_particion[i].part_start;
                    break;
                }
        }

        // POSICIONES DE EBRS
        if (inicio_extendida != -1) {
            int pos[23] = {-1};
            for (int i = 0; i < 23; i++){
                pos[i] = -1;
            }
            pos[0] = inicio_extendida;

            for (int e = 0; e < 23 - 1; e++) {
                if (pos[e] != -1) {
                    // BUSCAR EBR
                    Estructuras::EBRparticion  ebr_;
                    fseek(disco_base, pos[e], SEEK_SET);
                    fread(&ebr_, sizeof(Estructuras::EBRparticion), 1, disco_base);
                    string ebr_nombre(ebr_.part_name);

                    if (ebr_.part_status == '1') {
                        if (e > 0 &&
                            ebr_nombre.find("F_") == std::string::npos) {
                            float ebr_per = ((float)ebr_.part_size / ext_size) * 100;
                            int inicio = ebr_.part_start;
                            int fin= ebr_.part_start + ebr_.part_size;
                            contenido_grafica += "<td>EBR <br/> Inicio:" + to_string(inicio - sizeof (Estructuras::EBRparticion)) +
                                                 " <br/> Fin: " + to_string(inicio) +
                                                 " </td>" +
                                                 "<td>Logica <br/> Nombre: " + ebr_nombre +
                                                 " <br/> Inicio: " + to_string(inicio) +
                                                 " <br/> Fin: " + to_string(fin) +
                                                 " <br/> Prcentaje: " + to_string(ebr_per) + " %</td>";
                        }
                        pos[e + 1] = ebr_.part_next;
                    }
                } else
                    break;
            }
        }
    }
    return contenido_grafica;
}
//====================================== OBTENER GRAFICA DE EBR  PARA EL REPORTE TREE =======================
void Reportes::Reporte_tree(string id, string path,string name){
    Estructuras::Particiones_Montadas particion_reporte;
    bool existe = false;
    for(int i=0;i< Lista_part_montadas.size();i++){
        if(Lista_part_montadas.at(i).id == id){
            particion_reporte =Lista_part_montadas.at(i);
            existe = true;
        }
    }
    if(existe){
        // LEER SUPERBLOQUE
        Estructuras::Super_bloque  super_bloque_reporte;
        int partition_start =particion_reporte.start;
        FILE *disco_base_r = fopen(particion_reporte.path.c_str(), "rb+");


        if (disco_base_r != NULL) {
            fseek(disco_base_r, partition_start, SEEK_SET);
            fread(&super_bloque_reporte, sizeof(Estructuras::Super_bloque), 1, disco_base_r);
            string dot_name = "/home/archivos/reportes/tree.dot";
            FILE *archivo_dot = fopen(dot_name.c_str(), "w+");
            if (archivo_dot != NULL) {
                // GENERAR DOT
                string grafica = "digraph G {graph[rankdir=LR, overlap=false, "
                             "splines=true];node[shape = record, fontsize = 9, fontname "
                             "= \"Verdana\"];";

                // BITMAP DE INODOS Y BLOQUES
                char bitinodes[super_bloque_reporte.s_inodes_count];
                char bitblocks[super_bloque_reporte.s_blocks_count];

                fseek(disco_base_r, super_bloque_reporte.s_bm_inode_start, SEEK_SET);
                fread(bitinodes, super_bloque_reporte.s_inodes_count, 1, disco_base_r);

                fseek(disco_base_r, super_bloque_reporte.s_bm_block_start, SEEK_SET);
                fread(bitblocks, super_bloque_reporte.s_blocks_count, 1, disco_base_r);

                vector<string> componentes_p;
                stringstream ss(name);
                string item;
                while (getline(ss, item, '/')){
                    componentes_p.push_back(item);
                }
                int path_cmp_size = componentes_p.size();
                componentes_p.at(0) = "/";

                Estructuras::Inodo inodo_padre;
                int Inicio_Ino = super_bloque_reporte.s_inode_start;

                fseek(disco_base_r, Inicio_Ino, SEEK_SET);
                fread(&inodo_padre, sizeof(Estructuras::Inodo), 1, disco_base_r);

                //======================== BUSCAR INODO
                for (int i = 0; i < path_cmp_size; i++) {
                    for (int j = 0; j < 15; j++) {
                        if (inodo_padre.i_block[j] != -1) {
                            Estructuras::Bloque_carpeta  bloque_carp;
                            fseek(disco_base_r, inodo_padre.i_block[j], SEEK_SET);
                            fread(&bloque_carp, sizeof(Estructuras::Bloque_carpeta), 1, disco_base_r);

                            //======================= BUSCAR CONTENIDO
                            for (int k = 0; k < 4; k++) {
                                if (bloque_carp.b_content[k].b_inodo != -1) {
                                    string nombre_bloque(bloque_carp.b_content[k].b_name);

                                    if (nombre_bloque == componentes_p.at(i)) {
                                        Estructuras::Inodo aux_inodo;

                                        fseek(disco_base_r, bloque_carp.b_content[k].b_inodo,
                                              SEEK_SET);
                                        fread(&aux_inodo, sizeof(Estructuras::Inodo), 1, disco_base_r);

                                        // ES UN INODO DE CARPETA
                                        if (aux_inodo.i_type == '1') {
                                            Inicio_Ino = bloque_carp.b_content[k].b_inodo;
                                            inodo_padre = aux_inodo;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // RECORRER
                deque<Estructuras::Inodo> _inodos;
                deque<int> inicio_inodos;
                _inodos.push_back(inodo_padre);
                inicio_inodos.push_back(Inicio_Ino);

                while (_inodos.size() > 0) {
                    Estructuras::Inodo actual_inodo = _inodos.at(0);
                    Inicio_Ino = inicio_inodos.at(0);
                    _inodos.pop_front();
                    inicio_inodos.pop_front();

                    string atime(actual_inodo.i_atime);
                    string ctime(actual_inodo.i_ctime);
                    string mtime(actual_inodo.i_mtime);

                    string id_ino = "INODO" + to_string(Inicio_Ino);
                    int indice_inodo =(int)(((super_bloque_reporte.s_inode_start - Inicio_Ino) * -1) /sizeof(Estructuras::Inodo));

                    grafica +=
                            id_ino +
                            " [shape=none, margin=0, label=<<table "
                            "border=\"0\" "
                            "cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">" +
                            "<tr><td bgcolor=\"#03A9F4\"><font color=\"white\"><b>INode" +
                            to_string(indice_inodo) + "</b></font></td></tr>" +
                            "<tr><td bgcolor=\"#b3e5fc\">Append time: " + atime + "</td></tr>" +
                            "<tr><td bgcolor=\"#e1f5fe\">Creation time: " + ctime +
                            "</td></tr>" + "<tr><td bgcolor=\"#b3e5fc\">Modify time: " + mtime +
                            "</td></tr>" + "<tr><td bgcolor=\"#e1f5fe\">Type: " +
                            (actual_inodo.i_type == '1' ? "Directory" : "File") + "</td></tr>" +
                            "<tr><td bgcolor=\"#b3e5fc\">Size: " +
                            to_string(actual_inodo.i_size) + "</td></tr>" +
                            "<tr><td bgcolor=\"#e1f5fe\">Permissions: " +
                            to_string(actual_inodo.i_perm) + "</td></tr>" +
                            "<tr><td bgcolor=\"#b3e5fc\">UID: " + to_string(actual_inodo.i_uid) +
                            "</td></tr>" +
                            "<tr><td bgcolor=\"#e1f5fe\">GID: " + to_string(actual_inodo.i_gid) +
                            "</td></tr>";

                    for (int i = 0; i < 15; i++) {
                        if (actual_inodo.i_block[i] != -1) {
                            string color = i % 2 == 0 ? "b3e5fc" : "e1f5fe";
                            int indice_sb = (int)(((super_bloque_reporte.s_block_start - actual_inodo.i_block[i]) *-1) /sizeof(Estructuras::Bloque_archivos));
                            grafica += "<tr><td bgcolor=\"#" + color + "\">Block " +
                                       to_string(i + 1) + ": #" +
                                       to_string(indice_sb) + "</td></tr>";
                        }
                    }
                    grafica += "</table>>];";
                    for (int j = 0; j < 15; j++) {
                        if (actual_inodo.i_block[j] != -1) {
                            int sup_block_index = (int)(((super_bloque_reporte.s_block_start -actual_inodo.i_block[j]) * -1) / sizeof(Estructuras::Bloque_archivos));

                            if (actual_inodo.i_type == '1') {
                                Estructuras::Bloque_carpeta bloque_d;
                                fseek(disco_base_r, actual_inodo.i_block[j], SEEK_SET);
                                fread(&bloque_d, sizeof(Estructuras::Bloque_carpeta), 1, disco_base_r);
                                string block_id = id_ino + "_bloque_" + to_string(j);
                                grafica += block_id +
                                       " [shape=none, margin=0, label=<<table border=\"0\" "
                                       "cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">" +
                                       "<tr><td bgcolor=\"#ff6f00\"><font "
                                       "color=\"white\"><b>" +
                                           "Block " + to_string(sup_block_index) +
                                           "</b></font></td></tr></table>>];";
                                grafica += id_ino + " -> " + block_id + ";";

                                //=========================  BUSCAR CONTENIDO
                                for (int k = 0; k < 4; k++) {
                                    if (bloque_d.b_content[k].b_inodo != -1) {
                                        string nombre_b(bloque_d.b_content[k].b_name);
                                        string content_id =block_id + "_content_" + to_string(k);
                                        int siguiente_inodo = bloque_d.b_content[k].b_inodo;
                                        int indice_siguiente =(int)(((super_bloque_reporte.s_inode_start - siguiente_inodo) * -1) /sizeof(Estructuras::Inodo));

                                        grafica += content_id +
                                               " [shape=none, margin=0, label=<<table border=\"0\" "
                                               "cellborder=\"1\" cellspacing=\"0\" "
                                               "cellpadding=\"4\">" +
                                               "<tr><td bgcolor=\"#4caf50\"><font "
                                               "color=\"white\"><b>Directory "
                                               "Content</b></font></td></tr>" +
                                                   "<tr><td bgcolor=\"#a5d6a7\">Name: " + nombre_b +
                                                   "</td></tr>" + "<tr><td bgcolor=\"#c8e6c9\">INode: " +
                                                   to_string(indice_siguiente) + "</td></tr>" +
                                                   "</table>>];";
                                        grafica += block_id + " -> " + content_id + " [label=\"" +
                                                   to_string(k + 1) + "\"];";

                                        Estructuras::Inodo aux_inodo;
                                        fseek(disco_base_r, siguiente_inodo, SEEK_SET);
                                        fread(&aux_inodo, sizeof(Estructuras::Inodo), 1, disco_base_r);

                                        _inodos.push_back(aux_inodo);
                                        inicio_inodos.push_back(siguiente_inodo);
                                        grafica += content_id + " -> " + "INode" +
                                                   to_string(siguiente_inodo) + ";";
                                    }
                                }
                            } else {
                                Estructuras::Bloque_archivos bloque_arch;
                                fseek(disco_base_r, actual_inodo.i_block[j], SEEK_SET);
                                fread(&bloque_arch, sizeof(Estructuras::Bloque_archivos), 1, disco_base_r);

                                string file_content(bloque_arch.content);

                                std::string::size_type n = 0;
                                string salto = "\n";
                                string br = "<br/>";
                                while ((n = file_content.find( "\n", n)) !=
                                       std::string::npos) {
                                    file_content.replace(n, salto.size(), br);
                                    n += br.size();
                                }

                                string block_id = id_ino + "_bloque_" + to_string(j);
                                grafica += block_id +
                                       " [shape=none, margin=0, label=<<table "
                                       "border=\"0\" "
                                       "cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">" +
                                       "<tr><td bgcolor=\"#FFC107\"><font "
                                       "color=\"white\"><b>" +
                                           "Block " + to_string(sup_block_index) +
                                       "</b></font></td></tr><tr><td "
                                       "bgcolor=\"#FFF9C4\">" +
                                           file_content + "     </td></tr></table>>];";
                                grafica += id_ino + " -> " + block_id + ";";
                            }
                        }
                    }
                }

                grafica += "}";
                fwrite(grafica.c_str(), grafica.length(), 1, archivo_dot);
                fclose(archivo_dot);
                //Creacion de la carpeta
                string comando2 = "rmdir \"" + path + "\"";
                string crear = "mkdir -p \"" + path + "\"" ;
                system(crear.c_str());
                system(comando2.c_str());

                // GENERAR IMAGEN
                string image_svg = "dot -Tsvg " + dot_name + " -o " + path;
                system(image_svg.c_str());
            }

            fclose(disco_base_r);
        }

    }else{
        cout <<RED""<<"REPORTE NO GENERADO!, NO SE ENCONTRO:  " <<YLLW<<id<<CYN""<<endl;
    }
}
