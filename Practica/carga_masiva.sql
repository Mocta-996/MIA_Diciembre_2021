-- =========================================== CARGAR LOS ARCHIVOS CSV A LAS TABLAS TEMPORALES ===========================================
-- =========================================== CARGAR A LA TABLA TEMPORAL 1 =============================================
-- ===========================================         geonamess           =============================================
use practica;
SELECT '' as '1. Carga del archivo geonames.csv a la tabla geonamess_t ';

LOAD DATA  LOCAL
INFILE "/home/pilo-tuy/Escritorio/MIA VACAS/Practica/ArchivosCSV/geonames.csv"
INTO TABLE  geonamess_t
CHARACTER SET UTF8
fields terminated by ','
ENCLOSED BY '"'
LINES TERMINATED BY '\r\n'
IGNORE 1 ROWS;


-- SELECT count(*) FROM information_schema.columns WHERE table_name = 'geonamess';

-- =========================================== CARGAR A LA TABLA TEMPORAL 2 =============================================
-- ===========================================         projects           =============================================
SELECT '' as '2. Carga del archivo projects.csv  a la tabla  projects_t ';
LOAD DATA  LOCAL
INFILE "/home/pilo-tuy/Escritorio/MIA VACAS/Practica/ArchivosCSV/projects.csv"
IGNORE
INTO TABLE  projects_t 
CHARACTER SET latin1
fields terminated by ',' 
ENCLOSED BY '"'
LINES TERMINATED BY '\r\n'
IGNORE 1 ROWS
(project_id,is_geocoded ,project_title,@start_actual_isodate, @end_actual_isodate,donors, donors_iso3, recipients ,recipients_iso3 ,ad_sector_codes ,ad_sector_names ,status,transactions_start_year ,transactions_end_year ,total_commitments,total_disbursements )
SET start_actual_isodate = STR_TO_DATE(@start_actual_isodate, '%d/%m/%Y'), end_actual_isodate=STR_TO_DATE(@end_actual_isodate, '%d/%m/%Y') ; 


-- =========================================== CARGAR A LA TABLA TEMPORAL 3 =============================================
-- ===========================================         level1_a           =============================================
SELECT '' as '3. Carga del archivo level_1a.csv a la tabla  level1_a_t ';
LOAD DATA  LOCAL
INFILE "/home/pilo-tuy/Escritorio/MIA VACAS/Practica/ArchivosCSV/level_1a.csv"
IGNORE
INTO TABLE  level1_a_t 
CHARACTER SET UTF8
COLUMNS TERMINATED BY "," 
ENCLOSED BY '"'
LINES TERMINATED BY '\r\n'
IGNORE 1 ROWS;


-- =========================================== CARGAR A LA TABLA TEMPORAL 4 =============================================
-- ===========================================        transactions           =============================================
SELECT '' as '4. Carga del archivo transactions.csv a la tabla  transactions_t ';
LOAD DATA  LOCAL
INFILE "/home/pilo-tuy/Escritorio/MIA VACAS/Practica/ArchivosCSV/transactions.csv"
IGNORE
INTO TABLE  transactions_t
CHARACTER SET UTF8
COLUMNS TERMINATED BY "," 
ENCLOSED BY '"'
LINES TERMINATED BY '\r\n'
IGNORE 1 ROWS
(transaction_id, project_id,@transaction_isodate,transaction_year,transaction_value_code,transaction_currency,transaction_value)
SET transaction_isodate = STR_TO_DATE(@transaction_isodate, '%d/%m/%Y'); 

-- =========================================== CARGAR A LA TABLA TEMPORAL 5 =============================================
-- ===========================================        country_codes          =============================================
SELECT '' as '5. Carga del archivo country_codes.tsv a la tabla  country_codes_t ';
LOAD DATA  LOCAL
INFILE "/home/pilo-tuy/Escritorio/MIA VACAS/Practica/ArchivosCSV/country_codes.tsv"
IGNORE
INTO TABLE  country_codes_t
COLUMNS TERMINATED BY "\t" 
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;

-- =========================================== CARGAR A LA TABLA TEMPORAL 6 =============================================
-- ===========================================        country_codes          =============================================
SELECT '' as '6. Carga del archivo locations.csv a la tabla  locations_t ';
LOAD DATA  LOCAL
INFILE "/home/pilo-tuy/Escritorio/MIA VACAS/Practica/ArchivosCSV/locations.csv"
IGNORE
INTO TABLE  locations_t
COLUMNS TERMINATED BY "\t" 
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;
