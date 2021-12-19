#! /bin/bash
echo " ======================= UNIVERSIDAD DE SAN CARLOS DE GUATEMALA ======================= "
echo " =======================    MIA - VACACIONES DICIEMBRE 2021     ======================= "
echo " =======================               PRACTICA                 ======================= "
read -rsp $'...\n'
echo -e "\e[36m Accediento a la base de datos con el usuario:\e[34mADMIN  \e[39m"
mysql --local_infile=1 -u ADMIN -p123Admin -e "SELECT user FROM mysql.user;SHOW GRANTS FOR 'ADMIN'@'localhost';" 
read -rsp $'...\n'
echo -e "\e[36m Mostrando las bases de datos existentes  \e[39m"
mysql --local_infile=1 -u ADMIN -p123Admin  -e  "show databases"
read -rsp $'...\n'
echo -e "\e[36m Eliminando las tablas y base de datos \e[39m"
mysql --local_infile=1 -u ADMIN -p123Admin <"/home/pilo-tuy/Escritorio/MIA VACAS/Practica/eliminacion_tablas.sql"
read -rsp $'...\n'
echo -e "\e[36m Mostrando las bases de datos existentes  \e[39m "
mysql --local_infile=1 -u ADMIN -p123Admin  -e  "show databases"
read -rsp $'...\n'
echo -e "\e[36m Creando base de datos \e[34m practica  \e[39m, \e[34m tablas temporales \e[39m y \e[34m tablas ER  \e[39m"
mysql --local_infile=1 -u ADMIN -p123Admin <"/home/pilo-tuy/Escritorio/MIA VACAS/Practica/tablas_temporales.sql"
read -rsp $'...\n'
echo -e "\e[36m Carga masiva a las tablas temporales  \e[39m"
mysql --local_infile=1 -u ADMIN -p123Admin <"/home/pilo-tuy/Escritorio/MIA VACAS/Practica/carga_masiva.sql"
read -rsp $'...\n'
echo -e "\e[36m Carga de datos de las tablas temporales a las tablas ER \e[39m"
mysql --local_infile=1 -u ADMIN -p123Admin <"/home/pilo-tuy/Escritorio/MIA VACAS/Practica/carga_tablas.sql"
read -rsp $'...\n'
#----------------- consultas -------------------------------
echo -e "\e[36m Accediento a la base de datos con el usuario:\e[34mEMPLOY  \e[39m"
mysql --local_infile=1 -u ADMIN -p123Admin -e "SHOW GRANTS FOR 'EMPLOY'@'localhost'" 
read -rsp $'...\n'
echo -e "\e[36m Consultas a la base de datos\e[39m"
#mysql --local_infile=1 -u EMPLOY -p123Employ <"/home/pilo-tuy/Escritorio/MIA VACAS/Practica/consultas.sql"
read -rsp $'...\n'
echo -e "\e[92m 1. ¿Cuántos proyectos iniciaron en el 2011? \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT COUNT(*) as consulta_1
FROM projects
WHERE transactions_start_year = 2011;"

read -rsp $'...\n'
echo -e "\e[92m 2. ¿Cuál fue el proyecto de mayor duración? \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT project_title, start_actual_isodate, end_actual_isodate,  TIMESTAMPDIFF(DAY, start_actual_isodate, end_actual_isodate) 
AS consulta_2 
FROM projects
ORDER BY consulta_2 DESC LIMIT 1;"

read -rsp $'...\n'
echo -e "\e[92m 3. ¿Cuál fue el proyecto de menor duración? \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT project_title, start_actual_isodate, end_actual_isodate,  TIMESTAMPDIFF(DAY, start_actual_isodate, end_actual_isodate) 
AS consulta_3 
FROM projects
WHERE end_actual_isodate IS NOT NULL
ORDER BY consulta_3 ASC LIMIT 1;"

read -rsp $'...\n'
echo -e "\e[92m 4. Top 5 de países con más proyectos asignados \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT c.name,
    COUNT(*) as consulta_4
FROM projects as p
INNER JOIN country_codes as c
ON p.recipients =  c.id_country_codes
WHERE p.recipients IS NOT NULL
GROUP BY recipients
ORDER BY consulta_4 DESC LIMIT 5;"

read -rsp $'...\n'
echo -e "\e[92m 5. Top 5 de países con menos proyectos asignados  \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT c.name,
    COUNT(*) as consulta_5
FROM projects as p
INNER JOIN country_codes as c
ON p.recipients =  c.id_country_codes
WHERE p.recipients IS NOT NULL
GROUP BY recipients
ORDER BY consulta_5 ASC LIMIT 5;"

read -rsp $'...\n'
echo -e "\e[92m 6. ¿Cuál fue el proyecto con más costo?   \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT p.project_title as consulta_6, p.total_commitments as Costo
FROM projects as p
ORDER BY p.total_commitments DESC LIMIT 1;"

read -rsp $'...\n'
echo -e "\e[92m 7. ¿Cuál fue el proyecto menos costoso?    \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT p.project_title as consulta_7 , p.total_commitments as Costo
FROM projects as p
ORDER BY p.total_commitments ASC LIMIT 1;"

read -rsp $'...\n'
echo -e "\e[92m 8. Top 5 de proyectos más costosos     \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT p.project_title as consulta_8 , p.total_commitments as Costo
FROM projects as p
ORDER BY p.total_commitments DESC LIMIT 5;"

read -rsp $'...\n'
echo -e "\e[92m 9. Costo Total de Todos los Proyectos    \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT '' as 'Consulta 9', SUM(total_commitments)  as Costo_total
FROM projects;"

read -rsp $'...\n'
echo -e "\e[92m 10. Top 3 de las Transacciones más Costosas RI-East Africa Public Health Laboratory Networking Project    \e[39m"
mysql --local_infile=1 -u EMPLOY -p123Employ practica -e "SELECT ts.transaction_value as consulta_10
FROM transactions  as ts
INNER JOIN projects as pj
ON pj. id_projects =ts.project_id
WHERE pj.project_title = 'AFCC2/RI-East Africa Public Health Laboratory Networking Project'
ORDER BY consulta_10  DESC LIMIT 3;"
read -rsp $'...\n'
echo -e "\e[36m Fin... \e[39m"