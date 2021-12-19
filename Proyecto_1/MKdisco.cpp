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
/*
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
*/

void MKdisco::Crear_Disco(int size, string fit, string unit, string path)
{
    //--- se obtiene el tamaño del disco
    int tam=0;
    if (unit=="m")
    {
        tam= size * 1024 ;
    }
    else
    {
        tam = size * 1;
    }
    // creando el disco
    // se mandan los datos al struc para la creacion del disco
    Estructuras::MBRdisco crear_disco;
    crear_disco.mbr_tamano= unit=="k"? size*1024: size *1024*1024;
    crear_disco.disk_fit =fit[0];
    //crear_disco.mbr_fecha_creacion= Fecha_actual();
    crear_disco.disk_signature=rand() % 100;
    string date = Obtener_fecha();
    strcpy(crear_disco.mbr_fecha_creacion, date.c_str());

    //Creando EBR inicial
    string _name = "-";
    Estructuras::EBRparticion  inicial;
    inicial.part_fit = 'w';
    inicial.part_size = -1;
    inicial.part_start =-1;
    inicial.part_status ='0';
    inicial.part_next=-1;
    strcpy(inicial.part_name, _name.c_str());

    // creacion de particiones del mbr
    Estructuras::Particion  particion_inicial;
    strcpy(particion_inicial.part_name, _name.c_str());
    particion_inicial.part_status = '0';
    particion_inicial.part_type = 'p';
    particion_inicial.part_start = -1;
    particion_inicial.part_size = -1;
    particion_inicial.part_fit = 'w';

    // agregando las particiones vacias
    for(int i=0;i<4;i++){
        crear_disco.disco_particion[i]= particion_inicial;
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
        char buffer[1024];
        for (int i = 0; i < 1024; i++){
            buffer[i] = '\0';
        }

        for(int i=0; i<tam;i++)
        {
            fwrite(&buffer, 1024, 1, binario);
            //fwrite("\0", 1, 1, binario);
        }
        //Guardando el mbr
        fseek(binario, 0, SEEK_SET);
        fwrite(&crear_disco, sizeof(Estructuras::MBRdisco), 1, binario);
        // GUARDAR EBR
        fseek(binario, sizeof(Estructuras::MBRdisco) + 1, SEEK_SET);
        fwrite(&inicial, sizeof( Estructuras::EBRparticion), 1, binario);
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
        char buffer[1024];
        for (int i = 0; i < 1024; i++){
            buffer[i] = '\0';
        }

        for(int i=0; i<tam;i++)
        {
            fwrite(&buffer, 1024, 1, binario);
        }
        //Guardando el mbr
        fseek(binario, 0, SEEK_SET);
        fwrite(&crear_disco, sizeof(Estructuras::MBRdisco), 1, binario);
        // GUARDAR EBR
        fseek(binario, sizeof(Estructuras::MBRdisco) + 1, SEEK_SET);
        fwrite(&inicial, sizeof( Estructuras::EBRparticion), 1, binario);
        fclose(binario);
        cout << GRN " DISCO CREADO CON EXITO !" << endl;
    }
    //cout << "Bits del MBR: " << sizeof(Estructuras::MBRdisco) << endl;
}

//============= Obtener fecha =================
string MKdisco:: Obtener_fecha() {
    time_t time_now = time(0);
    tm *now = localtime(&time_now);
    string retornar_fecha =
            formato_fecha(now->tm_mday) + "/" + formato_fecha(now->tm_mon + 1) +
            "/" + to_string(now->tm_year + 1900) + " " +
                    formato_fecha(now->tm_hour) + ":" + formato_fecha(now->tm_min);
    return retornar_fecha;
}

string MKdisco::formato_fecha(int field) {
    return (field < 10 ? "0" : "") + to_string(field);
}

