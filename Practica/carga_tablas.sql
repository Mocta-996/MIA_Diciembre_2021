-- =================================================  VERIFICAR DATOS REPETIDOS ==============================
-- ======================== 1. insercion en tabla locations =================
use practica;
SELECT '' as '1. Carga de datos a la tabla locations ';

INSERT INTO locations(location_type_code, location_type_name)
SELECT location_type_code, location_type_name
FROM locations_t
GROUP BY location_type_code, location_type_name;
SELECT COUNT(*) FROM locations;
-- ======================== 2. insercion en tabla currency =================
SELECT '' as '2. Carga de datos a la tabla currency ';
INSERT INTO currency(name_currency)
SELECT transaction_currency 
FROM transactions_t
WHERE transaction_currency IS NOT NULL
GROUP BY transaction_currency;
SELECT COUNT(*) FROM currency;

-- ======================== 3. status  =================
SELECT '' as '3. Carga de datos a la tabla status ';
INSERT INTO status(name_status)
SELECT status 
FROM projects_t
WHERE status  IS NOT NULL
GROUP BY status ;
SELECT COUNT(*) FROM status;

-- ======================== 4. country_codes =================
SELECT '' as '4. Carga de datos a la tabla country_codes ';
INSERT INTO country_codes (name,iso2,name_name,name_aiddata_code,name_aiddata_name, name_cow_alpha,name_cow_numeric,name_fao_code,name_fips,name_geonames_id,
name_imf_code,name_iso2,name_iso3,name_iso_numeric,name_oecd_code,name_oecd_name,name_un_code,name_wb_code)
SELECT  name,iso2, name_name, name_aiddata_code , name_aiddata_name, name_cow_alpha,name_cow_numeric ,name_fao_code , name_fips, 
name_geonames_id, name_imf_code, name_iso2, name_iso3, name_iso_numeric, name_oecd_code, name_oecd_name,name_un_code,name_wb_code
FROM country_codes_t
WHERE name IS NOT NULL;
SELECT COUNT(*) FROM  country_codes;

-- ======================== 5. geoname =================
SELECT '' as '5. Carga de datos a la tabla geoname  ';
INSERT INTO geoname (location_type_code,geoname_id,place_name,latitude,longitude,gazetteer_adm_code,gazetteer_adm_name, location_class,geographic_exactness)
SELECT  (SELECT loc.id_location FROM locations loc WHERE loc.location_type_code = geonamess_t.location_type_code OR loc.location_type_name = geonamess_t.location_type_name  LIMIT 1),
geoname_id,place_name,latitude, longitude, gazetteer_adm_code, gazetteer_adm_name,location_class,geographic_exactness
FROM geonamess_t
WHERE geoname_id IS NOT NULL;
SELECT COUNT(*) FROM  geoname;

-- ======================== 6 projects  =================
SELECT '' as '6. Carga de datos a la tabla projects  ';
INSERT INTO projects (status, recipients,project_id, is_geocoded,project_title, start_actual_isodate,end_actual_isodate,donors, 
donors_iso3, recipients_iso3,ad_sector_codes,ad_sector_names , transactions_start_year, transactions_end_year, total_commitments, total_disbursements )
SELECT  (SELECT stat.id_status FROM status stat WHERE stat.name_status=projects_t.status LIMIT 1),
        (SELECT cc.id_country_codes FROM country_codes cc WHERE cc.name_iso3 = projects_t.recipients_iso3 OR cc.name  =  projects_t.recipients LIMIT 1),
        project_id, is_geocoded, project_title, start_actual_isodate, end_actual_isodate, donors, donors_iso3,
        recipients_iso3,ad_sector_codes,ad_sector_names , transactions_start_year, transactions_end_year, total_commitments, total_disbursements
FROM projects_t ;
SELECT COUNT(*) FROM  projects_t;

SELECT '' as '7. Carga de datos a la tabla level  ';
-- ======================== level  =================
INSERT INTO level (project_id,geoname_id,project_location_id,transactions_start_year,transactions_end_year,even_split_commitments,even_split_disbursements)
SELECT  id_projects,id_geoname, project_location_id,transactions_t_start_year,transactions_t_end_year ,even_split_commitments,even_split_disbursements
    FROM level1_a_t                                 
    INNER JOIN projects ON projects.project_id  =  ( SELECT project_id FROM level1_a_t   LIMIT 1)
    INNER JOIN geoname ON geoname.geoname_id  =  ( SELECT geoname_id FROM level1_a_t   LIMIT 1)  
    GROUP BY id_projects,id_geoname, project_location_id,transactions_t_start_year,transactions_t_end_year ,even_split_commitments,even_split_disbursements;
SELECT COUNT(*) FROM  level;


-- ========================transactions   =================
SELECT '' as '8. Carga de datos a la tabla transactions ';
INSERT INTO transactions (project_id,transaction_currency,transaction_id,transaction_isodate,transaction_year,transaction_value_code,transaction_value  )
SELECT 
(SELECT pj.id_projects FROM projects pj WHERE pj.project_id = transactions_t.project_id LIMIT 1),
(SELECT cr.id_currency FROM currency cr WHERE cr.name_currency = transactions_t.transaction_currency LIMIT 1),
transaction_id,transaction_isodate,transaction_year,transaction_value_code,transaction_value 
FROM transactions_t ;
SELECT COUNT(*) FROM  transactions;

/*

CREATE TEMPORARY TABLE projects_temp
    (
    campo_1 VARCHAR(250) NULL,
    campo_2 int(11) NULL,
    campo_3 VARCHAR(250) NULL,
    campo_4 VARCHAR(250) NULL,
    campo_5 VARCHAR(250) NULL,
    campo_6 VARCHAR(250) NULL,
    campo_7 VARCHAR(250) NULL,
    campo_8 VARCHAR(250) NULL,
    campo_9 VARCHAR(250) NULL,
    campo_10 VARCHAR(250) NULL,
    campo_11 VARCHAR(250) NULL,
    campo_12 VARCHAR(250) NULL,
    campo_13 int(11) NULL,
    campo_14 int(11) NULL,
    campo_15 decimal(16,5) NULL,
    campo_16 decimal(16,5) NULL
    );
    INSERT IGNORE INTO Status(Status.Name)
    SELECT projects_temp.campo_12
    FROM projects_temp;
    INSERT IGNORE INTO project(project.project_id,
                            project.is_geocoded,
                            project.project_title,
                            project.start_actual_isodate,
                            project.end_actual_isodate,
                            project.donors,
                            project.donors_iso3,
                            project.recipients,
                            project.recipients_iso3,
                            project.ad_sector_codes,
            
    project.ad_sector_names,
                            project.status,
                            
                            project.transactions_start_year,
                            project.transactions_end_year,
                            project.total_commitments,
                            project.total_disbursements)
        SELECT projects_temp.campo_1,
    projects_temp.campo_2,
            projects_temp.campo_3,
            date_format(str_to_date(projects_temp.campo_4, '%d/%m/%Y'), '%Y%m%d'),
            CASE  WHEN projects_temp.campo_5 IS NULL OR projects_temp.campo_5 = '' THEN NULL ELSE date_format(str_to_date(projects_temp.campo_5, '%d/%m/%Y'), '%Y%m%d') END,
            projects_temp.campo_6,
    projects_temp.campo_7,
            (SELECT c.id FROM countryCodes c WHERE c.name_iso3 = projects_temp.campo_9 OR c.name = projects_temp.campo_8  LIMIT 1),
            projects_temp.campo_9,
            projects_temp.campo_10,
    projects_temp.campo_11,
            (SELECT s.idStatus FROM Status s WHERE projects_temp.campo_12=s.Name),
            projects_temp.campo_13,
            projects_temp.campo_14,
            projects_temp.campo_15,
            projects_temp.campo_16
    FROM projects_temp;
 */