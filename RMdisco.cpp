//
// Created by pilo-tuy on 6/12/21.
//
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
#include "RMdisco.h"
#include <string>
#include <iostream>
using namespace std;

RMdisco::RMdisco()
{
}

void RMdisco::Eliminar_disco(string path)
{

    cout<<CYN " DESEA ELIMINAR DISCO?"<<endl;
    cout<<CYN"S/N?"<<endl;
    string respuesta;
    cin >> respuesta;
    if (respuesta == "s")
    {
        if (remove(path.c_str()) == 0)
        {
            cout <<GRN "¡DISCO ELIMINADO CON ÉXITO !" <<CYN"" <<endl;
        }
        else
        {
            cout <<RED "¡ EL DISCO NO EXISTE !" << CYN"" << endl;
        }

    }
}