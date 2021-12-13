//
// Created by pilo-tuy on 6/12/21.
//
#define RED "\e[0;31m"  // rojo
#define GRN "\e[0;32m" // verde
#define CYN "\e[0;36m" // cian

bool comp(const int &a, const int &b);

#include "Fdisco.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "Estructuras.h"
using namespace std;
Fdisco::Fdisco()
{
}
/*
void Fdisco::Crear_particion(int size, string fit, string unit, string path,string type , string name)
{
    // tamaño de la particion a crear
    int tam =0;
    if(unit =="b"){
        tam= size;
    }else if(unit == "k"){
        tam= size*1024;
    }else if(unit =="m"){
        tam= (size*1024*1024);
    }

    string _error ="";
    bool bandera = false;
    //verificar si existe el disco
    Estructuras::MBRdisco discoparticion;//disco base
    FILE *disco_base = fopen(path.c_str(), "rb+");
    if(disco_base !=NULL){
        rewind(disco_base);
        fread(&discoparticion, sizeof(discoparticion),1,disco_base);
        fclose(disco_base);
        //vector<Estructuras::Particion > particiones_disco;
        //particiones_disco.push_back(discoparticion.disco_particion);
        vector<Aux> _aux;

        int ocupados=0;
        int extendido =0;
        int contador =0;
        int tam_disco = sizeof(discoparticion);
        Estructuras::Particion ext;
        for(Estructuras::Particion part_aux: discoparticion.disco_particion){
            if(part_aux.part_status == '1'){
                Aux _aux2;
                _aux2.inicio= part_aux.part_start;
                _aux2.fin = part_aux.part_start + part_aux.part_size;
                _aux2.particion = contador;
                _aux2.anterior = _aux2.inicio + tam_disco;
                tam_disco = _aux2.fin;
                if(ocupados !=0)
                {
                    _aux.at(ocupados-1).siguiente = _aux2.inicio - (_aux.at(ocupados-1).fin);
                }
                _aux.push_back(_aux2);
                ocupados++;
                if(part_aux.part_type == 'e'){
                    extendido++;
                    ext = part_aux;
                }

            }
            if(ocupados == 4 && type !="l"){
                bandera = true;
                _error = "NO SE PUEDEN AGREGAR MAS PARTICIONES AL DISCO! ";
                break;
            }
            if(extendido>0 && type =="e" ){
                bandera = true;
                _error = "YA EXISTE UNA PARTICIÓN EXTENDIDA!, NO SE PERMITEN MAS ";
                break;
            }
            contador ++;
        }
        //-------------------  COMPROBANDO ERRORES---------------
        if(bandera == false){
            // comporbando si existen extendidas para particiones logicas
            if(extendido ==0 && type =="l"){
                bandera = true;
                _error = "NO EXISTE PARTICION EXTENDIDAD PARA LA PARTICION LOGICA! ";
            }
            if (ocupados != 0) {
                _aux.at(_aux.size() - 1).anterior = discoparticion.mbr_tamano - _aux.at(_aux.size() - 1).fin;
            }
            //-------------------  COMPROBANDO ERRORES---------------
            if(bandera){
                // imprimiendo error
                cout <<RED <<_error << CYN""<< endl;
            }else {
                //-------------------  VERIFICANDO SI YA EXISTE LA PARTICION ---------------
                if(!Verficarnombre(discoparticion,type,name)){
                    // creando nueva particion
                    Estructuras::Particion nueva_particion;
                    nueva_particion.part_type=type[0];
                    nueva_particion.part_status = '1';
                    nueva_particion.part_size =tam;
                    nueva_particion.part_fit = fit[0];
                    strcpy(nueva_particion.part_name, name.c_str());


                }
                else{
                    cout <<RED "YA QUE EXISTE UNA PARTICION CON EL NOMBRE:  " << name<<CYN""<< endl;
                }
            }
        }
        else {
            cout <<RED <<_error << CYN""<< endl;
        }
    }
    else {
        cout <<RED "NO SE ENCOTRO EL DISCO! " << CYN""<< endl;
    }


    //cout << "Bits del MBR: " << sizeof(Estructuras::MBRdisco) << endl;
}

bool Fdisco::Verficarnombre(Estructuras::MBRdisco disco,string type , string name)
{
    bool existe=false;
    for(Estructuras::Particion part: disco.disco_particion){
        if(part.part_status =='1' ){
            string aux;
            aux.assign(part.part_name, 16);
            if(aux==name){
                existe = true;
                break;
            }else if(part.part_type =='e'){
                cout <<"EXISTE PARTICION EXTENDIDAD"<< endl;
                existe = true;
                break;
            }

        }
    }



    return existe;

}
*/

/*void Fdisco::Crear_particion(int size, string fit, string unit, string path,string type , string name)
{
    // tamaño de la particion a crear
    int tam =0;
    if(unit =="b"){
        tam= size;
    }else if(unit == "k"){
        tam= size*1024;
    }else if(unit =="m"){
        tam= (size*1024*1024);
    }

    string _error ="";
    bool bandera = false;
    //verificar si existe el disco
    Estructuras::MBRdisco discoparticion;//disco base
    FILE *disco_base = fopen(path.c_str(), "rb+");
    if(disco_base !=NULL){
        // variables para el espacio disponible y posiciones
        int inicio_particion=-1;
        int particion_extendida=-1;
        int total_particiones=0;

        // OBTENER MBR
        fseek(disco_base, 0, SEEK_SET);
        fread(&discoparticion, sizeof(Estructuras::MBRdisco), 1, disco_base);
        // obteniendo espacio para la particion
        for (int i = 0; i < 4; i++) {
            if (discoparticion.disco_particion[i].part_status == '1') {
                total_particiones++;
                // si existe particion extendida
                if (discoparticion.disco_particion[i].part_type == 'e')
                    particion_extendida = i;
                // busacar nombre
                if (discoparticion.disco_particion[i].part_name == name) {
                    bandera=true;
                    _error ="YA EXISTE UNA PARTICION CON EL MISMO NOMBRE!";
                    inicio_particion = i;
                }
            }
        }
        //verificar si ya existe la particion
        if(bandera ==false){
            //verificar si ya existe una extendida
            if( type =="e" && particion_extendida !=-1){
                bandera=true;
                _error ="SOLO SE PUEDE CREAR UNA PARTICION EXTENDIDA! " ;
            }
            //verificar si es una logica pero sin nunguna extendiad
            if(particion_extendida ==-1 && type =="l"){
                bandera=true;
                _error ="NO EXISTE UNA PARTICION EXTENDIDA, NO SE PUEDE CREAR UNA PARTICION LOGICA! ";
            }
            if(bandera==false){
                    // calculando el espacio
                    bool inactivo = 0;
                    int indice_libre = -1;
                    int inicio_particion = sizeof(Estructuras::MBRdisco) + 1;
                    int libre = discoparticion.mbr_tamano - sizeof(Estructuras::MBRdisco);
                    //buscar posicion
                    int f = Ajuste(tam, discoparticion, fit);
                    if(total_particiones>0){
                        for (int i = 0; i < 4;i++) {
                            // particion eliminada ydisponible
                            if (discoparticion.disco_particion[i].part_status== '0') {
                                if (discoparticion.disco_particion[i].part_size != -1) {
                                    int siguiente = i + 1;
                                    int s_libre =-2;
                                    if(siguiente < 4){
                                        s_libre =discoparticion.disco_particion[siguiente].part_start - discoparticion.disco_particion[i].part_start;
                                    }
                                    else{
                                        s_libre =discoparticion.mbr_tamano - sizeof(Estructuras::MBRdisco)-
                                                (discoparticion.disco_particion[i].part_start + discoparticion.disco_particion[i].part_size);
                                    }
                                    libre -= s_libre;
                                    // validando y asignando bloque
                                    if (s_libre>= tam) {
                                        if (siguiente == 4){
                                            inactivo = 1;
                                            indice_libre= i;
                                            inicio_particion =discoparticion.disco_particion[i].part_start;
                                            break;
                                        }
                                    }
                                }
                            } else{
                                libre  -= discoparticion.disco_particion[i].part_size;
                            }
                        }
                        // ultimo
                        if ((indice_libre == -1 && libre >= tam) || inactivo) {
                            int contador = 0;
                            for (int i = 0; i < 4;i++) {
                                if (discoparticion.disco_particion[i].part_status == '1') {
                                    discoparticion.disco_particion[contador] = discoparticion.disco_particion[i];
                                    contador++;
                                }
                            }
                            indice_libre = total_particiones;
                            inicio_particion =discoparticion.disco_particion[total_particiones - 1].part_start + 1 + discoparticion.disco_particion[total_particiones - 1].part_size;
                        }
                    }
                    else{
                        indice_libre =0;
                    }
                    //--- CREANDO PARTICIONES -----
                if (indice_libre != -1) {

                    if (total_particiones < 4) {
                            // particiones logicas
                            if(type =="l"){
                                // BUSCAR EN PARTICIONES LOGICAS
                                int buscar_logica = -1;
                                Estructuras::EBRparticion  particiones_logicas[23];
                                for (int i = 0;i < 23;i++) {
                                    Estructuras::EBRparticion _ebr;
                                    _ebr.part_status = '0';
                                    _ebr.part_start = -1;
                                    _ebr.part_next = -1;
                                    _ebr.part_size = 0;
                                    particiones_logicas[i] = _ebr;
                                }

                                if (particion_extendida != -1) {
                                    // CABECERA DE PARTICIONES LOGICAS
                                    Estructuras::EBRparticion ebr_cabecera;
                                    fseek(disco_base, discoparticion.disco_particion[particion_extendida ].part_start,SEEK_SET);
                                    fread(&ebr_cabecera, sizeof(Estructuras::EBRparticion), 1, disco_base);
                                    particiones_logicas[0] = ebr_cabecera;

                                    // BUSCAR RESTO DE PARTICIONES LOGICAS
                                    for (int i = 0; i < 23;i++) {
                                        Estructuras::EBRparticion part_logica;
                                        fseek(disco_base, particiones_logicas[i].part_next,SEEK_SET);
                                        fread(&part_logica, sizeof(Estructuras::EBRparticion), 1, disco_base);

                                        // NOMBRE ENCONTRADO
                                        if (part_logica.part_name == name) {
                                            bandera= true;
                                            _error = "  YA EXISTE UNA PARTICION LOGICA CON ESTE NOMBRE";
                                            buscar_logica = i + 1;
                                        }
                                        // ASIGNAR SIGUIENTE
                                        if (part_logica.part_status== '1')
                                            particiones_logicas[i + 1] = part_logica;
                                    }
                                }
                                if(bandera ==false){
                                    int logica_inicio = 0;
                                    int nueva_logica_indice = 0;
                                    // calculando espacio disponible
                                    for (int i = 0; i < 23; i++) {
                                        int inicio_l = particiones_logicas[i].part_start +sizeof(Estructuras::EBRparticion ) +particiones_logicas[i].part_size;
                                        int logica_libre = particiones_logicas[i + 1].part_start -inicio_l;
                                        if (logica_libre >= tam) {
                                            logica_inicio = inicio_l + 1;
                                            nueva_logica_indice =i + 1;
                                            break;
                                        }
                                    }

                                    // GUARDAR NUEVA PARTICION LOGICA
                                    if (logica_inicio != 0) {
                                        Estructuras::EBRparticion nueva_logica_particion;
                                        strcpy(nueva_logica_particion.part_name, name.c_str());
                                        nueva_logica_particion.part_next = particiones_logicas[nueva_logica_indice].part_start;
                                        nueva_logica_particion.part_start = logica_inicio;
                                        nueva_logica_particion.part_status = '1';
                                        nueva_logica_particion.part_size = tam;
                                        nueva_logica_particion.part_fit = fit[0];

                                        // guardando particion logica
                                        fseek(disco_base, logica_inicio, SEEK_SET);
                                        fwrite(&nueva_logica_particion, sizeof(Estructuras::EBRparticion), 1, disco_base);

                                        // CAMBIAR PUNTERO DE PARTICION ANTERIOR
                                        Estructuras::EBRparticion aux_logica = particiones_logicas[nueva_logica_indice - 1];
                                        aux_logica.part_next = nueva_logica_particion.part_start;

                                        // GUardando particion logica
                                        fseek(disco_base, aux_logica.part_start, SEEK_SET);
                                        fwrite(&aux_logica, sizeof(Estructuras::EBRparticion), 1,disco_base);
                                        fclose(disco_base);
                                        cout <<GRN<<"PARTICION LOGICA CREADA CON EXITO:  "<<name<< CYN""<< endl;

                                    }else {
                                        cout <<GRN<<"NO HAY ESPACIO DISPONIBLE EN EL DISCO: "<< CYN""<< endl;
                                    }
                                } else{
                                    cout <<RED<<_error << CYN""<< endl;
                                }

                            }
                            // particiones primarias y extendidas
                            else {
                            // CREAR NUEVA PARTICION
                            Estructuras::Particion nueva_particion;
                            strcpy(nueva_particion.part_name, name.c_str());
                            nueva_particion.part_start = inicio_particion;
                            nueva_particion.part_status = '1';
                            nueva_particion.part_type = type[0];
                            nueva_particion.part_size = tam;
                            nueva_particion.part_fit = fit[0];
                            discoparticion.disco_particion[indice_libre ] = nueva_particion;
                            fseek(disco_base, 0, SEEK_SET);
                            fwrite(&discoparticion, sizeof(Estructuras::MBRdisco), 1, disco_base);
                            type != "e"?cout <<GRN<<"PARTICION PRIMARIA CREADO CON EXITO: "<< name<< CYN""<< endl: cout <<""<<endl;
                            // ebr para las extendida

                            if (type == "e") {
                                // NUEVO EBR
                                Estructuras::EBRparticion  nueva_ebr;
                                string cabecera_ebr = "H_" + to_string(discoparticion.disk_signature);
                                nueva_ebr.part_next = inicio_particion + tam - sizeof(Estructuras::EBRparticion);
                                strcpy(nueva_ebr.part_name, cabecera_ebr.c_str());
                                nueva_ebr.part_start = inicio_particion;
                                nueva_ebr.part_size = 0;
                                nueva_ebr.part_status = '1';
                                nueva_ebr.part_fit = 'w';
                                // GUARDAR
                                fseek(disco_base, inicio_particion, SEEK_SET);
                                fwrite(&nueva_ebr, sizeof(Estructuras::EBRparticion), 1, disco_base);

                                // NUEVO EBR
                                Estructuras::EBRparticion ebr_f;
                                string foot_ebr_name ="F_" + to_string(discoparticion.disk_signature);
                                strcpy(ebr_f.part_name, foot_ebr_name.c_str());
                                ebr_f.part_start =inicio_particion+ tam - sizeof(Estructuras::EBRparticion);
                                ebr_f.part_size = 0;
                                ebr_f.part_next = -1;
                                ebr_f.part_status = '1';
                                ebr_f.part_fit = 'w';
                                // GUARDAR
                                fseek(disco_base, inicio_particion + tam - sizeof(Estructuras::EBRparticion),SEEK_SET);
                                fwrite(&ebr_f, sizeof(Estructuras::EBRparticion), 1, disco_base);
                                cout <<GRN<<"PARTICION EXTENDIDA CREADO CON EXITO: "<< name<< CYN""<< endl;
                            }
                                fclose(disco_base);
                            }
                    } else{
                        cout <<RED<<"NO SE PERMITEN MAS PARTICIONES"<< CYN""<< endl;
                    }
                } else{
                    cout <<RED<<"NO EXISTE ESPACIO SUFICIENTE EN EL DISCO" << CYN""<< endl;
                }


            }
            else {
                cout <<RED<<_error << CYN""<< endl;
            }
        }
        else {
            cout <<RED<<_error << CYN""<< endl;
        }
    }
    else {
        cout <<RED "NO SE ENCOTRO EL DISCO! " << CYN""<< endl;
    }
}
 *
 * */
void Fdisco::Crear_particion(int size, string fit, string unit, string path,string type , string name)
{
    // tamaño de la particion a crear
    int tam =0;
    if(unit =="b"){
        tam= size;
    }else if(unit == "k"){
        tam= size*1024;
    }else if(unit =="m"){
        tam= (size*1024*1024);
    }

    string _error ="";
    bool bandera = false;
    //verificar si existe el disco
    Estructuras::MBRdisco discoparticion;//disco base
    FILE *disco_base = fopen(path.c_str(), "rb+");
    if(disco_base !=NULL){
        // variables para el espacio disponible y posiciones
        int inicio_particion=-1;
        int particion_extendida=-1;
        int total_particiones=0;

        // OBTENER MBR
        fseek(disco_base, 0, SEEK_SET);
        fread(&discoparticion, sizeof(Estructuras::MBRdisco), 1, disco_base);
        // obteniendo espacio para la particion
        for (int i = 0; i < 4; i++) {
            if (discoparticion.disco_particion[i].part_status == '1') {
                total_particiones++;
                // si existe particion extendida
                if (discoparticion.disco_particion[i].part_type == 'e')
                    particion_extendida = i;
                // busacar nombre
                if (discoparticion.disco_particion[i].part_name == name) {
                    bandera=true;
                    _error ="YA EXISTE UNA PARTICION CON EL MISMO NOMBRE!";
                    inicio_particion = i;
                }
            }
        }
        //verificar si ya existe la particion
        if(bandera ==false){
            //verificar si ya existe una extendida
            if( type =="e" && particion_extendida !=-1){
                bandera=true;
                _error ="SOLO SE PUEDE CREAR UNA PARTICION EXTENDIDA! " ;
            }
            //verificar si es una logica pero sin nunguna extendiad
            if(particion_extendida ==-1 && type =="l"){
                bandera=true;
                _error ="NO EXISTE UNA PARTICION EXTENDIDA, NO SE PUEDE CREAR UNA PARTICION LOGICA! ";
            }
            if(bandera==false){
                    // calculando el espacio
                    bool inactivo = 0;
                    int indice_libre = -1;
                    int inicio_particion = sizeof(Estructuras::MBRdisco) + 1;
                    int libre = discoparticion.mbr_tamano - sizeof(Estructuras::MBRdisco);
                    //buscar posicion
                    //int f = Ajuste(tam, discoparticion, fit);
                    // EXISTEN PARTICIONES?
                    if(total_particiones>0){
                        for (int i = 0; i < 4;i++) {
                            // particion eliminada ydisponible
                            if (discoparticion.disco_particion[i].part_status== '0') {
                                if (discoparticion.disco_particion[i].part_size != -1) {
                                    int siguiente = i + 1;
                                    int s_libre =-2;
                                    if(siguiente < 4){
                                        s_libre =discoparticion.disco_particion[siguiente].part_start - discoparticion.disco_particion[i].part_start;
                                    }
                                    else{
                                        s_libre =discoparticion.mbr_tamano - sizeof(Estructuras::MBRdisco)-
                                                (discoparticion.disco_particion[i].part_start + discoparticion.disco_particion[i].part_size);
                                    }
                                    libre -= s_libre;
                                    // validando y asignando bloque
                                    if (s_libre>= tam) {
                                        if (siguiente == 4){
                                            inactivo = 1;
                                            indice_libre= i;
                                            inicio_particion =discoparticion.disco_particion[i].part_start;
                                            break;
                                        }
                                    }
                                }
                            } else{
                                libre  -= discoparticion.disco_particion[i].part_size;
                            }
                        }
                        // ultimo
                        if ((indice_libre == -1 && libre >= tam) || inactivo) {
                            int contador = 0;
                            for (int i = 0; i < 4;i++) {
                                if (discoparticion.disco_particion[i].part_status == '1') {
                                    discoparticion.disco_particion[contador] = discoparticion.disco_particion[i];
                                    contador++;
                                }
                            }
                            indice_libre = total_particiones;
                            inicio_particion =discoparticion.disco_particion[total_particiones - 1].part_start + 1 + discoparticion.disco_particion[total_particiones - 1].part_size;
                        }

                        /*if(f!=-1){
                            inicio_particion = f;
                        }
                        else{
                            indice_libre = -1;
                        }*/
                    }
                    else{
                        indice_libre =0;
                    }
                    //--- CREANDO PARTICIONES -----
                if (indice_libre != -1) {

                    if (total_particiones < 4) {
                            // particiones logicas
                            if(type =="l"){
                                // BUSCAR EN PARTICIONES LOGICAS
                                int buscar_logica = -1;
                                Estructuras::EBRparticion  particiones_logicas[23];
                                for (int i = 0;i < 23;i++) {
                                    Estructuras::EBRparticion _ebr;
                                    _ebr.part_status = '0';
                                    _ebr.part_start = -1;
                                    _ebr.part_next = -1;
                                    _ebr.part_size = 0;
                                    particiones_logicas[i] = _ebr;
                                }

                                if (particion_extendida != -1) {
                                    // CABECERA DE PARTICIONES LOGICAS
                                    Estructuras::EBRparticion ebr_cabecera;
                                    fseek(disco_base, discoparticion.disco_particion[particion_extendida ].part_start,SEEK_SET);
                                    fread(&ebr_cabecera, sizeof(Estructuras::EBRparticion), 1, disco_base);
                                    particiones_logicas[0] = ebr_cabecera;

                                    // BUSCAR RESTO DE PARTICIONES LOGICAS
                                    for (int i = 0; i < 23;i++) {
                                        Estructuras::EBRparticion part_logica;
                                        fseek(disco_base, particiones_logicas[i].part_next,SEEK_SET);
                                        fread(&part_logica, sizeof(Estructuras::EBRparticion), 1, disco_base);

                                        // NOMBRE ENCONTRADO
                                        if (part_logica.part_name == name) {
                                            bandera= true;
                                            _error = "  YA EXISTE UNA PARTICION LOGICA CON ESTE NOMBRE";
                                            buscar_logica = i + 1;
                                        }
                                        // ASIGNAR SIGUIENTE
                                        if (part_logica.part_status== '1')
                                            particiones_logicas[i + 1] = part_logica;
                                    }
                                }
                                if(bandera ==false){
                                    int logica_inicio = 0;
                                    int nueva_logica_indice = 0;
                                    // calculando espacio disponible
                                    for (int i = 0; i < 23; i++) {
                                        int inicio_l = particiones_logicas[i].part_start +sizeof(Estructuras::EBRparticion ) +particiones_logicas[i].part_size;
                                        int logica_libre = particiones_logicas[i + 1].part_start -inicio_l;
                                        if (logica_libre >= tam) {
                                            logica_inicio = inicio_l + 1;
                                            nueva_logica_indice =i + 1;
                                            break;
                                        }
                                    }

                                    // GUARDAR NUEVA PARTICION LOGICA
                                    if (logica_inicio != 0) {
                                        Estructuras::EBRparticion nueva_logica_particion;
                                        strcpy(nueva_logica_particion.part_name, name.c_str());
                                        nueva_logica_particion.part_next = particiones_logicas[nueva_logica_indice].part_start;
                                        nueva_logica_particion.part_start = logica_inicio;
                                        nueva_logica_particion.part_status = '1';
                                        nueva_logica_particion.part_size = tam;
                                        nueva_logica_particion.part_fit = fit[0];

                                        // guardando particion logica
                                        fseek(disco_base, logica_inicio, SEEK_SET);
                                        fwrite(&nueva_logica_particion, sizeof(Estructuras::EBRparticion), 1, disco_base);

                                        // CAMBIAR PUNTERO DE PARTICION ANTERIOR
                                        Estructuras::EBRparticion aux_logica = particiones_logicas[nueva_logica_indice - 1];
                                        aux_logica.part_next = nueva_logica_particion.part_start;

                                        // GUardando particion logica
                                        fseek(disco_base, aux_logica.part_start, SEEK_SET);
                                        fwrite(&aux_logica, sizeof(Estructuras::EBRparticion), 1,disco_base);
                                        fclose(disco_base);
                                        cout <<GRN<<"PARTICION LOGICA CREADA CON EXITO:  "<<name<< CYN""<< endl;

                                    }else {
                                        cout <<GRN<<"NO HAY ESPACIO DISPONIBLE EN EL DISCO: "<< CYN""<< endl;
                                    }
                                } else{
                                    cout <<RED<<_error << CYN""<< endl;
                                }

                            }
                            // particiones primarias y extendidas
                            else {
                            // CREAR NUEVA PARTICION
                            Estructuras::Particion nueva_particion;
                            strcpy(nueva_particion.part_name, name.c_str());
                            nueva_particion.part_start = inicio_particion;
                            nueva_particion.part_status = '1';
                            nueva_particion.part_type = type[0];
                            nueva_particion.part_size = tam;
                            nueva_particion.part_fit = fit[0];
                            discoparticion.disco_particion[indice_libre ] = nueva_particion;
                            fseek(disco_base, 0, SEEK_SET);
                            fwrite(&discoparticion, sizeof(Estructuras::MBRdisco), 1, disco_base);
                            type != "e"?cout <<GRN<<"PARTICION PRIMARIA CREADO CON EXITO: "<< name<< CYN""<< endl: cout <<""<<endl;
                            // ebr para las extendida

                            if (type == "e") {
                                // NUEVO EBR
                                Estructuras::EBRparticion  nueva_ebr;
                                string cabecera_ebr = "H_" + to_string(discoparticion.disk_signature);
                                nueva_ebr.part_next = inicio_particion + tam - sizeof(Estructuras::EBRparticion);
                                strcpy(nueva_ebr.part_name, cabecera_ebr.c_str());
                                nueva_ebr.part_start = inicio_particion;
                                nueva_ebr.part_size = 0;
                                nueva_ebr.part_status = '1';
                                nueva_ebr.part_fit = 'w';
                                // GUARDAR
                                fseek(disco_base, inicio_particion, SEEK_SET);
                                fwrite(&nueva_ebr, sizeof(Estructuras::EBRparticion), 1, disco_base);

                                // NUEVO EBR
                                Estructuras::EBRparticion ebr_f;
                                string foot_ebr_name ="F_" + to_string(discoparticion.disk_signature);
                                strcpy(ebr_f.part_name, foot_ebr_name.c_str());
                                ebr_f.part_start =inicio_particion+ tam - sizeof(Estructuras::EBRparticion);
                                ebr_f.part_size = 0;
                                ebr_f.part_next = -1;
                                ebr_f.part_status = '1';
                                ebr_f.part_fit = 'w';
                                // GUARDAR
                                fseek(disco_base, inicio_particion + tam - sizeof(Estructuras::EBRparticion),SEEK_SET);
                                fwrite(&ebr_f, sizeof(Estructuras::EBRparticion), 1, disco_base);
                                cout <<GRN<<"PARTICION EXTENDIDA CREADO CON EXITO: "<< name<< CYN""<< endl;
                            }
                                fclose(disco_base);
                            }
                    } else{
                        cout <<RED<<"NO SE PERMITEN MAS PARTICIONES"<< CYN""<< endl;
                    }
                } else{
                    cout <<RED<<"NO EXISTE ESPACIO SUFICIENTE EN EL DISCO" << CYN""<< endl;
                }


            }
            else {
                cout <<RED<<_error << CYN""<< endl;
            }
        }
        else {
            cout <<RED<<_error << CYN""<< endl;
        }
    }
    else {
        cout <<RED "NO SE ENCOTRO EL DISCO! " << CYN""<< endl;
    }
}

void Fdisco::Agregar_espacio(int add, string unit, string path , string name)
{
    // tamaño de la particion a crear
    int tam =0;
    if(unit =="b"){
        tam= add;
    }else if(unit == "k"){
        tam= add*1024;
    }else if(unit =="m"){
        tam= (add*1024*1024);
    }

    string _error ="";
    bool bandera = false;
    //verificar si existe el disco
    Estructuras::MBRdisco discoparticion;//disco base
    FILE *disco_base = fopen(path.c_str(), "rb+");
    if(disco_base !=NULL){
        // variables para el espacio disponible y posiciones
        int inicio_particion=-1;
        int particion_extendida=-1;
        int total_particiones=0;

        // OBTENER MBR
        fseek(disco_base, 0, SEEK_SET);
        fread(&discoparticion, sizeof(Estructuras::MBRdisco), 1, disco_base);
        // obteniendo espacio para la particion
        for (int i = 0; i < 4; i++) {
            if (discoparticion.disco_particion[i].part_status == '1') {
                total_particiones++;
                // si existe particion extendida
                if (discoparticion.disco_particion[i].part_type == 'e')
                    particion_extendida = i;
                // busacar nombre
                if (discoparticion.disco_particion[i].part_name == name) {
                    bandera=true;
                    inicio_particion = i;
                }
            }
        }
        // buscando una particion logica
        int buscar_logica = -1;
        Estructuras::EBRparticion  particiones_logicas[23];
        for (int i = 0;i < 23;i++) {
            Estructuras::EBRparticion _ebr;
            _ebr.part_status = '0';
            _ebr.part_start = -1;
            _ebr.part_next = -1;
            _ebr.part_size = 0;
            particiones_logicas[i] = _ebr;
        }

        if (particion_extendida != -1 && bandera) {
            // particiones logicas cabeceras
            Estructuras::EBRparticion ebr_cabecera;
            fseek(disco_base, discoparticion.disco_particion[particion_extendida ].part_start,SEEK_SET);
            fread(&ebr_cabecera, sizeof(Estructuras::EBRparticion), 1, disco_base);
            particiones_logicas[0] = ebr_cabecera;

            // Buscar otras particiones logicas
            for (int i = 0; i < 23;i++) {
                Estructuras::EBRparticion part_logica;
                fseek(disco_base, particiones_logicas[i].part_next,SEEK_SET);
                fread(&part_logica, sizeof(Estructuras::EBRparticion), 1, disco_base);
                // Buscar nombre
                if (part_logica.part_name == name) {
                    bandera= true;
                    buscar_logica = i + 1;
                    break;
                }
                // siguente
                if (part_logica.part_status== '1')
                    particiones_logicas[i + 1] = part_logica;
            }
        }


        //verificar si ya existe la particion
        if(bandera) {
            // PARTICIONES PRIMARIAS Y EXTENDIDAS
            if (inicio_particion != -1) {
                // CALCULAR ESPACIO DISPONIBLE EN PARTICION
                int anterior_espacio = discoparticion.disco_particion[inicio_particion].part_start;
                int espacio_siguiente = discoparticion.disco_particion[inicio_particion + 1].part_start;
                int nuevo_tamanio = discoparticion.disco_particion[inicio_particion].part_size + tam;

                // VALIDAR Y GUARDAR
                if (nuevo_tamanio > 0) {
                    if ((espacio_siguiente - anterior_espacio) >= nuevo_tamanio) {
                        // ESCRIBIR MBR
                        discoparticion.disco_particion[inicio_particion].part_size = nuevo_tamanio;
                        fseek(disco_base, 0, SEEK_SET);
                        fwrite(&discoparticion, sizeof(Estructuras::MBRdisco), 1, disco_base);
                        fclose(disco_base);
                        cout <<RED "SE AÑADIO ESPACIO CON EXITO EN LA PARTICION: " <<name<< CYN""<< endl;
                    } else{
                        cout <<RED "NO HAY SUFICIENTE ESPACION EN PARTICION " << CYN""<< endl;
                    }
                } else{
                    cout <<RED "NO SE PUEDE REDUCIR MENOS QUE CERO EL TAMAÑO DE LA PARTICION: " <<name<< CYN" !"<< endl;
                }
            }
            else {
               //Para las particiones logicas
                int anterior_espacio = particiones_logicas[buscar_logica].part_start + sizeof(Estructuras::EBRparticion );
                int espacio_siguiente =particiones_logicas[buscar_logica + 1].part_start;
                int nuevo_tamanio = particiones_logicas[buscar_logica].part_size + tam;

                // validando
                if (nuevo_tamanio > 0) {
                    if (espacio_siguiente - anterior_espacio >= nuevo_tamanio) {
                       //modificando ebr
                        Estructuras::EBRparticion  particion_l = particiones_logicas[buscar_logica];
                        particion_l.part_size = nuevo_tamanio;
                        // guardando
                        fseek(disco_base, particion_l.part_start, SEEK_SET);
                        fwrite(&particion_l, sizeof(Estructuras::EBRparticion), 1, disco_base);
                        fclose(disco_base);
                        cout <<RED "SE AÑADIO ESPACIO CON EXITO EN LA PARTICION: " <<name<< CYN""<< endl;
                    } else{
                        cout <<RED "NO HAY SUFICIENTE ESPACION EN PARTICION " << CYN""<< endl;
                    }
                } else{
                    cout <<RED "NO SE PUEDE REDUCIR MENOS QUE CERO EL TAMAÑO DE LA PARTICION: " <<name<< CYN" !"<< endl;
                }
            }
        }
        else {
            cout <<RED "NO SE ENCONTRO LA PARTICION " << CYN""<< endl;
        }
    }
    else {
        cout <<RED "NO SE ENCOTRO EL DISCO! " << CYN""<< endl;
    }
}


 void Fdisco::Eliminar_particion(string eliminar, string path , string name)
{
    string _error ="";
    bool bandera = false;
    //verificar si existe el disco
    Estructuras::MBRdisco discoparticion;//disco base
    FILE *disco_base = fopen(path.c_str(), "rb+");
    if(disco_base !=NULL){
        // variables para el espacio disponible y posiciones
        int inicio_particion=-1;
        int particion_extendida=-1;
        int total_particiones=0;

        // OBTENER MBR
        fseek(disco_base, 0, SEEK_SET);
        fread(&discoparticion, sizeof(Estructuras::MBRdisco), 1, disco_base);
        // obteniendo espacio para la particion
        for (int i = 0; i < 4; i++) {
            if (discoparticion.disco_particion[i].part_status == '1') {
                total_particiones++;
                // si existe particion extendida
                if (discoparticion.disco_particion[i].part_type == 'e')
                    particion_extendida = i;
                // busacar nombre
                if (discoparticion.disco_particion[i].part_name == name) {
                    bandera=true;
                    inicio_particion = i;
                }
            }
        }
        // buscando una particion logica
        int buscar_logica = -1;
        Estructuras::EBRparticion  particiones_logicas[23];
        for (int i = 0;i < 23;i++) {
            Estructuras::EBRparticion _ebr;
            _ebr.part_status = '0';
            _ebr.part_start = -1;
            _ebr.part_next = -1;
            _ebr.part_size = 0;
            particiones_logicas[i] = _ebr;
        }

        if (particion_extendida != -1 && bandera) {
            // particiones logicas cabeceras
            Estructuras::EBRparticion ebr_cabecera;
            fseek(disco_base, discoparticion.disco_particion[particion_extendida ].part_start,SEEK_SET);
            fread(&ebr_cabecera, sizeof(Estructuras::EBRparticion), 1, disco_base);
            particiones_logicas[0] = ebr_cabecera;

            // Buscar otras particiones logicas
            for (int i = 0; i < 23;i++) {
                Estructuras::EBRparticion part_logica;
                fseek(disco_base, particiones_logicas[i].part_next,SEEK_SET);
                fread(&part_logica, sizeof(Estructuras::EBRparticion), 1, disco_base);
                // Buscar nombre
                if (part_logica.part_name == name) {
                    bandera= true;
                    buscar_logica = i + 1;
                    break;
                }
                // siguente
                if (part_logica.part_status== '1')
                    particiones_logicas[i + 1] = part_logica;
            }
        }


        //verificar si ya existe la particion
        if(bandera) {
            cout<<CYN " DESEA ELIMINAR LA PARTICION?"<<name<<endl;
            cout<<CYN"S/N?"<<endl;
            string respuesta;
            cin >> respuesta;
            if (respuesta == "s")
            {
                //borrando particiones primarias y extendidas
                if (buscar_logica == -1) {
                    Estructuras::Particion  particion_eliminiar =discoparticion.disco_particion[inicio_particion];
                    // borrar en el mbr
                    discoparticion.disco_particion[inicio_particion].part_status = '0';
                    // fast
                    int espacio_disp_part =0;
                    if( particion_eliminiar.part_type =='e'){
                        espacio_disp_part = 2 * sizeof(Estructuras::EBRparticion );
                    }
                    int tam_contenido = particion_eliminiar.part_size - espacio_disp_part;
                    char n_buff[1024];
                    for (int i = 0; i < 1024; i++){
                        n_buff[i] = '\0';
                    }
                    // borrado full
                    if (eliminar == "full" || particion_eliminiar.part_type == 'e') {
                        string nomb_ = "";
                        strcpy(discoparticion.disco_particion[inicio_particion].part_name,nomb_.c_str());
                        discoparticion.disco_particion[inicio_particion].part_fit = 'w';
                        discoparticion.disco_particion[inicio_particion].part_type = 'p';
                        discoparticion.disco_particion[inicio_particion].part_start = 0;
                        discoparticion.disco_particion[inicio_particion].part_size = 0;

                        // BORRAR CONTENIDO EN DISCO
                        fseek(disco_base, particion_eliminiar.part_start +(particion_eliminiar.part_type == 'e' ? sizeof(Estructuras::EBRparticion):0), SEEK_SET);
                        for (int i = 0; i < tam_contenido / 1024;i++){
                            fwrite(&n_buff, 1024, 1, disco_base);
                        }
                        if (particion_eliminiar.part_type == 'e') {
                            // RESTABLECER EBR INICIAL
                            Estructuras::EBRparticion  ebr_cabecera;
                            string head_ebr_name ="H_" + to_string(discoparticion.disk_signature);
                            ebr_cabecera.part_next = particion_eliminiar.part_start +  particion_eliminiar.part_size - sizeof(Estructuras::EBRparticion);
                            strcpy(ebr_cabecera.part_name, head_ebr_name.c_str());
                            ebr_cabecera.part_start = particion_eliminiar.part_start;
                            ebr_cabecera.part_size = 0;
                            ebr_cabecera.part_status = '1';
                            ebr_cabecera.part_fit = 'w';

                            // GUARDAR
                            fseek(disco_base, particion_eliminiar.part_start, SEEK_SET);
                            fwrite(&ebr_cabecera, sizeof(Estructuras::EBRparticion), 1, disco_base);

                        }
                    }
                    fseek(disco_base, 0, SEEK_SET);
                    fwrite(&discoparticion, sizeof(Estructuras::MBRdisco), 1, disco_base);
                    fclose(disco_base);
                    cout <<RED "SE ELIMINO LA PARTICION EXITOSAMENTE! " << CYN""<< endl;

                }
            }
            else{
                fclose(disco_base);
                cout <<RED "NO SE ELIMINO LA PARTICION! " << CYN""<< endl;
            }

        }
        else {
            cout <<RED "NO SE ENCONTRO LA PARTICION " << CYN""<< endl;
        }
    }
    else {
        cout <<RED "NO SE ENCOTRO EL DISCO! " << CYN""<< endl;
    }
}

int Fdisco::Ajuste(int size,Estructuras::MBRdisco disco,string ajuste){
    vector <Espacio_libre> espacios;
    vector <int> inicios;
    vector <int> bestfit;
    vector <int> worstfit;
    int espacio_inicial =sizeof (Estructuras::MBRdisco)+1;
    int espacio_final =0;
    int retornar=-1;
    //inicios.push_back(sizeof (Estructuras::MBRdisco)+1);
    // obteniendo los inicios de cada partcion usada
    for(int i =0;i<4;i++){
        if(disco.disco_particion[i].part_status == '1'){
            inicios.push_back(disco.disco_particion[i].part_start);
        }
    }

    // ordenando los inicios de cada particion
    std::sort(inicios.begin(), inicios.end());
    // obteniendo el tamaño de cada particion
    for(int i =0;i<4;i++){
        Espacio_libre  aux;
        if(disco.disco_particion[i].part_status == '1'){
            for(int j=0; j< sizeof(inicios);j++){
                if(disco.disco_particion[i].part_start == inicios[j]){
                    espacio_final = disco.disco_particion[i].part_start;
                    aux.inicio = espacio_inicial;
                    aux.final = espacio_final;
                    aux.size = espacio_final - espacio_inicial;
                    aux.libre = (espacio_final - espacio_inicial)-size;
                    bestfit.push_back((espacio_final - espacio_inicial) - size);
                    worstfit.push_back(espacio_final - espacio_inicial);
                    espacios.push_back(aux);
                    espacio_inicial = disco.disco_particion[i].part_start + disco.disco_particion[i].part_size +1;
                    break;
                }
            }
        }
        if(i==3){
            espacio_final=disco.mbr_tamano;
            aux.inicio = espacio_inicial;
            aux.final = espacio_final;
            aux.size = espacio_final - espacio_inicial;
            aux.libre = (espacio_final - espacio_inicial)-size;
            bestfit.push_back((espacio_final - espacio_inicial) - size);
            worstfit.push_back(espacio_final - espacio_inicial);
            espacios.push_back(aux);
        }
    }
    //============= PRIMER AJUSTE
    if(disco.disk_fit =='f'){
        for(int i=0;i<espacios.size();i++){
            if(espacios[i].size >=size){
                retornar = espacios[i].inicio;
                break;
            }
        }
    }
    //============== mejor auste ============
    else if(disco.disk_fit =='b'){
        std::sort(bestfit.begin(), bestfit.end());
        bool salir = false;
        for(int i=0;i<bestfit.size();i++){
            if(bestfit[i] >= 0){
                for(int j=0;j<espacios.size();j++){
                    if(espacios[j].libre == bestfit[i]){
                        retornar = espacios[j].inicio;
                        salir = true;
                        break;
                    }
                }
            }
            if(salir ){
                break;
            }
        }
    }
    //=================== PEOR AJUSTE ============
    else{
        sort(worstfit.begin(), worstfit.end());
        if(!worstfit.empty()){
            if(worstfit[worstfit.size()-1]>= size){
                for(int j=0;j<espacios.size();j++){
                    if(espacios[j].libre == worstfit[worstfit.size()-1]){
                        retornar = espacios[j].inicio;
                        break;
                    }
                }
            }
        }
    }
    return retornar;
}


/*
int Fdisco::Add(int size,Estructuras::MBRdisco disco, int index){
    vector <Espacio_libre> espacios;
    vector <int> inicios;
    vector <int> bestfit;
    vector <int> worstfit;
    int espacio_inicial =sizeof (Estructuras::MBRdisco)+1;
    int espacio_final =0;
    int retornar=-1;
    //inicios.push_back(sizeof (Estructuras::MBRdisco)+1);
    // obteniendo los inicios de cada partcion usada
    for(int i =0;i<4;i++){
        if(disco.disco_particion[i].part_status == '1'){
            inicios.push_back(disco.disco_particion[i].part_start);
        }
    }

    // ordenando los inicios de cada particion
    std::sort(inicios.begin(), inicios.end());
    // obteniendo el tamaño de cada particion
    for(int i =0;i<4;i++){
        Espacio_libre  aux;
        if(disco.disco_particion[i].part_status == '1'){
            for(int j=0; j< sizeof(inicios);j++){
                if(disco.disco_particion[i].part_start == inicios[j]){
                    espacio_final = disco.disco_particion[i].part_start;
                    aux.inicio = espacio_inicial;
                    aux.final = espacio_final;
                    aux.size = espacio_final - espacio_inicial;
                    aux.libre = (espacio_final - espacio_inicial)-size;
                    espacios.push_back(aux);
                    espacio_inicial = disco.disco_particion[i].part_start + disco.disco_particion[i].part_size +1;
                    break;
                }
            }
        }
        if(i==3){
            espacio_final=disco.mbr_tamano;
            aux.inicio = espacio_inicial;
            aux.final = espacio_final;
            aux.size = espacio_final - espacio_inicial;
            aux.libre = (espacio_final - espacio_inicial)-size;
            espacios.push_back(aux);
        }
    }
    // ============= VERIFICANDO SI HAY ESPACIO DISPONIBLE
    //============= PRIMER AJUSTE
    if(disco.disk_fit =='f'){
        for(int i=0;i<espacios.size();i++){
            if(espacios[i].size >=size){
                retornar = espacios[i].inicio;
                break;
            }
        }
    }
        //============== mejor auste ============
    else if(disco.disk_fit =='b'){
        std::sort(bestfit.begin(), bestfit.end());
        bool salir = false;
        for(int i=0;i<bestfit.size();i++){
            if(bestfit[i] >= 0){
                for(int j=0;j<espacios.size();j++){
                    if(espacios[j].libre == bestfit[i]){
                        retornar = espacios[j].inicio;
                        salir = true;
                        break;
                    }
                }
            }
            if(salir ){
                break;
            }
        }
    }
        //=================== PEOR AJUSTE ============
    else{
        sort(worstfit.begin(), worstfit.end());
        if(!worstfit.empty()){
            if(worstfit[worstfit.size()-1]>= size){
                for(int j=0;j<espacios.size();j++){
                    if(espacios[j].libre == worstfit[worstfit.size()-1]){
                        retornar = espacios[j].inicio;
                        break;
                    }
                }
            }
        }
    }
    return retornar;
}

*/