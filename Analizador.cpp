//
// Created by pilo-tuy on 4/12/21.
// Manejo e Implementación de Archivos 2S 2021
// CLASE ANALIZADOR PARA INTERPRETAR LOS COMANDOS DE ENTRADA

#include "Analizador.h"
#include "MKdisco.h"
#include "RMdisco.h"
#include "Fdisco.h"
#include "Mount.h"
#include "Mkfs.h"
#include "Reportes.h"
#include <iostream>
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian
#define YLLW "\e[0;33m"
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
        verificar_fdisk(arrayinstrucciones);

    }else if(instruccion =="mount")
    {
        verificar_mount(arrayinstrucciones);
    }else if(instruccion =="umount")
    {
        verificar_unmount(arrayinstrucciones);
    }
    else if(instruccion  =="mkfs")
    {
        verificar_mkfs(arrayinstrucciones);
    }
    else if(instruccion  =="login")
    {
        verificar_login(arrayinstrucciones);
    }
    else if(instruccion == "rep")
    {
        verificar_reporte(arrayinstrucciones);
    }
    else if(instruccion == "pause")
    {
        Ejecutar_pause();
    }
    else if(instruccion  =="exec")
    {
        verificar_exec(arrayinstrucciones);
    }
    else if(instruccion.substr(0,1)  =="#")
    {
        cout <<GRN">>Comentario:  "<<instruccion <<endl;
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
        else if(param.substr(0,1) =="#"){
            cout << "Comentario" <<param<<endl;
            break;
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
        /*cout << "PARAMETROS OBTENIDOS MKDIR" <<endl;
        cout << "Size: " << size << endl;
        cout << "path: " << path << endl;
        cout << "Unit: " << unit << endl;
        cout << "Fit: " << fit << endl;*/

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
        /*cout << "PARAMETROS OBTENIDOS RMDISK" <<endl;
        cout << "path: " << path << endl;*/
        RMdisco eliminar;
        eliminar.Eliminar_disco(path);
    }else {
        cout <<RED""<< _error <<CYN""<<endl;
    }
}

// ================================  verifica los paramentros que para el comando fdisk ========================
void Analizador::verificar_fdisk(vector <string> lineainstruccion)
{
    string size="";
    string fit="";
    string path="";
    string unit="";
    string type="";
    string eliminar="";
    string name="";
    string add="";
    string _error="";
    bool bandera= true;

    //--- se recorren primero los parametros obligatorios
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
            else if(aux=="b"){
                unit ="b";
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
        else if(param =="-type"){
            string aux = Analizador::AtributoComando(lineainstruccion[i]);
            aux = Analizador::StringToLower(aux);
            if(aux =="p"){
                type = "p";
            }
            else if(aux=="e"){
                type ="e";
            }
            else if(aux=="l"){
                type ="l";
            }
            else{
                _error= "TIPO DE PARTICION NO VALIDO";
                bandera = false;
            }
        }
        else if(param =="-delete"){
            string aux = Analizador::AtributoComando(lineainstruccion[i]);
            aux = Analizador::StringToLower(aux);
            if(aux =="fast"){
                eliminar = "fast";
            }
            else if(aux=="full"){
                eliminar ="full";
            }
            else{
                _error= "TIPO DE ELIMINACIÓN DE PARTICIÓN NO VALIDO! ";
                bandera = false;
            }
        }
        else if(param =="-name"){
            name = Analizador::AtributoComando(lineainstruccion[i]);
        }
        else if(param =="-add"){
            add = Analizador::AtributoComando(lineainstruccion[i]);
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "\"  EN COMADO FDISK  \"" + path+"\"";
            bandera = false;
        }
    }
    if(bandera){
        // parametros obligatorios
        if(path ==""){
            _error = "PATH NO VALIDO!";
            bandera = false;
        }else if(name==""){
            _error = "NOMBRE NO VALIDO!";
            bandera = false;
        }
        if(bandera)
        {
            // CASO ELIMINAR PARTICION
            if(eliminar !=""){
                /*cout << "PARAMETROS OBTENIDOS ELIMINAR PARTICION" <<endl;
                cout << "path: " << path << endl;
                cout << "eliminar: " << eliminar << endl;
                cout << "name: " << name << endl;
                //accion de ,eliminar, path,name*/
                Fdisco agregar;
                agregar.Eliminar_particion(eliminar,path , name);
            }
            // agregar o quitar espacios
            else if(add !=""){
                if(unit ==""){
                    unit = "k";
                }
                /*cout << "PARAMETROS OBTENIDOS ADD PARTICION" <<endl;
                cout << "path: " << path << endl;
                cout << "Unit: " << unit << endl;
                cout << "name: " << name << endl;
                cout << "add: " << add << endl;*/
                int tam= 0;
                try {
                    tam= stoi(add);
                    // accion de quitar path, unit, name
                    Fdisco agregar;
                    agregar.Agregar_espacio(tam,  unit,path , name);
                } catch (exception &e) {
                    cout <<RED""<< "TAMAÑO DE PARTICION NO VALIDO" <<CYN""<<endl;
                    cout <<RED""<< "NO SE CREO LA PARTICION!" <<CYN""<<endl;
                }
            }
            // crear particion
            else {
                if(unit ==""){
                    unit = "k";
                }
                if(type ==""){
                    type = "p";
                }
                if(fit ==""){
                    fit = "wf";
                }
                int tam =0;
                try {
                    tam= stoi(size);
                } catch (exception &e) {
                    cout <<RED""<< "TAMAÑO DE PARTICION NO VALIDO" <<CYN""<<endl;
                    cout <<RED""<< "NO SE CREO LA PARTICION!" <<CYN""<<endl;
                }
                if(tam>0){
                    /*cout << "PARAMETROS OBTENIDOS CREACION DE PARTICION" <<endl;
                    cout << "Size: " << size << endl;
                    cout << "path: " << path << endl;
                    cout << "Unit: " << unit << endl;
                    cout << "Fit: " << fit << endl;
                    cout << "Type: " << type << endl;
                    cout << "name: " << name << endl;*/
                    Fdisco agregar_particion;
                    agregar_particion.Crear_particion(tam,fit,unit,path,type,name);
                    // agregar aacion de crear particion, name, path, size, fit, type, unit,
                }
                else{
                    cout <<RED""<< "EL TAMAÑO DE LA PARTICION DEBE SER MAYOR A 0" <<CYN""<<endl;
                    cout <<RED""<< "NO SE CREO LA PARTICION!" <<CYN""<<endl;
                }

            }

        }
        else
        {
            cout <<RED""<< _error <<CYN""<<endl;
        }


    }else {
        cout <<RED""<< _error <<CYN""<<endl;
        cout <<RED""<< "NO SE CREO LA PARTICION!" <<CYN""<<endl;
    }

}
// ================================  verifica los paramentros que para el comando Mount ========================
void Analizador::verificar_mount(vector <string> lineainstruccion)
{

    string path="";
    string name="";
    string _error="";
    bool bandera= true;

    //--- se recorren primero los parametros obligatorios
    //--- param. -SIZE , -PATH
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
        else if(param =="-name"){
            string aux = Analizador::AtributoComando(lineainstruccion[i]);
            //aux = Analizador::StringToLower(aux);
            name = aux;
        }
        else if(param.substr(0,1) =="#"){
            cout << "Comentario" <<param<<endl;
            break;
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "\"  EN EL COMANDO MOUNT  \"" + path+"\"";
            bandera = false;
        }
    }
   if(path ==""){
        _error = "PATH NO VALIDO!";
        bandera = false;
    }
    else if(name ==""){
        _error = "NOMBRE NO VALIDO!";
        bandera = false;
    }
    if(bandera){
        /*cout << "PARAMETROS OBTENIDOS MKDIR" <<endl;
        cout << "Size: " << name << endl;
        cout << "path: " << path << endl;*/
        Mount montar;
        montar.Montar(path, name);

    }else {
        cout <<RED""<< _error <<YLLW"   PARA EL COMANDO MOUNT"<<CYN""<<endl;
    }

}

// ================================  verifica los paramentros que para el comando UnMount ========================
void Analizador::verificar_unmount(vector <string> lineainstruccion)
{
    string id="";
    string _error="";
    bool bandera= true;

    for(int i=1;i<lineainstruccion.size();i++ ){
        int pos = lineainstruccion[i].find(":");
        string param = Analizador::StringToLower(lineainstruccion[i].substr(0,pos-1));
        //cout << "parametro: " << param << endl;
        if(param =="-id"){
            string aux = Analizador::AtributoComando(lineainstruccion[i]);
            id = aux;
        }
        else if(param.substr(0,1) =="#"){
            cout << "Comentario" <<param<<endl;
            break;
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "\"  EN EL COMANDO UNMOUNT  ";
            bandera = false;
        }
    }
    if(id ==""){
        _error = "NOMBRE NO VALIDO!";
        bandera = false;
    }
    if(bandera){
        /*cout << "PARAMETROS OBTENIDOS MKDIR" <<endl;
        cout << "id: " << id << endl;*/
        Mount montar;
        montar.UnMontar( id);

    }else {
        cout <<RED""<< _error <<CYN""<<endl;
    }

}

// ================================  verifica los paramentros que para el comando mk disk ========================
void Analizador::verificar_mkfs(vector <string> lineainstruccion)
{
    string id="";       //obligatorio
    string type="";     // opcional
    string fs="";       //opcional
    string _error ="";
    bool bandera= true;

    //--- se recorren primero los parametros obligatorios
    //--- param. -SIZE , -PATH
    for(int i=1;i<lineainstruccion.size();i++ ){
        int pos = lineainstruccion[i].find(":");
        string param = Analizador::StringToLower(lineainstruccion[i].substr(0,pos-1));
        //cout << "parametro: " << param << endl;
        if(param =="-id"){
            id=Analizador::AtributoComando(lineainstruccion[i]);
        }
        else if(param =="-type"){
            string aux = Analizador::AtributoComando(lineainstruccion[i]);
            aux = Analizador::StringToLower(aux);
            if(aux =="fast"){
                type = "fast";
            }
            else if(aux=="full"){
                type ="full";
            }
            else{
                _error= " PARAMETRO TYPE, COMANDO MKFS NO VALIDO!";
                bandera = false;
            }
        }
        else if(param =="-fs"){
            string aux = Analizador::AtributoComando(lineainstruccion[i]);
            aux = Analizador::StringToLower(aux);
            if(aux =="2fs"){
                fs = "2fs";
            }
            else if(aux=="3fs"){
                fs ="3fs";
            }
            else{
                _error= "PARAMETRO FS, COMANDO MKFS NO VALIDO!";
                bandera = false;
            }
        }
        else if(param.substr(0,1) =="#"){
            cout << "Comentario" <<param<<endl;
            break;
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "  COMANDO MKFS ";
            bandera = false;
        }
    }
    if(type ==""){
        type ="full";
    }
    if(fs ==""){
        type ="2fs";
    }
    if(id ==""){
        _error = "PARAMETRO ID NO VALIDO,COMANDO MKFS NO VALIDO!";
        bandera = false;
    }
    if(bandera){
        /*cout << "PARAMETROS OBTENIDOS MKDIR" <<endl;
        cout << "Id: " << id << endl;
        cout << "Type: " << type << endl;
        cout << "fs: " << fs << endl;*/
        Mkfs formateo;
        formateo.formateo_particion(id,type,fs);
    }else {
        cout <<RED""<< _error <<CYN""<<endl;
    }

}

// ================================  verifica los paramentros que para el comando Login ========================
void Analizador::verificar_login(vector <string> lineainstruccion)
{
    string usr="";
    string pwd="";
    string id="";
    string _error="";
    bool bandera= true;

    //--- se recorren primero los parametros obligatorios
    //--- param. -SIZE , -PATH
    for(int i=1;i<lineainstruccion.size();i++ ){
        int pos = lineainstruccion[i].find(":");
        string param = Analizador::StringToLower(lineainstruccion[i].substr(0,pos-1));
        //cout << "parametro: " << param << endl;
        if(param =="-usr"){
            if(lineainstruccion[i].substr(pos+2,1) =="\""){
                usr = Analizador::RutaCompleta(lineainstruccion,i);
                int salto = RutaCompletaSaltos(lineainstruccion,i);
                i=i+salto;
            }else{
                usr =Analizador::AtributoComando(lineainstruccion[i]);
            }
        }
        else if(param =="-id"){
            id =Analizador::AtributoComando(lineainstruccion[i]);
        }
        else if(param =="-pwd"){
            pwd =Analizador::AtributoComando(lineainstruccion[i]);
        }
        else if(param.substr(0,1) =="#"){
            cout << "Comentario" <<param<<endl;
            break;
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "\"  EN EL COMANDO UNMOUNT  ";
            bandera = false;
        }
    }
    if(id ==""){
        _error = "NOMBRE NO VALIDO! ";
        bandera = false;
    }
    if(pwd ==""){
        _error = "CONTRASEÑA  NO VALIDA!";
        bandera = false;
    }
    if(usr ==""){
        _error = "USUARIO NO VALIDO!";
        bandera = false;
    }
    if(bandera){
        cout << "PARAMETROS OBTENIDOS LOGIN" <<endl;
        cout << "id: " << id << endl;
        cout << "pwd: " << pwd << endl;
        cout << "usr: " << usr << endl;
        /*Mount montar;
        montar.UnMontar( id);*/

    }else {
        cout <<RED""<< _error <<YLLW"   PARA EL COMANDO LOGIN"<<CYN""<<endl;
    }

}

// ================================  verifica los paramentros que para el comando reporte ========================
void Analizador::verificar_reporte(vector <string> lineainstruccion)
{
    string name="";
    string path="";
    string id="";
    string ruta="";
    string _error="";
    bool bandera= true;

    //--- se recorren primero los parametros obligatorios
    //--- param. -SIZE , -PATH
    for(int i=1;i<lineainstruccion.size();i++ ){
        int pos = lineainstruccion[i].find(":");
        string param = Analizador::StringToLower(lineainstruccion[i].substr(0,pos-1));
        //cout << "parametro: " << param << endl;
        if(param =="-path"){
            if(lineainstruccion[i].substr(pos+2,1) =="\""){
                path = Analizador::RutaCompleta(lineainstruccion,i);
                int salto = RutaCompletaSaltos(lineainstruccion,i);
                i=i+salto;
            }else{
                path =Analizador::AtributoComando(lineainstruccion[i]);
            }
        }
        else if(param =="-name"){
            name =Analizador::AtributoComando(lineainstruccion[i]);
        }
        else if(param =="-id"){
            id =Analizador::AtributoComando(lineainstruccion[i]);
        }
        else if(param =="-ruta"){
            if(lineainstruccion[i].substr(pos+2,1) =="\""){
                ruta = Analizador::RutaCompleta(lineainstruccion,i);
                int salto = RutaCompletaSaltos(lineainstruccion,i);
                i=i+salto;
            }else{
                ruta =Analizador::AtributoComando(lineainstruccion[i]);
            }
        }
        else if(param.substr(0,1) =="#"){
            cout << "Comentario" <<param<<endl;
            break;
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "\"  EN EL COMANDO UNMOUNT  ";
            bandera = false;
        }
    }
    if(id ==""){
        _error = "ID REPORTE NO VALIDO! ";
        bandera = false;
    }
    if(name ==""){
        _error = "NOMBRE  DE REPORTE NO VALIDO!";
        bandera = false;
    }
    if(path ==""){
        _error = "RUTA DE REPORTE NO VALIDO!";
        bandera = false;
    }
    if(bandera){
        Reportes  reporte;
        string aux= StringToLower(name);
        // reporte mbr
        if(aux== "mbr"){
            /*cout << "PARAMETROS OBTENIDOs DE REPORTE MBR" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;*/
            reporte.Reporte_mbr( id,  path);
        }// reporte disk
        else if(aux== "disk"){
            /*cout << "PARAMETROS OBTENIDOs DE REPORTE DISK" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;*/
            reporte.Reporte_disk( id,  path);

        }// reporte inode
        else if(aux== "inode"){
            /*cout << "PARAMETROS OBTENIDOs DE REPORTE Inode" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;*/
        }// reporte journaling
        else if(aux== "journaling"){
            cout << "PARAMETROS OBTENIDOs DE REPORTE JOURNALING" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;
        }// reporte block
        else if(aux== "block"){
            cout << "PARAMETROS OBTENIDOs DE REPORTE BLOCK" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;
        }// reporte bm inode
        else if(aux== "bm_inode"){
            cout << "PARAMETROS OBTENIDOs DE REPORTE BMINODE" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;
        }// reporte bm block
        else if(aux== "bm_block"){
            cout << "PARAMETROS OBTENIDOs DE REPORTE BMBLOCK" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;
        }// reporte tree
        else if(aux== "tree"){
            cout << "PARAMETROS OBTENIDOs DE REPORTE TREE" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;
        }// reporte inode
        else if(aux== "sb"){
            cout << "PARAMETROS OBTENIDOs DE REPORTE SB" <<endl;
            cout << "name: " << name << endl;
            cout << "id: " << id << endl;
            cout << "path: " << path << endl;
        }


        /*Mount montar;
        montar.UnMontar( id);*/

    }else {
        cout <<RED""<< _error <<YLLW"   PARA EL COMANDO REPORTE"<<CYN""<<endl;
    }

}

// ================================  verifica los paramentros que para el comando Mount ========================
void Analizador::verificar_exec(vector <string> lineainstruccion)
{

    string path="";
    string _error="";
    bool bandera= true;

    for(int i=1;i<lineainstruccion.size();i++ ){
        int pos = lineainstruccion[i].find(":");
        string param = Analizador::StringToLower(lineainstruccion[i].substr(0,pos-1));
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
        else if(param.substr(0,1) =="#"){
            cout << "Comentario" <<param<<endl;
            break;
        }
        else {
            _error= "ENTRADA NO VALIDA: \"" + param + "\"  EN EL COMANDO MOUNT  \"" + path+"\"";
            bandera = false;
        }
    }
    if(path ==""){
        _error = "PATH NO VALIDO!";
        bandera = false;
    }
    if(bandera){
        /*cout << "PARAMETROS OBTENIDOS EXEC" <<endl;
        cout << "path: " << path << endl;*/
        Ejecutar_exec(path);

    }else {
        cout <<RED""<< _error <<YLLW"   PARA EL COMANDO EXEC"<<CYN""<<endl;
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

//=========================================== COMANDO EXEC =======================
void Analizador::Ejecutar_exec(string path)
{
    vector <string> comandos_exec;
    ifstream a(path.c_str());
    if(a){
        std::string entrada;
        while (std::getline(a, entrada))
        {
            std::istringstream iss(entrada);
            if (entrada != "" && entrada != "\n")
            {
                comandos_exec.push_back(entrada);
                //cout << "COMANDO INGRESANDO: "<<entrada << endl;
                //
            }
        }
        cout<< "COMANDOS EXEC INGRESADOS"<<endl;
        for(int i=0; i<comandos_exec.size();i++){
            //cout<< comandos_exec[i]<<endl;
            Analizador::Analizar_Comando(comandos_exec[i]);
        }
    }else {
        cout<<RED "NO SE ENCONTRO EL ARCHIVO PARA EL COMANDO EXEC!  "<<YLLW""<<path<<endl;
    }

}

//=========================================== COMANDO PAUSE =======================
void Analizador::Ejecutar_pause()
{
    int flag;
    cout<<YLLW "PROCESO PAUSADO!  "<<CYN""<<endl;
    cout<<YLLW "PRESIONE ENTER PARA CONTINUAR!  "<<CYN""<<endl;
    flag = getc(stdin);
    cout<<YLLW "CONTINUANDO...  "<<CYN""<<endl;
}
