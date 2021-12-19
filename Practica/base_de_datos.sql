-- ========================================= CREACION DE BASE DE DATOS PRACTICA ===================================
/*create database practica;
use practica;*/
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
-- SET FOREIGN_KEY_CHECKS=0; TRUNCATE TABLE status; TRUNCATE TABLE projects;  SET FOREIGN_KEY_CHECKS=1;


--  ========================================= CREACION DE TABLAS  =================================== 

-- ===========================================    TABLA 1    ===========================================
-- =========================================== locations       ===========================================

CREATE TABLE locations (
    id_location  INT AUTO_INCREMENT PRIMARY KEY,
    location_type_code  VARCHAR(15),
    location_type_name VARCHAR(200)
);
-- ===========================================    TABLA 2    ===========================================
-- =========================================== CURRENCY       ===========================================
CREATE TABLE currency (
    id_currency INT AUTO_INCREMENT PRIMARY KEY,
    name_currency VARCHAR(10)
);
-- ===========================================    TABLA 3    ===========================================
-- ===========================================   STATUS       ===========================================
CREATE TABLE status (
    id_status INT AUTO_INCREMENT PRIMARY KEY,
    name_status VARCHAR(50)
);

-- =========================================== TABLA 4 ===========================================
-- =========================================== country_codes        ===========================================
CREATE TABLE country_codes (
    id_country_codes INT AUTO_INCREMENT PRIMARY KEY,
    name  VARCHAR(150),
    iso2 VARCHAR(10),
    name_name VARCHAR(150),
    name_aiddata_code INTEGER,
    name_aiddata_name VARCHAR(150),
    name_cow_alpha VARCHAR(10),
    name_cow_numeric INTEGER,
    name_fao_code INTEGER,
    name_fips  VARCHAR(10),
    name_geonames_id INTEGER,
    name_imf_code INTEGER,
    name_iso2 VARCHAR(10),
    name_iso3 VARCHAR(10),
    name_iso_numeric INTEGER,
    name_oecd_code INTEGER,
    name_oecd_name VARCHAR(150),
    name_un_code INTEGER,
    name_wb_code VARCHAR(150)
);

-- =========================================== TABLA 5  ===========================================
-- =========================================== GEONAME_ID        ===========================================
-- contien llave foranea de  location
CREATE TABLE geoname(
    id_geoname INT AUTO_INCREMENT PRIMARY KEY,
    location_type_code INT NOT NULL,
    geoname_id INTEGER,
    place_name VARCHAR(200),
    latitude FLOAT,
    longitude FLOAT,
    gazetteer_adm_code VARCHAR(200),
    gazetteer_adm_name VARCHAR(200),
    location_class INTEGER,
    geographic_exactness INTEGER,
    FOREIGN KEY (location_type_code) REFERENCES locations(id_location)
);
 -- location_type_code VARCHAR(50) esta es la llave foranea
--  location_type_name VARCHAR(200)

-- =========================================== TABLA  6 ===========================================
-- =========================================== PROJECTS        ===========================================
-- contiene llave foranea de country codes
-- contiene llave foranea de status
CREATE TABLE projects(
    id_projects INT AUTO_INCREMENT PRIMARY KEY,
    status  INT NOT NULL,
    recipients  INT NOT NULL,
    project_id  VARCHAR(50),
    is_geocoded INTEGER,
    project_title VARCHAR(200),
    start_actual_isodate DATETIME,
    end_actual_isodate DATETIME,
    donors VARCHAR(50),
    donors_iso3 VARCHAR(20),
    recipients_iso3 VARCHAR(20),
    ad_sector_codes VARCHAR(30),
    ad_sector_names VARCHAR(250),
    transactions_start_year INTEGER,
    transactions_end_year INTEGER,
    total_commitments FLOAT,
    total_disbursements FLOAT,
    FOREIGN KEY (status) REFERENCES status(id_status),
    FOREIGN KEY (recipients) REFERENCES country_codes(id_country_codes)
);
-- status VARCHAR(50), llave foranea
--  recipients VARCHAR(100), llave foranea

-- =========================================== TABLA 7 ===========================================
-- =========================================== LEVEL        ===========================================
-- contiene llave foranea de  geonamess 
-- contiene llave foranea de project 
CREATE TABLE level(
    id_level INT AUTO_INCREMENT PRIMARY KEY,
    project_id INT NOT NULL,
    geoname_id INT NOT NULL,
    project_location_id VARCHAR(50),
    transactions_start_year INTEGER,
    transactions_end_year INTEGER,
    even_split_commitments DECIMAL(20, 5),
    even_split_disbursements DECIMAL(20,5),
    FOREIGN KEY (project_id) REFERENCES projects(id_projects),
    FOREIGN KEY (geoname_id) REFERENCES geoname(id_geoname )
);
-- project_id  VARCHAR(50),  llave foranea
 -- geoname_id INTEGER,llave foranea 

-- =========================================== TABLA 8  ===========================================
-- ===========================================  TRANSACCION       ===========================================
-- contiene llave foranea de currency
-- contiene llave foranea de project
CREATE TABLE transactions(
    id_transactions INT AUTO_INCREMENT PRIMARY KEY,
    project_id INT NOT NULL,
    transaction_currency INT NOT NULL,
    transaction_id  VARCHAR(100),
    transaction_isodate DATETIME,
    transaction_year INTEGER,
    transaction_value_code VARCHAR(10),
    transaction_value DECIMAL(16,4),
    FOREIGN KEY (project_id) REFERENCES projects(id_projects),
    FOREIGN KEY (transaction_currency) REFERENCES currency(id_currency)
);
-- project_id VARCHAR(100), llave foranea
-- transaction_currency VARCHAR(10), llave foranea