SELECT d_tempo.mes, d_tempo.ano, analise.especialidade, COUNT(*)
FROM f_analise
INNER JOIN d_tempo ON (f_analise.id_data_registo = d_tempo.id_tempo)
INNER JOIN analise ON (f_analise.id_analise = analise.num_analise)
WHERE analise.nome = 'Glicémia (mg/dL)' AND d_tempo.ano >= 2017 AND d_tempo.ano <= 2020
GROUP BY
    GROUPING SETS ((d_tempo.mes, analise.especialidade), (d_tempo.ano, analise.especialidade))
ORDER BY analise.especialidade, d_tempo.mes, d_tempo.ano;


SELECT DISTINCT d_instituicao.num_regiao, d_instituicao.num_concelho, d_tempo.dia_da_semana, d_tempo.mes, COUNT(*) as total,
                COUNT(*) / COUNT(DISTINCT (d_tempo.semana, d_tempo.dia_da_semana)) AS average_by_day,
				COUNT(*) / COUNT(DISTINCT (d_tempo.dia, d_tempo.mes)) AS average_by_month,
				COUNT(*) / COUNT(DISTINCT (d_instituicao.num_concelho, d_tempo.dia, d_tempo.mes)) AS average_by_concelho
FROM f_presc_venda
INNER JOIN d_tempo ON f_presc_venda.id_data_registo = d_tempo.id_tempo
INNER JOIN d_instituicao ON f_presc_venda.id_inst = d_instituicao.id_inst
WHERE d_tempo.trimestre = 1 AND d_tempo.ano = '2020' AND d_instituicao.num_regiao = 3
GROUP BY
    ROLLUP ((d_instituicao.num_regiao, d_instituicao.num_concelho), (d_tempo.dia_da_semana), (d_tempo.mes))
HAVING COUNT(*) > 0
ORDER BY d_instituicao.num_regiao, d_instituicao.num_concelho, d_tempo.dia_da_semana, d_tempo.mes;
