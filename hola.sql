DROP VIEW sigad;
-------------------------------
CREATE VIEW VW_EXPEDIENTES AS
SELECT 
    ANOEXPEDI AS AÑO, 
    CODI_ADUA AS ADUANA, 
    OFIC_REC, 
    AREAEJEC,
    TO_CHAR(FD_INREC, 'MM') AS MES,
    NROEXPEDI AS EXP, 
    PROCEDIM,
    FD_INREC AS FECHA_EXPEDIENTE, 
    FD_TEREC AS FECHA_CONCLUSION,
    USER_DIGIC,

    TO_CHAR(TRUNC(FD_TEREC - FD_INREC)) || ' - ' ||
    TO_CHAR(TRUNC(MOD(FD_TEREC - FD_INREC, 1) * 24)) || ':' ||
    TO_CHAR(TRUNC(MOD(FD_TEREC - FD_INREC, 1) * 1440) - TRUNC(MOD(FD_TEREC - FD_INREC, 1) * 24) * 60) || ':' ||
    TO_CHAR(TRUNC(MOD(FD_TEREC - FD_INREC, 1) * 86400) - TRUNC(MOD(FD_TEREC - FD_INREC, 1) * 1440) * 60) AS "DD - HH:MI:SS"

    (FD_TEREC - FD_INREC) * 24 AS TOTALENHORAS,

    (FD_TEREC - FD_INREC) AS TOTALENDIAS

FROM 
    (
    SELECT 
        A.ANOEXPEDI, 
        A.OFIC_REC, 
        A.CODI_ADUA, 
        A.AREAEJEC,
        A.NROEXPEDI, 
        A.PROCEDIM, 
        A.FECEXP, 
        A.TIMEEXP,
        A.FECH_CONC, 
        A.TIM_DIGIC, 
        USER_DIGIC, 
        
        TO_TIMESTAMP(
            SUBSTR(TO_CHAR(A.FECEXP), 1, 4) || '/' || 
            SUBSTR(TO_CHAR(A.FECEXP), 5, 2) || '/' || 
            SUBSTR(TO_CHAR(A.FECEXP), 7, 2) || ' ' || A.TIMEEXP, 
            'YYYY/MM/DD HH24:MI:SS'
        ) AS FD_INREC,

        TO_TIMESTAMP(
            SUBSTR(TO_CHAR(A.FECH_CONC), 1, 4) || '/' || 
            SUBSTR(TO_CHAR(A.FECH_CONC), 5, 2) || '/' || 
            SUBSTR(TO_CHAR(A.FECH_CONC), 7, 2) || ' ' || A.TIM_DIGIC, 
            'YYYY/MM/DD HH24:MI:SS'
        ) AS FD_TEREC

    FROM 
        EXPEDI A 
    WHERE 
        A.ANOEXPEDI IN ('2024', '2025')
        AND A.PROCEDIM IN ('2301', '3068')
        AND A.CODI_ADUA = '172'
        AND A.FECH_CONC > 0
    )
WHERE 
    FD_INREC >= (SYSDATE - INTERVAL '90' DAY)
ORDER BY 
    TO_CHAR(FD_INREC, 'MM');
-------------------
SELECT 
    COUNT(*) AS TOTAL_EXPEDIENTES,
    COUNT(CASE WHEN HORAS_TRANSCURRIDAS < 96 THEN 1 END) AS EXPEDIENTES_ACEPTADOS,
    ROUND(
        (COUNT(CASE WHEN HORAS_TRANSCURRIDAS < 96 THEN 1 END) * 100.0) / COUNT(*), 
        2
    ) AS PORCENTAJE_ACEPTADOS
FROM 
    VW_EXPEDIENTES;
