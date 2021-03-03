SELECT concelho.num_concelho AS concelho, concelho.num_regiao AS regiao
FROM ((concelho
INNER JOIN instituicao ON concelho.num_concelho =  instituicao.num_concelho AND concelho.num_regiao =  instituicao.num_regiao)
INNER JOIN venda_farmacia ON instituicao.nome = venda_farmacia.inst)
WHERE venda_farmacia.data_registo = CURRENT_DATE
GROUP BY concelho.num_concelho, concelho.num_regiao
HAVING COUNT(*) >= ALL (
    SELECT COUNT(*) AS count
    FROM ((concelho
    INNER JOIN instituicao ON concelho.num_concelho =  instituicao.num_concelho AND concelho.num_regiao =  instituicao.num_regiao)
    INNER JOIN venda_farmacia ON instituicao.nome = venda_farmacia.inst)
    WHERE venda_farmacia.data_registo = CURRENT_DATE
    GROUP BY concelho.num_concelho, concelho.num_regiao);


SELECT regiao.nome AS regiao, medico.num_cedula AS cedula
FROM ((((regiao
INNER JOIN instituicao ON regiao.num_regiao =  instituicao.num_regiao)
INNER JOIN consulta ON instituicao.nome = consulta.nome_instituicao)
INNER JOIN prescricao ON consulta.num_cedula = prescricao.num_cedula AND consulta.num_doente = prescricao.num_doente AND consulta.data = prescricao.data)
INNER JOIN medico ON prescricao.num_cedula = medico.num_cedula)
WHERE prescricao.data >= '2019-01-01' AND prescricao.data <= '2019-06-30'
GROUP BY regiao.nome, medico.num_cedula
HAVING (regiao.nome, COUNT(*)) IN (
    SELECT regiao, MAX(num_prescricoes) AS max_prescricoes
	FROM (
        SELECT regiao.nome AS regiao, medico.nome AS medico, COUNT(*) AS num_prescricoes
        FROM ((((regiao
        INNER JOIN instituicao ON regiao.num_regiao =  instituicao.num_regiao)
        INNER JOIN consulta ON instituicao.nome = consulta.nome_instituicao)
        INNER JOIN prescricao ON consulta.num_cedula = prescricao.num_cedula AND consulta.num_doente = prescricao.num_doente AND consulta.data = prescricao.data)
        INNER JOIN medico ON prescricao.num_cedula = medico.num_cedula)
        WHERE prescricao.data >= '2019-01-01' AND prescricao.data <= '2019-06-30'
        GROUP BY regiao.nome, medico.num_cedula
        HAVING COUNT(*) > 0) child
	    GROUP BY regiao);


SELECT medico.num_cedula AS cedula
FROM (((((((regiao
INNER JOIN concelho ON regiao.num_regiao = concelho.num_regiao)
INNER JOIN instituicao ON concelho.num_regiao =  instituicao.num_regiao AND concelho.num_concelho = instituicao.num_concelho)
INNER JOIN consulta ON instituicao.nome = consulta.nome_instituicao)
INNER JOIN prescricao ON consulta.num_cedula = prescricao.num_cedula AND consulta.num_doente = prescricao.num_doente AND consulta.data = prescricao.data)
INNER JOIN prescricao_venda ON prescricao.num_cedula = prescricao_venda.num_cedula AND prescricao.num_doente = prescricao_venda.num_doente AND prescricao.data = prescricao_venda.data)
INNER JOIN medico ON prescricao_venda.num_cedula = medico.num_cedula)
INNER JOIN venda_farmacia ON prescricao_venda.num_venda = venda_farmacia.num_venda)
WHERE prescricao_venda.substancia = 'Aspirina' AND concelho.nome = 'Arouca'
GROUP BY medico.num_cedula
HAVING COUNT(DISTINCT venda_farmacia.inst) = ALL (
    SELECT COUNT(*)
    FROM (concelho
    INNER JOIN instituicao ON concelho.num_regiao =  instituicao.num_regiao AND concelho.num_concelho = instituicao.num_concelho)
    WHERE concelho.nome = 'Arouca' AND instituicao.tipo = 'farmacia');


SELECT DISTINCT analise.num_doente AS doente
FROM (analise
INNER JOIN prescricao ON analise.num_cedula = prescricao.num_cedula AND analise.num_doente = prescricao.num_doente AND analise.data = prescricao.data)
WHERE (prescricao.num_cedula, prescricao.num_doente, prescricao.data, prescricao.substancia) NOT IN (
    SELECT num_cedula, num_doente, data, substancia
    FROM prescricao_venda) AND EXTRACT(MONTH from prescricao.data) = EXTRACT(MONTH from CURRENT_DATE);
