# Mafalda Serafim , n 92512

# funcoes basicas
# celula : {'valor': {1,0,-1},'estado': {'ativo','inativo','incerto'}} - dict
# coordenada: (l,c) - tuplo
# tabuleiro : [[xxx],[xxx],[xx]] - lista (x in (0,1,-1))

# construtores

def cria_celula(v):
    """cria_celula: {1,0,-1} -> celula"""
    dict_cel = {}
    if v in (0,1,-1):
        if v == 1:
            dict_cel = {'valor': 1, 'estado': 'ativo'}
        elif v == 0:
            dict_cel = {'valor': 0, 'estado': 'inativo'}
        else:
            dict_cel = {'valor': -1, 'estado': 'incerto'}
        return dict_cel
    raise ValueError('cria_celula: argumento invalido.')

def cria_coordenada(l,c):
    """cria_coordenada: N*N -> coordenada"""
    if l not in (0,1,2) or c not in (0,1,2):
        raise ValueError ('cria_coordenada: argumentos invalidos.')
    elif l == 2 and c == 0:  #nao existe coordenada(2,0)
        raise ValueError ('cria_coordenada: argumentos invalidos.')
    return (l,c)

def tabuleiro_inicial():
    """tabuleiro_inicial: {} -> tabuleiro"""
    return [[-1,-1,-1], [0,0,-1], [0,-1]]

def str_para_tabuleiro(s):
    """str_para_tabuleiro: cad.caracteres -> tabuleiro"""
    if isinstance(s,str) and isinstance(eval(s), tuple) and len(eval(s)) == 3 and\
       isinstance(eval(s)[0], tuple) and  isinstance(eval(s)[1], tuple) and \
       isinstance(eval(s)[2], tuple):
        t, str_lst = eval(s), []
        for i in range(len(t)):
            str_lst = str_lst + [list(t[i]),]
        if eh_tabuleiro(str_lst):
            return str_lst
    raise ValueError('str_para_tabuleiro: argumento invalido.')

# seletores 

def obter_valor(c):
    """obter_valor: celula -> {1,0,-1}"""
    return c['valor']

def coordenada_linha(c):
    """coordenada_linha: coordenada -> N"""
    return c[0]

def coordenada_coluna(c):
    """coordenada_coluna: coordenada -> N"""
    return c[1]

def tabuleiro_dimensao(t):
    """tabuleiro_dimensao: tabuleiro -> N"""
    return len(t)

def tabuleiro_celula(t, coor):
    """tabuleiro_celula: tabuleiro x coordenada -> celula"""
    if coordenada_linha(coor) != 2:
        return cria_celula(t[coordenada_linha(coor)][coordenada_coluna(coor)])
    else:
        return cria_celula(t[coordenada_linha(coor)][coordenada_coluna(coor)-1])
    
# modificadores
       
def inverte_estado(c):
    """inverte_estado: celula -> celula"""
    if c['estado'] == 'ativo':
        c['valor'], c['estado'] = 0, 'inativo'
    elif c['estado'] == 'inativo':
        c['valor'], c['estado'] = 1, 'ativo'
    return c

def tabuleiro_substitui_celula(t, cel, coor):
    """tabuleiro_substitui_celula: tabulerio x celula x coordenada -> tabuleiro"""
    if eh_tabuleiro(t) and eh_celula(cel) and eh_coordenada(coor):
        if coordenada_linha(coor) != 2:
            t[coordenada_linha(coor)][coordenada_coluna(coor)]=obter_valor(cel)
            return t
        else: 
            t[coordenada_linha(coor)][coordenada_coluna(coor)-1]=obter_valor(cel)
            return t
    raise ValueError('tabuleiro_substitui_celula: argumentos invalidos.')
            
def tabuleiro_inverte_estado(t, coor):
    """tabuleiro_inverte_estado: tabuleiro x coordenada -> tabuleiro"""
    if eh_tabuleiro(t) and eh_coordenada(coor):    
        return tabuleiro_substitui_celula(t,inverte_estado\
                                      (tabuleiro_celula(t,coor)),coor)
    raise ValueError ('tabuleiro_inverte_estado: argumentos invalidos.')

# reconhecedores 

def eh_celula(arg):
    """eh_celula: universal -> logico"""
    return isinstance(arg, dict) and len(arg) == 2 and 'valor' in arg and \
           'estado' in arg and arg['valor'] in (0,1,-1) and arg['estado'] in \
           ('ativo', 'inativo', 'incerto') 

def eh_coordenada(arg):
    """eh_coordenada: universal -> logico"""
    return isinstance(arg, tuple) and len(arg) == 2 and arg[0] in (0,1,2) and \
           arg[1] in (0,1,2) and coordenada_existente(arg)

def eh_tabuleiro(arg):
    """eh_tabuleiro: universal -> logico"""
    def verifica_list_len(arg):
        return isinstance(arg, list) and len(arg) == 3 and \
               isinstance(arg[0], list) and isinstance(arg[1], list) and \
               isinstance (arg[2],list)
    def verifica_len(arg):
        return len(arg[0]) == len(arg[1]) == 3 and len(arg[2]) == 2
    def verifica_arg(arg):
        for i in range(2):
            if arg[0][i] not in (-1,0,1) or arg[1][i] not in (-1,0,1):
                return False
        for i in arg[2]:
            if i not in (-1,0,1):
                return False
        return True
    return verifica_list_len(arg) and verifica_len(arg) and verifica_arg(arg)

# testes

def celulas_iguais(c1, c2):    
    """celulas_iguais: celula*celula -> logico"""
    return c1 == c2 and eh_celula(c1) and eh_celula(c2)


def coordenadas_iguais(c1, c2):
    """coordenadas_iguais: coordenada*coordenada -> logico"""
    return c1 == c2 

def tabuleiros_iguais(t1,t2):
    """tabuleiros_iguais: tabuleiro x tabuleiro -> logico"""
    return t1 == t2 and eh_tabuleiro(t1) and eh_tabuleiro(t2)

# transformadores

def celula_para_str(c):
    """celula_para_str: celula -> cad.caracteres"""
    if c['estado'] != 'incerto':
        return str(obter_valor(c))
    else:
        return 'x'
    
def coordenada_para_str(c):
    """coordenada_para_str: coordenada -> cad.caracteres"""
    return str(c)

def tabuleiro_para_str(t):
    """tabuleiro_para_str: tabuleiro -> cad.caracteres"""
    str_tab = ''
    for i in range(len(t)):
        for j in range(len(t[i])):
            str_tab = str_tab + celula_para_str(cria_celula(t[i][j]))
    return '+-------+\n|...' + str_tab[2] + '...|\n|..' + str_tab[1] +'.'+ \
           str_tab[5] + '..|\n|.'+ str_tab[0] + '.'+ str_tab[4]+ '.'+ \
           str_tab[7] +'.|\n|..' + str_tab[3] +'.'+ str_tab[6]+ '..|\n+-------+'

# funcoes auxiliares    

# verifica que o arg nunca sera a coordenada (2,0), um vez que esta nao existe no jogo

def coordenada_existente(arg):
    if arg[0] in (0,1):
        return arg[1] in (0,1,2)
    else:
        return arg[1] in (1,2)

# auxiliar da porta_h, recebe o tabuleiro, o lado e as 3 celulas que e necessario salvaguardar
# de maneira a que seja possivel trocar duas linhas ('E') ou duas colunas ('D')
# realizando, entao, a substituicao do correspondente pelas celulas salvaguardadas

def aux_portah(t, lado, t0,t1,t2):
    if lado == 'E':
        tabuleiro_substitui_celula(t, t0, cria_coordenada(0,0))
        tabuleiro_substitui_celula(t, t1, cria_coordenada(0,1))
        tabuleiro_substitui_celula(t, t2, cria_coordenada(0,2))
    else:
        tabuleiro_substitui_celula(t, t0, cria_coordenada(0,2))         
        tabuleiro_substitui_celula(t, t1, cria_coordenada(1,2))
        tabuleiro_substitui_celula(t, t2, cria_coordenada(2,2))
    return t   

# funcoes de alto nivel

def porta_x(t, lado): # altera a linha 2 ou coluna 2, invertendo as celulas
    """porta_x: tabuleiro x {'E', 'D'} -> tabuleiro"""
    if not eh_tabuleiro(t) or lado not in ('E', 'D'):
        raise ValueError('porta_x: argumentos invalidos.')
    elif lado == 'E':
        for c in range(0,3):
            tabuleiro_inverte_estado(t, cria_coordenada(1,c))
    else:
        tabuleiro_inverte_estado(t, cria_coordenada(2,1))
        for l in range(0,2):
            tabuleiro_inverte_estado(t, cria_coordenada(l,1))
    return t
        
def porta_z(t, lado):  # altera a linha 1 ou coluna 3, invertendo as celulas 
    """porta_z: tabuleiro x {'E', 'D'} -> tabuleiro"""
    if not eh_tabuleiro(t) or lado not in ('E', 'D'):
        raise ValueError('porta_z: argumentos invalidos.')
    elif lado == 'E':
        for c in range(0,3):
            tabuleiro_inverte_estado(t, cria_coordenada(0,c))
    else:
        tabuleiro_inverte_estado(t, cria_coordenada(2,2))
        for l in range(0,2):
            tabuleiro_inverte_estado(t, cria_coordenada(l,2))
    return t

def porta_h(t, lado):
    """porta_h: tabuleiro x {'E', 'D'} -> tabuleiro"""
    if not eh_tabuleiro(t) or lado not in ('E', 'D'):
        raise ValueError('porta_h: argumentos invalidos.')
    elif lado == 'E': #troca-se a linha 1 com a 2
        t00, t01, t02 = tabuleiro_celula(t, cria_coordenada(1,0)), \
            tabuleiro_celula(t, cria_coordenada(1,1)),\
            tabuleiro_celula(t, cria_coordenada(1,2))
        for c in range(0,3):
            tabuleiro_substitui_celula(t,\
                                       tabuleiro_celula(t,cria_coordenada(0,c)),\
                                       cria_coordenada(1,c))
        aux_portah(t, 'E', t00, t01, t02)
    else: # troca-se a coluna 2 com a 3
        t01, t11, t20 = tabuleiro_celula(t, cria_coordenada(0,1)), \
            tabuleiro_celula(t, cria_coordenada(1,1)),\
            tabuleiro_celula(t, cria_coordenada(2,1))
        for l in range(0,2):
            tabuleiro_substitui_celula(t, \
                                       tabuleiro_celula(t,cria_coordenada(l,2)),\
                                       cria_coordenada(l,1))
        tabuleiro_substitui_celula(t, tabuleiro_celula(t, cria_coordenada(2,2)),\
                                   cria_coordenada(2,1))
        aux_portah(t, 'D', t01, t11, t20)
    return t

def hello_quantum(s): 
    """hello_quantum: cad.caracteres -> logico"""
    for i in range(len(s)): # separa a string em 2
        if s[i] == ':':
            s1, s2 = s[:i], s[i+1:]
    t, t1, n, n1 = str_para_tabuleiro(s1), tabuleiro_inicial(), eval(s2), 0
    print('Bem-vindo ao Hello Quantum!\nO seu objetivo e chegar ao tabuleiro:\n'\
          + tabuleiro_para_str(t) + '\nComecando com o tabuleiro que se segue:\n'\
          + tabuleiro_para_str(t1))
    while n1 < n and not tabuleiros_iguais(t, t1):
        porta = input('Escolha uma porta para aplicar (X, Z ou H): ')
        lado = input('Escolha um qubit para analisar (E ou D): ')
        if porta == 'X':
            porta_x(t1, lado)
        elif porta == 'Z':
            porta_z(t1, lado)
        elif porta == 'H':
            porta_h(t1, lado)
        n1 = n1 + 1 #numero de jogadas aumenta
        print(tabuleiro_para_str(t1)) # faz-se o print do tabuleiro modificado
    if tabuleiros_iguais(t, t1): # se o objetivo se cumprir
        print('Parabens, conseguiu converter o tabuleiro em ' + str(n1) + \
              ' jogadas!')        
    return tabuleiros_iguais(t, t1)