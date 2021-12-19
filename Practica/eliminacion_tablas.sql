-- ========================================= CREACION DE BASE DE DATOS PRACTICA ===================================
use practica;

-- ========================================== ELIMINACION DE TABLAS TEMPORALES ==================================
DROP TABLE IF EXISTS geonamess_t;
DROP TABLE IF EXISTS projects_t;
DROP TABLE IF EXISTS level1_a_t;
DROP TABLE IF EXISTS transactions_t;
DROP TABLE IF EXISTS country_codes_t;
DROP TABLE IF EXISTS locations_t;

--  ========================================= ELIMINACION DE TABLAS =================================== 
SET FOREIGN_KEY_CHECKS=0; 
DROP TABLE IF EXISTS locations CASCADE;
DROP TABLE IF EXISTS currency CASCADE;
DROP TABLE IF EXISTS status CASCADE ;
DROP TABLE IF EXISTS country_codes CASCADE; 
DROP TABLE IF EXISTS geoname CASCADE;
DROP TABLE IF EXISTS projects CASCADE;
DROP TABLE IF EXISTS level CASCADE;
DROP TABLE IF EXISTS transactions CASCADE;
SET FOREIGN_KEY_CHECKS=1;
DROP database IF EXISTS practica;
