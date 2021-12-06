//
// Created by pilo-tuy on 5/12/21.
//
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
#include "MKdisco.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "Estructuras.h"
using namespace std;
MKdisco::MKdisco()
{
}
void MKdisco::Crear_Disco(int size, string fit, string unit, string path)
{
    //--- se obtiene el tamaño del disco
    int tam=0;
    if (unit=="m")
    {
        tam= size * 1024 * 1024;
    }
    else
    {
        tam = size * 1024;
    }
    // creando el disco
    // se mandan los datos al struc para la creacion del disco
    Estructuras::MBRdisco crear_disco;
    crear_disco.mbr_tamano= tam;
    crear_disco.disk_fit =fit[0];
    crear_disco.mbr_fecha_creacion=time(nullptr);
    crear_disco.disk_signature=rand() % 100;
    // inicializando MBR
    for(int i=0;i<4;i++){
        crear_disco.disco_particion[i].part_status ='0';
        crear_disco.disco_particion[i].part_type ='0';
        crear_disco.disco_particion[i].part_fit ='0';
        crear_disco.disco_particion[i].part_start =-1;
        crear_disco.disco_particion[i].part_size =0;
        strcpy(crear_disco.disco_particion[i].part_name,"");
    }
    // se comprueba si existe el disco ingresado
    FILE *bandera = fopen(path.c_str(), "r");
    if (bandera != NULL)
    {
        cout <<RED "Disco ya existente., validar ruta!" << CYN""<<endl;
        fclose(bandera);
        return;
    }
    // CREANDO EL ARCHIVO
    FILE *binario = fopen(path.c_str(), "wb+");
    if (binario != NULL)
    {
        for(int i=0; i<tam;i++)
        {
            fwrite("\0", 1, 1, binario);
        }
        fseek(binario, 0, SEEK_SET);
        fwrite(&crear_disco, sizeof(Estructuras::MBRdisco), 1, binario);
        fclose(binario);
        cout << GRN " DISCO CREADO CON EXITO !" << endl;
    }
    else
    {
        //fclose(binario);
        //Creacion de la carpeta
        string comando2 = "rmdir \"" + path + "\"";
        string crear = "mkdir -p \"" + path + "\"" ;
        system(crear.c_str());
        system(comando2.c_str());
        //cout << "Path creado exitosamente" << endl;

        binario = fopen(path.c_str(), "wb+");
        for(int i=0; i<tam;i++)
        {
            fwrite("\0", 1, 1, binario);
        }
        fseek(binario, 0, SEEK_SET);
        fwrite(&crear_disco, sizeof(Estructuras::MBRdisco), 1, binario);
        fclose(binario);
        cout <<GRN  " DISCO CREADO CON EXITO !" << endl;
    }
    //cout << "Bits del MBR: " << sizeof(Estructuras::MBRdisco) << endl;
}