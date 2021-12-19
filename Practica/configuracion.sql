/*  ADMINISTRADOR: ADMIN
    CONSTRASEÑA: 123Admin
    EMPLEADO:   EMPLOY
    CONSTRASEÑA: 123Employ

    CREATE USER 'ADMIN'@'localhost' IDENTIFIED BY '123Admin';
    GRANT ALL PRIVILEGES ON * . * TO 'ADMIN'@'localhost';

    CREATE USER 'EMPLOY'@'localhost' IDENTIFIED BY '123Employ';
    GRANT SELECT ON * . * TO 'EMPLOY'@'localhost';
    SHOW GRANTS FOR 'ADMIN'@'localhost';
    96.cuenta.uno@gmail.com
    cuenta_ORACLE_2021

    ejecutar un script en mysql
     mysql --local_infile=1 -u ADMIN -p 123Admin

    // iniciar sesion para cargar archivos csv
    mysql> SET GLOBAL local_infile=1;
    mysql> quit
    mysql --local_infile=1 -u ADMIN -p

    source /home/pilo-tuy/Escritorio/MIA VACAS/Practica/tablas_temporales.sql;
    source /home/pilo-tuy/Escritorio/MIA VACAS/Practica/carga_masiva.sql;
    source /home/pilo-tuy/Escritorio/MIA VACAS/Practica/base_de_datos.sql;
    source /home/pilo-tuy/Escritorio/MIA VACAS/Practica/carga_tablas.sql;
    source /home/pilo-tuy/Escritorio/MIA VACAS/Practica/consultas.sql;
    source /home/pilo-tuy/Escritorio/MIA VACAS/Practica/archivo.sql;
*/
SELECT COUNT(*) FROM locations;
SELECT table_name, table_rows
    FROM INFORMATION_SCHEMA.TABLES
    WHERE TABLE_SCHEMA = practica;
