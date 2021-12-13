//
// Created by pilo-tuy on 12/12/21.
//

#include "Mount.h"
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
#define YLLW "\e[0;33m"
#include <string>
#include <iostream>
#include <algorithm>
#include "Estructuras.h"
#include <charconv>
using namespace std;
Mount::Mount() {

}

void Mount::Montar(string path, string name){
    //Comprobar si ya existe la particion montada
    bool montada=false;
    for(int i=0;i<Lista_part_montadas.size();i++){
        if(Lista_part_montadas[i].path == path && Lista_part_montadas[i].name == name){
            montada = true;
            break;
        }
    }
    if(!montada){
        // ABRIR ARCHIVO
        Estructuras::MBRdisco disco_montar;
        FILE *disco_base = fopen(path.c_str(), "rb+");

        // EXISTE
        if (disco_base != NULL) {
            // OBTENER MBR
            fseek(disco_base, 0, SEEK_SET);
            fread(&disco_montar, sizeof(Estructuras::MBRdisco), 1, disco_base);

            // GLOBALES
            Estructuras::EBRparticion particiones_logicas[23];
            int logica_index = -1;
            int particion_index = -1;
            int extendida_particion = -1;
            bool encontrar_nombre = 0;

            //EN PARTICIONES PRIMARIAS Y EXTENDIDAS
            for (int partition_index = 0; partition_index < 4; partition_index++) {
                if (disco_montar.disco_particion[partition_index].part_status == '1') {
                    // NOMBRE ENCONTRADO
                    if (disco_montar.disco_particion[partition_index].part_name == name) {
                        encontrar_nombre = 1;
                        particion_index = partition_index;
                    }// EXTENDIDA
                    if (disco_montar.disco_particion[partition_index].part_type == 'e')
                        extendida_particion = partition_index;
                }
            }

            // BUSACAR EN LOGICAS
            for (int logical_partition_index = 0;
                 logical_partition_index < 23;
                 logical_partition_index++) {
                Estructuras::EBRparticion  empty_ebr;
                empty_ebr.part_status = '0';
                empty_ebr.part_start = -1;
                empty_ebr.part_next = -1;
                empty_ebr.part_size = 0;
                particiones_logicas[logical_partition_index] = empty_ebr;
            }

            if (extendida_particion != -1) {

                Estructuras::EBRparticion ebr_cabecera;
                fseek(disco_base, disco_montar.disco_particion[extendida_particion].part_start,
                      SEEK_SET);
                fread(&ebr_cabecera, sizeof(Estructuras::EBRparticion), 1, disco_base);
                particiones_logicas[0] = ebr_cabecera;

                // BUSCAR RESTO DE PARTICIONES LOGICAS
                for (int i = 0;i < 23; i++) {
                    Estructuras::EBRparticion  part_logica;
                    fseek(disco_base, particiones_logicas[i].part_next,
                          SEEK_SET);
                    fread(&part_logica, sizeof(Estructuras::EBRparticion), 1, disco_base);
                    // NOMBRE ENCONTRADO
                    if (part_logica.part_name == name) {
                        encontrar_nombre = 1;
                        logica_index = i + 1;
                    }
                    // ASIGNAR SIGUIENTE
                    if (part_logica.part_status == '1')
                        particiones_logicas[i + 1] =
                                part_logica;
                }
            }

            // PARTICION ENCONTRADA
            if (encontrar_nombre) {
                Estructuras::Particiones_Montadas partition;
                int inicio_particion = (logica_index != -1 ? sizeof(Estructuras::EBRparticion ) : 0) + (logica_index != -1
                                                                                                        ? particiones_logicas[logica_index].part_start
                                                                                                        : disco_montar.disco_particion[particion_index].part_start);
                string nombre_particion;
                int numero_disco=1;
                int letra_particion=65;
                int nuevaletra =65;
                bool nuevo_mount =true;
                // buscando numero de disco
                if(!Lista_part_montadas.empty()){
                    for(int j=0; j<Lista_part_montadas.size(); j++){
                        if(Lista_part_montadas[j].path == path){
                            numero_disco = Lista_part_montadas[j].disco+1;
                            letra_particion=Lista_part_montadas[j].letra;
                            nuevo_mount = false;
                        }
                        nuevaletra= Lista_part_montadas[j].letra>nuevaletra?Lista_part_montadas[j].letra:nuevaletra;
                    }
                    if(nuevo_mount){
                        numero_disco =1;
                        letra_particion=nuevaletra+1;
                    }
                }
                string  l;
                l.assign(1, letra_particion);
                string partition_id = "VD"+l+ to_string(numero_disco);

                // EN PARTICIONES PRIMARIAS Y EXTENDIDAS
                if (particion_index != -1)
                    nombre_particion = disco_montar.disco_particion[particion_index].part_name;

                    // EN PARTICIONES LOGICAS
                else
                    nombre_particion =particiones_logicas[logica_index].part_name;

                partition.name = nombre_particion;
                partition.id = partition_id;
                partition.path = path;
                partition.start = inicio_particion;
                partition.size = disco_montar.disco_particion[particion_index].part_size;
                partition.type = disco_montar.disco_particion[particion_index].part_type;
                partition.letra= letra_particion;
                partition.disco = numero_disco;
                Lista_part_montadas.push_back(partition);

                // Imprimir particion montadad
                for (int i = 0; i < Lista_part_montadas.size(); i++) {
                    cout << (i+1) +": "
                    << Lista_part_montadas.at(i).path <<" - "
                    << Lista_part_montadas.at(i).name << " - "
                    << Lista_part_montadas.at(i).id << "\n"<< endl;
                }
                //fclose(disco_base );
            } else
                cout<<RED"PARTICION NO ENCONTRADA "<<YLLW ""<<name<<CYN""<<endl;
            fclose(disco_base);
        } else{
            cout<<RED"DISCO NO ENCONTRADO ! "<<CYN""<<endl;
        }
    }
    else{
        cout<<RED"ESTA PARTICION YA SE ENCUENTRA MONTADA : "<<YLLW ""<<name<<CYN""<<endl;
    }
};

void Mount::UnMontar( string id){
    //Comprobar si ya existe la particion montada
    bool montada=false;
    for(int i=0;i<Lista_part_montadas.size();i++){
        if(Lista_part_montadas[i].id == id){
            montada = true;
            Lista_part_montadas.erase(Lista_part_montadas.begin()+i);
            break;
        }
    }
    if(montada){
        cout<<RED"PARTICION DESMONTADA : "<<YLLW ""<<id<<CYN""<<endl;
    }
    else{
        cout<<RED"ESTA PARTICION NO SE ENCUENTRA MONTADA : "<<YLLW ""<<id<<CYN""<<endl;
    }

}