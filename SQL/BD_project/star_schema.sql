drop table if exists d_tempo cascade;
drop table if exists d_instituicao cascade;
drop table if exists f_presc_venda cascade;
drop table if exists f_analise cascade;

alter table prescricao_venda drop constraint if exists num_venda_unique;

alter table prescricao_venda add constraint num_venda_unique unique (num_venda);  -- delete l.1086 on populate.sql (check relatorio)

create table d_tempo (
    id_tempo serial not null,
    dia integer not null,
    dia_da_semana integer not null,
    semana integer not null,
    mes integer not null,
    trimestre integer not null,
    ano integer not null,
    constraint pk_d_tempo primary key(id_tempo)
);

create table d_instituicao (
    id_inst serial not null,
    nome varchar(50) not null,
    tipo varchar(11) not null,
    num_regiao serial not null,
    num_concelho serial not null,
    constraint pk_d_instituicao primary key(id_inst),
    constraint fk_d_instituicao_instituicao foreign key(nome) references instituicao(nome),
    constraint fk_d_instituicao_concelho foreign key(num_regiao, num_concelho) references concelho(num_regiao, num_concelho)
);

create table f_presc_venda (
    id_presc_venda serial not null,
    id_medico integer not null,
    num_doente integer not null,
    id_data_registo integer not null,
    id_inst integer not null,
    substancia varchar(50) not null,
    quant integer not null,
    constraint pk_f_presc_venda primary key(id_presc_venda),
    constraint fk_f_presc_venda_presc_venda foreign key(id_presc_venda) references prescricao_venda(num_venda),
    constraint fk_f_presc_venda_medico foreign key(id_medico) references medico(num_cedula),
    constraint fk_f_presc_venda_d_tempo foreign key(id_data_registo) references d_tempo(id_tempo),
    constraint fk_f_presc_venda_d_instituicao foreign key(id_inst) references d_instituicao(id_inst)
);

create table f_analise (
    id_analise serial not null,
    id_medico integer,
    num_doente integer,
    id_data_registo integer,
    id_inst integer not null,
    nome varchar(50) not null,
    quant integer not null,
    constraint pk_f_analise primary key(id_analise),
    constraint fk_f_analise_analise foreign key(id_analise) references analise(num_analise),
    constraint fk_f_analise_medico foreign key(id_medico) references medico(num_cedula),
    constraint fk_f_analise_d_tempo foreign key(id_data_registo) references d_tempo(id_tempo),
    constraint fk_f_analise_d_instituicao foreign key(id_inst) references d_instituicao(id_inst)
);
