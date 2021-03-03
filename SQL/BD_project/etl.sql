insert into d_tempo (dia, dia_da_semana, semana, mes, trimestre, ano)
    select distinct extract(day from data) as dia,
                    extract(dow from data) as dia_da_semana,
                    extract(week from data) as semana,
                    extract(month from data) as mes,
                    extract(quarter from data) as trimestre,
                    extract(year from data) as ano
    from prescricao_venda;

insert into d_tempo(dia, dia_da_semana, semana, mes, trimestre, ano)
    select distinct extract(day from data_registo) as dia,
                    extract(dow from data_registo) as dia_da_semana,
                    extract(week from data_registo) as semana,
                    extract(month from data_registo) as mes,
                    extract(quarter from data_registo) as trimestre,
                    extract(year from data_registo) as ano
    from analise;

delete from d_tempo t_presc_venda using d_tempo t_analise
where t_presc_venda.id_tempo > t_analise.id_tempo and
      t_presc_venda.dia = t_analise.dia and
      t_presc_venda.mes = t_analise.mes and
      t_presc_venda.ano = t_analise.ano;

insert into d_instituicao(nome, tipo, num_regiao, num_concelho)
    select distinct nome, tipo, num_regiao, num_concelho
    from instituicao;

insert into f_presc_venda(id_presc_venda, id_medico, num_doente, id_data_registo, id_inst, substancia, quant)
    select distinct prescricao_venda.num_venda as id_presc_venda, num_cedula as id_medico, num_doente,
                    id_tempo as id_data_registo, id_inst, prescricao_venda.substancia, quant
    from ((d_tempo
    natural join d_instituicao)
    inner join prescricao_venda on (
        extract(day from prescricao_venda.data) = d_tempo.dia and
        extract(month from prescricao_venda.data)= d_tempo.mes and
        extract(year from prescricao_venda.data) = d_tempo.ano))
    inner join venda_farmacia on
        (venda_farmacia.num_venda = prescricao_venda.num_venda and
         venda_farmacia.inst = d_instituicao.nome);

insert into f_analise(id_analise, id_medico, num_doente, id_data_registo, id_inst, nome, quant)
    select distinct num_analise as id_analise, num_cedula as id_medico, num_doente, id_tempo as id_data_registo,
                    id_inst, analise.nome, quant
    from (d_tempo
    natural join d_instituicao)
    inner join analise on (
        extract(day from analise.data_registo) = d_tempo.dia and
        extract(month from analise.data_registo)= d_tempo.mes and
        extract(year from analise.data_registo) = d_tempo.ano and
        analise.inst = d_instituicao.nome);
