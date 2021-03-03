-- --------------------- RI-100 -------------------------

drop trigger if exists ri_100_trigger on consulta;

create or replace function  ri_100_proc() returns trigger as $$
begin
    if (select count(*) from consulta
        where new.num_cedula = num_cedula and new.nome_instituicao = nome_instituicao and
        extract(isoyear from new.data) = extract(isoyear from data) and
        extract(week from new.data) = extract(week from data)) >= 100 then
        raise exception 'r-100: % already has 100 consultas on week %, year % in %',
        new.num_cedula, extract(week from new.data), extract(isoyear from new.data), new.nome_instituicao;
    end if;
    return new;

end;
$$ language plpgsql;

create trigger ri_100_trigger before insert or update on consulta
for each row execute procedure ri_100_proc();

-- ------------------------------------------------------

-- --------------------- RI-analise ---------------------
-- Some lines on populate.sql do not respect this RI

drop trigger if exists ri_analise_trigger on analise;

create or replace function  ri_analise_proc() returns trigger as $$
begin
    if new.num_cedula is not null and new.num_doente is not null and new.data is not null and
        (select especialidade from medico where new.num_cedula = num_cedula) != new.especialidade then
        raise exception 'r-analise: especialidade % does not match especialidade on medico', new.especialidade;
    end if;
    return new;

end;
$$ language plpgsql;

create trigger ri_analise_trigger before insert or update on analise
for each row execute procedure ri_analise_proc();

-- ------------------------------------------------------
