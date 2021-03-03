#!/usr/bin/python3

from wsgiref.handlers import CGIHandler
from flask import Flask
from flask import render_template, request, url_for, redirect

## Libs postgres
import psycopg2
import psycopg2.extras

app = Flask(__name__)
app.secret_key = "r:!X&_v[}u9@=yRYQx2O}-OkVQikeF"

## SGBD configs
DB_HOST="db.tecnico.ulisboa.pt"
DB_USER="ist192544"
DB_DATABASE=DB_USER
DB_PASSWORD="zxet4082"
DB_CONNECTION_STRING = "host=%s dbname=%s user=%s password=%s" % (DB_HOST, DB_DATABASE, DB_USER, DB_PASSWORD)

## Build pages
@app.route('/index')
@app.route('/')
def index():
    return render_template("index.html")


@app.route('/instmedic')
def instmedic():
    return render_template("instmedic.html")


@app.route('/iedmedico', methods=["GET", "POST"])
def iedmedico():
    dbConn = None
    cursor = None

    try:
        if request.method == "POST":
            if request.args.get("num_cedula") is None:
                op = request.form["op"]
                num_cedula = request.form["num_cedula"]

                return redirect(url_for('iedmedico', op=op, num_cedula=num_cedula, opnotdone=True))

            else:
                op = request.args.get("op")
                num_cedula = request.args.get("num_cedula")

                if op == "i":
                    nome_medico = request.form["nome_medico"]
                    especialidade = request.form["especialidade"]

                    return redirect(url_for('iedmedico', op=op, num_cedula=num_cedula, nome_medico=nome_medico, especialidade=especialidade))

                elif op == "e":
                    nome_medico = request.form["nome_medico"]
                    especialidade = request.form["especialidade"]

                    if nome_medico != "" and especialidade != "":
                        return redirect(url_for('iedmedico', op=op, num_cedula=num_cedula, nome_medico=nome_medico, especialidade=especialidade))

                    elif nome_medico != "":
                        return redirect(url_for('iedmedico', op=op, num_cedula=num_cedula, nome_medico=nome_medico))

                    elif especialidade != "":
                        return redirect(url_for('iedmedico', op=op, num_cedula=num_cedula, especialidade=especialidade))

                elif op == "d":
                    return redirect(url_for('iedmedico', op=op, num_cedula=num_cedula))

                else:
                    return render_template("iedmedico.html", params=request.args)

        if request.method == "GET":
            if request.args.get("num_cedula") is None:
                return render_template("iedmedico.html", params=request.args)

            elif request.args.get("opnotdone") is not None:
                return render_template("iedmedico.html", params=request.args)

            else:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                num_cedula = request.args.get("num_cedula")

                if request.args.get("op") == "i":
                    nome_medico = request.args.get("nome_medico")
                    especialidade = request.args.get("especialidade")

                    query = ("insert into medico values(%s, %s, %s);")

                    cursor.execute(query, (num_cedula, nome_medico, especialidade,))

                elif request.args.get("op") == "e":
                    nome_medico = request.args.get("nome_medico")
                    especialidade = request.args.get("especialidade")

                    if request.args.get("nome_medico") is not None:
                        query = ("UPDATE medico SET nome = %s WHERE num_cedula = %s;")

                        cursor.execute(query, (nome_medico, num_cedula,))

                    if request.args.get("especialidade") is not None:
                        query = ("UPDATE medico SET especialidade = %s WHERE num_cedula = %s;")

                        cursor.execute(query, (especialidade, num_cedula,))

                elif request.args.get("op") == "d":
                    query = ("DELETE FROM medico WHERE num_cedula = %s;")

                    cursor.execute(query, (num_cedula,))

                return render_template("iedmedico.html", cursor=cursor, params=request.args)

    except Exception as e:
        return str(e)

    finally:
        if request.method == "GET" and request.args.get("num_cedula") is not None and request.args.get("opnotdone") is None:
            dbConn.commit()
            cursor.close()
            dbConn.close()


@app.route('/iedinstituicao', methods=["GET", "POST"])
def iedinstituicao():
    dbConn = None
    cursor = None

    try:
        if request.method == "POST":
            if request.args.get("nome_inst") is None:
                op = request.form["op"]
                nome_inst = request.form["nome_inst"]

                return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, opnotdone=True))

            else:
                op = request.args.get("op")
                nome_inst = request.args.get("nome_inst")

                if op == "i":
                    tipo = request.form["tipo"]
                    num_reg = request.form["num_reg"]
                    num_conc = request.form["num_conc"]

                    return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, tipo=tipo, num_reg=num_reg, num_conc=num_conc))

                elif op == "e":
                    tipo = request.form["tipo"]
                    num_reg = request.form["num_reg"]
                    num_conc = request.form["num_conc"]

                    if tipo != "":
                        if num_reg != "":
                            if num_conc != "":
                                return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, tipo=tipo, num_reg=num_reg, num_conc=num_conc))

                            else:
                                return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, tipo=tipo, num_reg=num_reg))

                        else:
                            if num_conc != "":
                                return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, tipo=tipo, num_conc=num_conc))

                            else:
                                return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, tipo=tipo))

                    else:
                        if num_reg != "":
                            if num_conc != "":
                                return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, num_reg=num_reg, num_conc=num_conc))

                            else:
                                return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, num_reg=num_reg))
                        else:
                            if num_conc != "":
                                return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst, num_conc=num_conc))

                elif op == "d":
                    return redirect(url_for('iedinstituicao', op=op, nome_inst=nome_inst))

                else:
                    return render_template("iedinstituicao.html", params=request.args)

        if request.method == "GET":
            if request.args.get("nome_inst") is None:
                return render_template("iedinstituicao.html", params=request.args)

            elif request.args.get("opnotdone") is not None:
                return render_template("iedinstituicao.html", params=request.args)

            else:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                nome_inst = request.args.get("nome_inst")

                if request.args.get("op") == "i":
                    tipo = request.args.get("tipo")
                    num_reg = request.args.get("num_reg")
                    num_conc = request.args.get("num_conc")

                    query = ("insert into instituicao values(%s, %s, %s, %s);")

                    cursor.execute(query, (nome_inst, tipo, num_reg, num_conc,))

                elif request.args.get("op") == "e":
                    tipo = request.args.get("tipo")
                    num_reg = request.args.get("num_reg")
                    num_conc = request.args.get("num_conc")

                    if request.args.get("tipo") is not None:
                        query = ("UPDATE instituicao SET tipo = %s WHERE nome = %s;")

                        cursor.execute(query, (tipo, nome_inst,))

                    if request.args.get("num_reg") is not None:
                        query = ("UPDATE instituicao SET num_regiao = %s WHERE nome = %s;")

                        cursor.execute(query, (num_reg, nome_inst,))

                    if request.args.get("num_conc") is not None:
                        query = ("UPDATE instituicao SET num_concelho = %s WHERE nome = %s;")

                        cursor.execute(query, (num_conc, nome_inst,))

                elif request.args.get("op") == "d":
                    query = ("DELETE FROM instituicao WHERE nome = %s;")

                    cursor.execute(query, (nome_inst,))

                return render_template("iedinstituicao.html", cursor=cursor, params=request.args)

    except Exception as e:
        return str(e)

    finally:
        if request.method == "GET" and request.args.get("nome_inst") is not None and request.args.get("opnotdone") is None:
            dbConn.commit()
            cursor.close()
            dbConn.close()


@app.route('/prescanalysis')
def prescanalysis():
    return render_template("prescanalysis.html")


@app.route('/iedprescricao', methods=["GET", "POST"])
def iedprescricao():
    dbConn = None
    cursor = None

    try:
        if request.method == "POST":
            if request.args.get("num_cedula") is None:
                op = request.form["op"]
                num_cedula = request.form["num_cedula"]
                num_doente = request.form["num_doente"]
                data = request.form["data"]
                substancia = request.form["substancia"]

                return redirect(url_for('iedprescricao', op=op, num_cedula=num_cedula, num_doente=num_doente, data=data, substancia=substancia, opnotdone=True))

            else:
                op = request.args.get("op")
                num_cedula = request.args.get("num_cedula")
                num_doente = request.args.get("num_doente")
                data = request.args.get("data")
                substancia = request.args.get("substancia")

                if op == "i":
                    quant = request.form["quant"]

                    return redirect(url_for('iedprescricao', op=op, num_cedula=num_cedula, num_doente=num_doente, data=data, substancia=substancia, quant=quant))

                elif op == "e":
                    quant = request.form["quant"]

                    if quant != "":
                        return redirect(url_for('iedprescricao', op=op, num_cedula=num_cedula, num_doente=num_doente, data=data, substancia=substancia, quant=quant))

                elif op == "d":
                    return redirect(url_for('iedprescricao', op=op, num_cedula=num_cedula, num_doente=num_doente, data=data, substancia=substancia))

                else:
                    return render_template("iedprescricao.html", params=request.args)

        if request.method == "GET":
            if request.args.get("num_cedula") is None:
                return render_template("iedprescricao.html", params=request.args)

            elif request.args.get("opnotdone") is not None:
                return render_template("iedprescricao.html", params=request.args)

            else:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                op = request.args.get("op")
                num_cedula = request.args.get("num_cedula")
                num_doente = request.args.get("num_doente")
                data = request.args.get("data")
                substancia = request.args.get("substancia")

                if request.args.get("op") == "i":
                    quant = request.args.get("quant")

                    query = ("insert into prescricao values(%s, %s, %s, %s, %s);")

                    cursor.execute(query, (num_cedula, num_doente, data, substancia, quant,))

                elif request.args.get("op") == "e":
                    quant = request.args.get("quant")

                    if request.args.get("quant") is not None:
                        query = ("UPDATE prescricao SET quant = %s WHERE num_cedula = %s and num_doente = %s and data = %s and substancia = %s;")

                        cursor.execute(query, (quant, num_cedula, num_doente, data, substancia,))

                elif request.args.get("op") == "d":
                    query = ("DELETE FROM prescricao WHERE num_cedula = %s and num_doente = %s and data = %s and substancia = %s;")

                    cursor.execute(query, (num_cedula, num_doente, data, substancia,))

                return render_template("iedprescricao.html", cursor=cursor, params=request.args)

    except Exception as e:
        return str(e)

    finally:
        if request.method == "GET" and request.args.get("num_cedula") is not None and request.args.get("opnotdone") is None:
            dbConn.commit()
            cursor.close()
            dbConn.close()


@app.route('/iedanalise', methods=["GET", "POST"])
def iedanalise():
    dbConn = None
    cursor = None

    try:
        if request.method == "POST":
            if request.args.get("num_analise") is None:
                op = request.form["op"]
                num_analise = request.form["num_analise"]
                return redirect(url_for('iedanalise', op=op, num_analise=num_analise, opnotdone=True))

            else:
                op = request.args.get("op")
                num_analise = request.args.get("num_analise")

                if op == "i":
                    especialidade = request.form["especialidade"]
                    num_cedula = request.form["num_cedula"]
                    num_doente = request.form["num_doente"]
                    data = request.form["data"]
                    data_reg = request.form["data_reg"]
                    nome = request.form["nome"]
                    quant = request.form["quant"]
                    inst = request.form["inst"]

                    return redirect(url_for('iedanalise', op=op, num_analise=num_analise, especialidade=especialidade, num_cedula=num_cedula,
                    num_doente=num_doente, data=data, data_reg=data_reg, nome=nome, quant=quant, inst=inst))

                elif op == "e":
                    especialidade = request.form["especialidade"]
                    num_cedula = request.form["num_cedula"]
                    num_doente = request.form["num_doente"]
                    data = request.form["data"]
                    data_reg = request.form["data_reg"]
                    nome = request.form["nome"]
                    quant = request.form["quant"]
                    inst = request.form["inst"]

                    return redirect(url_for('iedanalise', op=op, num_analise=num_analise, especialidade=especialidade, num_cedula=num_cedula, num_doente=num_doente,
                    data=data, data_reg=data_reg, nome=nome, quant=quant, inst=inst))

                elif op == "d":
                    return redirect(url_for('iedanalise', op=op, num_analise=num_analise))

                else:
                    return render_template("iedanalise.html", params=request.args)

        if request.method == "GET":
            if request.args.get("num_analise") is None:
                return render_template("iedanalise.html", params=request.args)

            elif request.args.get("opnotdone") is not None:
                return render_template("iedanalise.html", params=request.args)

            else:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                op = request.args.get("op")
                num_analise = request.args.get("num_analise")

                if request.args.get("op") == "i":
                    especialidade = request.args.get("especialidade")
                    num_cedula = request.args.get("num_cedula")
                    num_doente = request.args.get("num_doente")
                    data = request.args.get("data")
                    data_reg = request.args.get("data_reg")
                    nome = request.args.get("nome")
                    quant = request.args.get("quant")
                    inst = request.args.get("inst")

                    if num_cedula == num_doente == data == "":
                        query = ("insert into analise values(%s, %s, null, null, null, %s, %s, %s, %s);")

                        cursor.execute(query, (num_analise, especialidade, data_reg, nome, quant, inst,))

                    else:
                        query = ("insert into analise values(%s, %s, %s, %s, %s, %s, %s, %s, %s);")

                        cursor.execute(query, (num_analise, especialidade, num_cedula, num_doente, data, data_reg, nome, quant, inst,))

                elif request.args.get("op") == "e":
                    especialidade = request.args.get("especialidade")
                    num_cedula = request.args.get("num_cedula")
                    num_doente = request.args.get("num_doente")
                    data = request.args.get("data")
                    data_reg = request.args.get("data_reg")
                    nome = request.args.get("nome")
                    quant = request.args.get("quant")
                    inst = request.args.get("inst")

                    if request.args.get("especialidade") != "":
                        query = ("UPDATE analise SET especialidade = %s WHERE num_analise = %s;")

                        cursor.execute(query, (especialidade, num_analise,))

                    if request.args.get("num_cedula") != "":
                        query = ("UPDATE analise SET num_cedula = %s WHERE num_analise = %s;")

                        cursor.execute(query, (num_cedula, num_analise,))

                    if request.args.get("num_doente") != "":
                        query = ("UPDATE analise SET num_doente = %s WHERE num_analise = %s;")

                        cursor.execute(query, (num_doente, num_analise,))

                    if request.args.get("data") != "":
                        query = ("UPDATE analise SET data = %s WHERE num_analise = %s;")

                        cursor.execute(query, (data, num_analise,))

                    if request.args.get("data_reg") != "":
                        query = ("UPDATE analise SET data_registo = %s WHERE num_analise = %s;")

                        cursor.execute(query, (data_reg, num_analise,))

                    if request.args.get("nome") != "":
                        query = ("UPDATE analise SET nome = %s WHERE num_analise = %s;")

                        cursor.execute(query, (nome, num_analise,))

                    if request.args.get("quant") != "":
                        query = ("UPDATE analise SET quant = %s WHERE num_analise = %s;")

                        cursor.execute(query, (quant, num_analise,))

                    if request.args.get("inst") != "":
                        query = ("UPDATE analise SET inst = %s WHERE num_analise = %s;")

                        cursor.execute(query, (inst, num_analise,))


                elif request.args.get("op") == "d":
                    query = ("DELETE FROM analise WHERE num_analise = %s;")

                    cursor.execute(query, (num_analise,))

                return render_template("iedanalise.html", cursor=cursor, params=request.args)

    except Exception as e:
        return str(e)

    finally:
        if request.method == "GET" and request.args.get("num_analise") is not None and request.args.get("opnotdone") is None:
            dbConn.commit()
            cursor.close()
            dbConn.close()


@app.route('/sales')
def sales():
    return render_template("sales.html")


@app.route('/vendasprescricao', methods=["GET", "POST"])
def vendasprescricao():
    dbConn = None
    cursor = None

    try:
        if request.method == "POST":
            if request.args.get("num_doente") is None:
                num_doente = request.form["num_doente"]

                return redirect(url_for('vendasprescricao', num_doente=num_doente))

            else:
                num_cedula = request.form["num_cedula"]
                num_doente = request.args.get("num_doente")
                data_presc = request.form["data_presc"]
                data_reg = request.form["data_reg"]
                substancia = request.form["substancia"]
                quant = request.form["quant"]
                preco = request.form["preco"]
                inst = request.form["inst"]
                num_venda = request.form["num_venda"]

                return redirect(url_for('vendasprescricao', num_cedula=num_cedula, num_doente=num_doente, data_presc=data_presc,
                data_reg=data_reg, substancia=substancia, quant=quant, preco=preco, inst=inst, num_venda=num_venda))

        if request.method == "GET":
            if request.args.get("num_doente") is None:
                return render_template("vendasprescricao.html", params=request.args)

            elif request.args.get("num_cedula") is None:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                num_doente = request.args.get("num_doente")

                query = ("SELECT * "
                         "FROM prescricao "
                         "WHERE prescricao.num_doente = %s;")

                cursor.execute(query, (num_doente,))

                return render_template("vendasprescricao.html", cursor=cursor, params=request.args)

            else:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                num_cedula = request.args.get("num_cedula")
                num_doente = request.args.get("num_doente")
                data_presc = request.args.get("data_presc")
                data_reg = request.args.get("data_reg")
                substancia = request.args.get("substancia")
                quant = request.args.get("quant")
                preco = request.args.get("preco")
                inst = request.args.get("inst")
                num_venda = request.args.get("num_venda")

                insert = ("insert into venda_farmacia values(%s, %s, %s, %s, %s, %s);"
                          "insert into prescricao_venda values(%s, %s, %s, %s, %s);")

                cursor.execute(insert, (num_venda, data_reg, substancia, quant, preco, inst, num_cedula, num_doente, data_presc, substancia, num_venda,))

                return render_template("vendasprescricao.html", params=request.args)

    except Exception as e:
        return str(e)

    finally:
        if request.method == "GET" and request.args.get("num_cedula") is not None:
            dbConn.commit()
            cursor.close()
            dbConn.close()


@app.route('/vendassemprescricao', methods=["GET", "POST"])
def vendassemprescricao():
    dbConn = None
    cursor = None

    try:
        if request.method == "POST":
            data_reg = request.form["data_reg"]
            substancia = request.form["substancia"]
            quant = request.form["quant"]
            preco = request.form["preco"]
            inst = request.form["inst"]
            num_venda = request.form["num_venda"]

            return redirect(url_for('vendassemprescricao', num_venda=num_venda, data_reg=data_reg, substancia=substancia,
            quant=quant, preco=preco, inst=inst))

        if request.method == "GET":
            if request.args.get("num_venda") is None:
                return render_template("vendassemprescricao.html", params=request.args)

            else:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                num_venda = request.args.get("num_venda")
                data_reg = request.args.get("data_reg")
                substancia = request.args.get("substancia")
                quant = request.args.get("quant")
                preco = request.args.get("preco")
                inst = request.args.get("inst")

                insert = ("insert into venda_farmacia values(%s, %s, %s, %s, %s, %s);")

                cursor.execute(insert, (num_venda, data_reg, substancia, quant, preco, inst,))

                return render_template("vendassemprescricao.html", cursor=cursor, params=request.args)

    except Exception as e:
        return str(e)

    finally:
        if request.method == "GET" and request.args.get("num_venda") is not None:
            dbConn.commit()
            cursor.close()
            dbConn.close()


@app.route('/substances', methods=["GET", "POST"])
def substances():
    dbConn = None
    cursor = None

    try:
        if request.method == "POST":
            num_cedula = request.form["num_cedula"]
            mes = request.form["mes"]

            return redirect(url_for('substances', num_cedula=num_cedula, mes=mes))

        if request.method == "GET":
            if request.args.get("num_cedula") is None:
                return render_template("substances.html", params=request.args)

            else:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                num_cedula = request.args.get("num_cedula")
                mes = request.args.get("mes")

                data = mes.split("-")

                query = ("SELECT DISTINCT substancia FROM prescricao WHERE num_cedula = %s AND EXTRACT(month FROM data) = %s "
                         "AND EXTRACT(year FROM data) = %s;")

                cursor.execute(query, (num_cedula, data[1], data[0],))

                return render_template("substances.html", cursor=cursor, params=request.args)

    except Exception as e:
        return str(e)

    finally:
        if request.method == "GET" and request.args.get("num_cedula") is not None:
            dbConn.commit()
            cursor.close()
            dbConn.close()


@app.route('/glicemia', methods=["GET", "POST"])
def glicemia():
    dbConn = None
    cursor = None

    try:
        if request.method == "POST":
            num_regiao = request.form["num_regiao"]
            num_concelho = request.form["num_concelho"]

            return redirect(url_for('glicemia', num_regiao=num_regiao, num_concelho=num_concelho))

        if request.method == "GET":
            if request.args.get("num_regiao") is None:
                return render_template("glicemia.html", params=request.args)

            else:
                dbConn = psycopg2.connect(DB_CONNECTION_STRING)
                cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)

                num_regiao = request.args.get("num_regiao")
                num_concelho = request.args.get("num_concelho")

                query = ("SELECT num_doente, MAX(quant), num_doente, MIN(quant) "
                         "FROM analise INNER JOIN instituicao ON analise.inst = instituicao.nome "
                         "WHERE analise.nome = 'Glicémia (mg/dL)' AND instituicao.num_regiao = %s AND instituicao.num_concelho = %s "
                         "GROUP BY num_doente;")

                cursor.execute(query, (num_regiao, num_concelho,))

                return render_template("glicemia.html", cursor=list(cursor), params=request.args)

    except Exception as e:
        return str(e)

    finally:
        if request.method == "GET" and request.args.get("num_regiao") is not None:
            dbConn.commit()
            cursor.close()
            dbConn.close()


CGIHandler().run(app)
