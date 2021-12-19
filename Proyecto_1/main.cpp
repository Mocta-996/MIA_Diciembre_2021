#include <iostream>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Analizador.h"
#define NC "\e[0m"      // blanco
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
#define REDB "\e[41m"   // color melon



int main() {
    cout <<CYN  "##################################################################" <<endl;
    cout <<CYN  "##            "<<GRN"UNIVERSIDAD DE SAN CARLOS DE GUATEMALA"<<CYN "            ##" <<endl;
    cout <<CYN    "##               "<<GRN"INGENIERIA EN CIENCIAS Y SISTEMAS "<<CYN "             ##"  << endl;
    cout <<CYN   "##              "<<GRN"MANEJO E IMPLEMENTACION DE ARCHIVOS"<<CYN "             ##" << endl;
    cout <<CYN   "##################################################################" <<endl;
    Analizador iniciar;
    iniciar.Analizar();
    return 0;
}