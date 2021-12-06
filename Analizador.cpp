//
// Created by pilo-tuy on 4/12/21.
// Manejo e Implementación de Archivos 2S 2021
// CLASE ANALIZADOR PARA INTERPRETAR LOS COMANDOS DE ENTRADA

#include "Analizador.h"
#include "MKdisco.h"
#include "RMdisco.h"
#include <iostream>
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
using namespace std;


Analizador::Analizador()
{

}

// ---- funcion inicial, recoge la linea de comandos
// ---- verifica que no este vacia " "
void Analizador::Analizar()
{
    string entrada;
    while(true){
        cout <<CYN">_ " <<endl;
        getline(cin,entrada);
        if (entrada !="")
        {
            //cout <<"Ingresando para analizar el comando "<<entrada <<endl;
            Analizador::Analizar_Comando(entrada);
        }
    }

}

// ---- funcion que analiza el primer comandos de entrada
// ---- verifica si el primer comando es valido
// ---- verifica si el primer comando no es valido
void Analizador::Analizar_Comando(string _entrada) {
    vector <string> arrayinstrucciones;                             //Array del comando  y los parametros
    stringstream  comando (_entrada);                           // Convertir  el comando ingresado en buffer
    string aux;
    while (getline(comando, aux, ' '))                  // Realizar el split de los comandos
    {
        arrayinstrucciones.push_back(aux);
    }
    string instruccion = Analizador::StringToLower(arrayinstrucciones[0]);   // Contiene el comando a ejecutar

    // Compara la cadena del primer comando con las opciones validas
    if(instruccion == "mkdisk")                                            //COMANDO MKDISK
    {
        verificar_mkdisk(arrayinstrucciones);
    }
    else if(instruccion == "rmdisk")
    {
        verificar_rmdisk(arrayinstrucciones);
    }
    else if(instruccion  == "fdisk")
    {
        //verificar_fdisk(arrayinstrucciones);

    }else if(instruccion =="mount")
    {
        //verificar_mount(arrayinstrucciones);
    }else if(instruccion =="umount")
    {
        //verificar_Umount(arrayinstrucciones);
    }
    else if(instruccion == "rep")
    {
        //verificar_reporte(arrayinstrucciones;
    }
    else if(instruccion  =="exec")
    {
        //verificar_exec(arrayinstrucciones);
    }else {
        cout <<RED">> COMANDO NO VALIDO:  "<<instruccion <<endl;
    }


}

// ================================  verifica los paramentros que para el comando mk disk ========================
void Analizador::verificar_mkdisk(vector <string> lineainstruccion)
{
    string size="";
    string fit="";
    string path="";
    string unit="";
    string _error="";
    bool bandera= true;

    //--- se recorren primero los parametros obligatorios
    //--- param. -SIZE , -PATH
    for(int i=1;i<lineainstruccion.size();i++ ){
        int pos = lineainstruccion[i].find(":");
        string param = Analizador::StringToLower(lineainstruccion[i].substr(0,pos-1));
        //cout << "parametro: " << param << endl;
        if(param =="-size"){
            size=Analizador::AtributoComando(lineainstruccion[i]);
        }
        else if(param =="-path")
        {
            if(lineainstruccion[i].substr(pos+2,1) =="\""){
                path = Analizador::RutaCompleta(lineainstruccion,i);
                int salto = RutaCompletaSaltos(lineainstruccion,i);
                i=i+salto;
            }else{
                path =Analizador::AtributoComando(lineainstruccion[i]);
            }
        }
        else if(param =="-unit"){
            string aux = Analizador::AtributoComando(lineainstruccion[i]);
            aux = Analizador::StringToLower(aux);
            if(aux =="k"){
                unit = "k";
            }
            else if(aux=="m"){
                unit ="m";
            }
            else{
                _error= "UNIDAD DE TAMAÑO NO VALIDA!";
                bandera = false;
            }
        }
        else if(param =="-fit"){
            string aux = Analizador::AtributoComando(lineainstruccion[i]);
            aux = Analizador::StringToLower(aux);
            if(aux =="bf"){
                fit = "bf";
            }
            else if(aux=="ff"){
                fit ="ff";
            }
            else if(aux=="wf"){
                fit ="wf";
            }
            else{
                _error= "TIPO DE AJUSTE NO VALIDO";
                bandera = false;
            }
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "\"  EN CREACION DE DISCO  \"" + path+"\"";
            bandera = false;
        }
    }
    if(unit ==""){
        unit ="m";
    }else if(fit ==""){
        fit ="ff";
    }
    else if(path ==""){
        _error = "PATH NO VALIDO!";
        bandera = false;
    }
    else if(size ==""){
        _error = "TAMAÑO NO VALIDO!";
        bandera = false;
    }
    if(bandera){
        cout << "PARAMETROS OBTENIDOS MKDIR" <<endl;
        cout << "Size: " << size << endl;
        cout << "path: " << path << endl;
        cout << "Unit: " << unit << endl;
        cout << "Fit: " << fit << endl;

        int s=stoi(size);
        if(s){
            MKdisco crear;
            crear.Crear_Disco(s,fit,unit,path);
        }else {
            cout <<RED""<< "TAMAÑO DE DISCO NO VALIDO" <<CYN""<<endl;

        }

    }else {
        cout <<RED""<< _error <<CYN""<<endl;
    }

}

// ================================ VERIFICAR PARAMETROS COMANDO RMDISK ===============================
void Analizador::verificar_rmdisk(vector <string> lineainstruccion)
{

    string path="";
    string _error="";
    bool bandera= true;
    //--- se recorren primero los parametros obligatorios
    //--- parametro -PATH
    for(int i=1;i<lineainstruccion.size();i++ ){
        int pos = lineainstruccion[i].find(":");
        string param = Analizador::StringToLower(lineainstruccion[i].substr(0,pos-1));
        //cout << "parametro: " << param << endl;
        if(param =="-path")
        {
            if(lineainstruccion[i].substr(pos+2,1) =="\""){
                path = Analizador::RutaCompleta(lineainstruccion,i);
                int salto = RutaCompletaSaltos(lineainstruccion,i);
                i=i+salto;
            }else{
                path =Analizador::AtributoComando(lineainstruccion[i]);
            }
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "\"  EN CREACION DE DISCO  \"" + path+"\"";
            bandera = false;
        }
    }
    if(path ==""){
        _error = "PATH NO VALIDO!";
        bandera = false;
    }

    if(bandera){
        cout << "PARAMETROS OBTENIDOS RMDISK" <<endl;
        cout << "path: " << path << endl;
        RMdisco eliminar;
        eliminar.Eliminar_disco(path);
    }else {
        cout <<RED""<< _error <<CYN""<<endl;
    }
}



//================================================ FUNCIONES AUXILIARES ============================================
// ---- funcion que convierte las palabras en minuscula
// ---- recibe como parametro un array de strings
string Analizador::StringToLower(string comando)
{
    for(int i=0;i<comando.length();i++)
    {
        comando[i]= tolower(comando[i]);
    }
    return  comando;
}

// --- obtiene los atributos de cada linea ingresada
string Analizador::AtributoComando(string comando)
{
    int pos = comando.find(":");
    string atributo = comando.substr(pos + 2);
    return atributo;
}

// ---- funcion que obtiene la ruta completa
string Analizador::RutaCompleta(vector<string> ruta, int pos)
{
    string ruta_aux = ruta[pos].substr(8, ruta[pos].size() - 5);
    for (int i = pos + 1; i < ruta.size(); i++)
    {
        if (ruta[i].substr(0, 1) != "-")
        {
            ruta_aux = ruta_aux + " " + ruta[i];
        }
        else
        {
            break;
        }
    }
    if (ruta_aux.substr(ruta_aux.size() - 1, 1) == " ")
    {
        ruta_aux = ruta_aux.substr(0, ruta_aux.size() - 1);
    }
    ruta_aux.erase(remove(ruta_aux.begin(), ruta_aux.end(), '\"'), ruta_aux.end());
    return ruta_aux;
}

int Analizador::RutaCompletaSaltos(vector<string> ruta, int pos)
{
    int salto=0;
    string ruta_aux = ruta[pos].substr(8, ruta[pos].size() - 5);
    for (int i = pos + 1; i < ruta.size(); i++)
    {
        if (ruta[i].substr(0, 1) != "-")
        {
            ruta_aux = ruta_aux + " " + ruta[i];
            salto=salto+1;
        }
        else
        {
            break;
        }
    }

    return salto;
}

