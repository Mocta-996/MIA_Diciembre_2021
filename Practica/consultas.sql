-- ============================================= CONSULTAS =============================================
-- =================== USAR ROL 
--      mysql> SET GLOBAL local_infile=1;
--      mysql> quit
--      mysql -u EMPLOY -p 123Employ
use practica;
-- ====================================== 1. ¿Cuántos proyectos iniciaron en el 2011? ================================
SELECT COUNT(*) as consulta_1
FROM projects
WHERE transactions_start_year = 2011;

-- ====================================== 2. ¿Cuál fue el proyecto de mayor duración? ================================

SELECT project_title, start_actual_isodate, end_actual_isodate,  TIMESTAMPDIFF(DAY, start_actual_isodate, end_actual_isodate) 
AS consulta_2 
FROM projects
ORDER BY consulta_2 DESC LIMIT 1;

-- ====================================== 3. ¿Cuál fue el proyecto de menor duración? ================================
SELECT project_title, start_actual_isodate, end_actual_isodate,  TIMESTAMPDIFF(DAY, start_actual_isodate, end_actual_isodate) 
AS consulta_3 
FROM projects
WHERE end_actual_isodate IS NOT NULL
ORDER BY consulta_3 ASC LIMIT 1;


-- ====================================== 4. Top 5 de países con más proyectos asignados  ================================
SELECT c.name,
    COUNT(*) as consulta_4
FROM projects as p
INNER JOIN country_codes as c
ON p.recipients =  c.id_country_codes
WHERE p.recipients IS NOT NULL
GROUP BY recipients
ORDER BY consulta_4 DESC LIMIT 5;

-- ====================================== 5. Top 5 de países con menos proyectos asignados  ================================
SELECT c.name,
    COUNT(*) as consulta_5
FROM projects as p
INNER JOIN country_codes as c
ON p.recipients =  c.id_country_codes
WHERE p.recipients IS NOT NULL
GROUP BY recipients
ORDER BY consulta_5 ASC LIMIT 5;

-- ====================================== 6. ¿Cuál fue el proyecto con más costo?  ================================
SELECT p.project_title as consulta_6, p.total_commitments as Costo
FROM projects as p
ORDER BY p.total_commitments DESC LIMIT 1;

-- ====================================== 7. ¿Cuál fue el proyecto menos costoso?  ================================
SELECT p.project_title as consulta_7 , p.total_commitments as Costo
FROM projects as p
ORDER BY p.total_commitments ASC LIMIT 1;

-- ====================================== 8. Top 5 de proyectos más costosos  ================================
SELECT p.project_title as consulta_8 , p.total_commitments as Costo
FROM projects as p
ORDER BY p.total_commitments DESC LIMIT 5;

-- ====================================== 9. Costo Total de Todos los Proyectos  ================================
SELECT '' as 'Consulta 9', SUM(total_commitments)  as Costo_total
FROM projects;

-- ====================================== 10. Top 3 de las Transacciones más Costosas RI-East Africa Public Health Laboratory Networking Project  ================================
SELECT ts.transaction_value as consulta_10
FROM transactions  as ts
INNER JOIN projects as pj
ON pj. id_projects =ts.project_id
WHERE pj.project_title = "AFCC2/RI-East Africa Public Health Laboratory Networking Project"
ORDER BY consulta_10  DESC LIMIT 3;


