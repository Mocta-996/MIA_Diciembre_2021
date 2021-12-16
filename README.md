# **DOCUMENTACION** 
---
### Proyecto 1 Manejo e Implementación de Archivos 2S 2021
### Universidad de San Carlos de Guatemala
### Facultad de Ingeniería 
### Manejo e implemtencación de Archivos
### Vacaciones Diciembre 2021
### Maynor Octavio Piló Tuy -201531166
---


#### Descripción:
* En este proyecto consiste en el manejo de los principales conceptos sobre la administración de archivos, tanto en hardware como software, sistemas de archivos, particiones, entre otros conceptos. Simulado en archivos binarios, Consiste en un Sistema de Archivos EXT2/EXT3.

#### Lenguajes y herramientas:
* Desarrollado en c++ 
* Desarrollo de graficas con Graphviz 
* S.O Ubuntu 20.04

#### Descripcion de la aplicación:
* La aplicación se maneja totalmente en consola, a excepción de los reportes que se generan automaticamente  en la extension deseada. no tiene menús, se maneja totalmente con comandos,no distinguie entre mayúsculas y minúsculas. la aplicación puede de leer desde standard input(stdin) y escribir a standard output. recibe parámetros obligatorios y opcionales. Solo se puede colocar un comando por línea. Si se utiliza un parámetro no especificado, se muestra un mensaje de error. Se utilizan espacios en blanco para separar cada parámetro. Cuando  algún valor lleve espacios en blanco se encerrará entre comillas " ".
* La aplicacion se divide en el manejo de archivos, que se divide en cuatro secciones:

## Administració de Discos
---
crea archivos que simularán discos duros en los que se podrá administrar más adelante con el sistema de archivos ext2 o ext3. estos comandos son:
#### MKDISK
Este comando crea un archivo binario que simulará un disco duro, estos archivos binarios se generan con la extensión .disk, y recibe los siguientes parametros. 

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -size | obligatorio | tamaño del disco a crear |
| -f | opcional | tipo de ajuste del disco |
| -u | opcional | indica las unidades para la creación del disco |
| -path | obligatorio | ruta de creacion del disco |
#### RMDISK
Este comando elimina un archivo binario que representa un disco duro. 

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -path | obligatorio | ruta del disco a eliminar |

#### FDISK
Este comando administra las particiones en el archivo que representa al disco duro.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -size | obligatorio | tamaño de la particion a crear |
| -f | opcional | tipo de ajuste del disco |
| -u | opcional | indica las unidades para la creación del disco |
| -path | obligatorio | ruta de creacion del disco |
| -type | obligatorio | Indica el tipo de partición a crear|
| -add | opcional | Indica que se eliminara una partición|
| -name | obligatorio | Indica el nombre de la partición a crear |
| -add | opcional | agregar o quitar espacio de la partición |

#### MOUNT
Este comando permite montar una partición del disco en el sistema.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -path | obligatorio | ruta del disco a eliminar |
| -path | obligatorio | indica el nombre de la particion a montar. |

#### UMOUNT
Este comando permite desmontar una partición del disco en el sistema.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -id | obligatorio | Especifica la particion a desmontar |

## Sistema de Archivos EXT2/EXT3
---
Se formatean los discos con el sistema ext2/ext3
#### MKFS
Este comando realiza un formateo completo de la partición. 

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -id | obligatorio | Indicará el id  de la particon o disco. |
| -type  | opcional | Indicará que tipo de formateo se realizará. |
| -fs | opcional | Indica el sistema de archivos a formatear. |

## Administración de Usuarios y Grupos
---
Se crean archivos .txt  llamado users.txt,que guarda el registro tanto para grupos y para usuarios. 
#### LOGIN
Este comando se utiliza para iniciar sesión en el sistema.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -user | obligatorio | Especifica el nombre del usuario que iniciará sesión. |
| -pwd  | obligatorio | Indicará la contraseña del usuario que iniciara sesión. |
| -id | obligatorio | Indicará el id de la partición montada de la cual van a iniciar sesión. |
#### LOGOUT
Este comando se utiliza para cerrar sesión. Debe haber una sesión activa anteriormente para poder utilizarlo.

#### MKGRP
Este comando crea un grupo para los usuarios de la partición.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -name | obligatorio | Indicará el nombre que tendrá el grupo. |

#### RMGRP
Este comando elimina un grupo para los usuarios de la partición.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -name | obligatorio | Indica el nombre del grupo a eliminar. |

#### MKUSER
Este comando crea un usuario en la partición. Solo lo puede ejecutar el usuario root.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -user | obligatorio | Especifica el nombre del usuario a crear. |
| -pwd  | obligatorio | Indicará la contraseña del usuario. |
| -grp | obligatorio | Indicará el grupo al que pertenece el usuario. |

#### RMUSER
Este comando elimina un usuario en la partición. Solo lo puede ejecutar el usuario root.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -user | obligatorio | Especifica el nombre del usuario a eliminar. |

## SCRIPT
---
Son archivos con los comandos definidos que podran cargarse al sistema. Tienen la extensión sh y se ejecutan con  el comando exec.

#### EXEC
Este comando crea un grupo para los usuarios de la partición.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -path | obligatorio | Especifica el nombre del script que se va a ejecutar. |

## REPORTES
---
Son los resultados de forma gráfica que se obtienen de haber ingresado los comandos en el programa. Para generar dichos reportes se debe de ingresar el siguiente comando:

#### REP
Recibe el nombre del reporte que se desea y lo generará con graphviz en una carpeta existente.

| parámetro | Categoría | Descripción |
| ------ | ------ | ------ |
| -name | obligatorio | Nombre del reporte a generar. |
| -path| obligatorio | Indica una carpeta y el nombre que tendrá el reporte. |
| -id | obligatorio | Indica el id de la partición que se utilizará. |
| -path| obligatorio | Indica una carpeta y el nombre que tendrá el reporte. |
| -ruta| Opcional | Será el nombre del archivo o carpeta del que se mostrará el reporte. |
| -root| Opcional| Este parámetro indica el índice del inodo que se utilizará como raíz para el reporte.|

