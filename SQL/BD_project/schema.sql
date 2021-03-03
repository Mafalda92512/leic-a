drop table if exists regiao cascade;
drop table if exists concelho cascade;
drop table if exists instituicao cascade;
drop table if exists medico cascade;
drop table if exists consulta cascade;
drop table if exists prescricao cascade;
drop table if exists analise cascade;
drop table if exists venda_farmacia cascade;
drop table if exists prescricao_venda cascade;


create table regiao
   (num_regiao     serial      not null unique,
    nome           varchar(8)  not null,
    num_habitantes integer     not null,
    constraint pk_regiao primary key(num_regiao),
    constraint ri_1 check(nome in ('Norte', 'Centro', 'Lisboa', 'Alentejo', 'Algarve')));

create table concelho
   (num_concelho   serial      not null,
    num_regiao	   serial      not null,
    nome           varchar(50) not null,
    num_habitantes integer	   not null,
    constraint pk_concelho primary key(num_concelho, num_regiao),
    constraint fk_concelho_regiao foreign key(num_regiao) references regiao(num_regiao));

create table instituicao
   (nome 		   varchar(50) not null unique,
    tipo 		   varchar(11) not null,
    num_regiao     serial      not null,
    num_concelho   serial      not null,
    constraint pk_instituicao primary key(nome),
    constraint fk_instituicao_concelho foreign key(num_concelho, num_regiao) references concelho(num_concelho, num_regiao),
	constraint ri_instituicao_1 check(tipo in ('farmacia', 'laboratorio', 'clinica', 'hospital')));

create table medico
   (num_cedula    serial      not null unique,
    nome		  varchar(50) not null,
    especialidade varchar(50) not null,
    constraint pk_medico primary key(num_cedula));

 create table consulta
   (num_cedula        serial	  not null,
    num_doente        serial	  not null,
    data		      date 	      not null,
    nome_instituicao  varchar(50) not null,
    constraint pk_consulta primary key(num_cedula, num_doente, data),
    constraint fk_consulta_medico foreign key(num_cedula) references medico(num_cedula),
    constraint fk_consulta_instituicao foreign key(nome_instituicao) references instituicao(nome),
    constraint ri_consulta_1 check(extract(isodow from data) not in (6, 7)),
    constraint ri_consulta_2 unique(num_doente, data, nome_instituicao));

create table prescricao
   (num_cedula   serial	     not null,
    num_doente   serial	     not null,
    data		 date 	     not null,
    substancia   varchar(50) not null,
    quant		 integer	 not null,
    constraint pk_prescricao primary key(num_cedula, num_doente, data, substancia),
    constraint fk_prescricao_consulta foreign key(num_cedula, num_doente, data) references consulta(num_cedula, num_doente, data));

create table analise
   (num_analise   serial	  not null unique,
    especialidade varchar(50) not null,
    num_cedula    integer,
    num_doente    integer,
    data		  date,
    data_registo  date        not null,
    nome 		  varchar(50) not null,
    quant		  integer	  not null,
    inst		  varchar(50) not null,
    constraint pk_analise primary key(num_analise),
    constraint fk_analise_consulta foreign key(num_cedula, num_doente, data) references consulta(num_cedula, num_doente, data),
    constraint fk_analise_instituicao foreign key(inst) references instituicao(nome),
    constraint ri_analise_aux check((num_cedula is not null and num_doente is not null and data is not null) or (num_cedula is null and num_doente is null and data is null)));

create table venda_farmacia
   (num_venda	   serial       not null unique,
    data_registo   date         not null,
    substancia	   varchar(50)  not null,
    quant		   integer	    not null,
    preco		   numeric(5,2) not null,
    inst		   varchar(50)  not null,
    constraint pk_venda_farmacia primary key(num_venda),
    constraint fk_venda_farmacia_instituicao foreign key(inst) references instituicao(nome));

create table prescricao_venda
   (num_cedula   serial      not null,
    num_doente   serial      not null,
    data		 date        not null,
    substancia   varchar(50) not null,
    num_venda	 serial      not null,
    constraint pk_prescricao_venda primary key(num_cedula, num_doente, data, substancia, num_venda),
    constraint fk_prescricao_venda_venda_farmacia foreign key(num_venda) references venda_farmacia(num_venda),
    constraint fk_prescricao_venda_prescricao foreign key(num_cedula, num_doente, data, substancia) references prescricao(num_cedula, num_doente, data, substancia));
