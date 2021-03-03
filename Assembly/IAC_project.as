; DIOGO BARBOSA, N 92451
; MAFALDA SERAFIM, N 92512

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; NESTE PROJETO O GRUPO OPTOU POR UMA REPRESENTACAO EM VIRGULA FIXA COM  ;;;;
;;;; 8 BITS PARA A PARTE INTEIRA E 8 BITS PARA A PARTE DECIMAL, UMA VEZ QUE ;;;;
;;;; ESTA E UMA NOTACAO QUE FACILITA A LEITURA DOS RESULTADOS (2 DIGITOS HEXA ;;
;;;; PARA CADA PARTE) E PERMITE UM GRAU DE PRECISAO BASTANTE RAZOAVEL, SENDO ;;;
;;;; QUE E POSSIVEL REPRESENTAR ATE 1/256.                                  ;;;;
;;;; REGISTOS R5 E R6 SAO ATUALIZADOS COM OS NOVOS VALORES DAS COORDENADAS  ;;;;
;;;; (X E Y, RESPETIVAMENTE) DE CADA VEZ QUE A FUNCAO TRAJETORIA E CHAMADA. ;;;;
;;;; NO CALCULO DESTAS COORDENADAS, CONSIDERA-SE A POSICAO INICIAL 0000,    ;;;;
;;;; RESTANDO APENAS SOMAR-LHES A POSICAO INICIAL.                          ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;TABELA DE CONSTANTES;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LEITURA         EQU FFFFh
ESCRITA         EQU FFFEh
ESTADO          EQU FFFDh
CONTROLO        EQU FFFCh
INICIA          EQU FFFFh
SP_INICIAL      EQU FDFFh
TIMER_COUNT     EQU FFF6h                                                       ;INDICA O NUM DE INTERVALOS (DE 100ms) AO FIM DO QUAL O TEMPORIZADOR GERA UMA INT
TIMER_CTRL      EQU FFF7h                                                       ;PERMITE ARRANCAR OU PARAR O TEMPORIZADOR
FIM_TEXTO       EQU 0

MASCARA         EQU FFFAh                                                       ;MASCARAS UTILIZADAS
MASCARA_INI     EQU 0C00h                                                       ;PERMITE INTA E INTB
MASCARA_JOGO    EQU 0BFFh                                                       ;PERMITE INT0 - INT9 E INTB
MASCARA_T       EQU 8800h                                                       ;PERMITE INTF E INTB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;TABELA DE VARIAVEIS;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ORIG 8000h

DIGITO_1        STR 0,0,0,0                                                     ;INICIA DISPLAY

FLAG_A          WORD 0                                                          ;FLAGS INTERRUPCOES
FLAG            WORD 0
FLAG_TRAJETORIA WORD 0
FLAG_E          WORD 0
FLAG_NUM        WORD 0
FLAG_SAIR       WORD 0
FLAG_2          WORD 0

GORILAL1        STR '(**)', FIM_TEXTO                                           ;STRINGS UTILIZADAS
GORILAL2        STR '/II\', FIM_TEXTO
GORILAL3        STR ' !! ', FIM_TEXTO
INSTRUCOES      STR 'Para iniciar o jogo, pressione a tecla IA. Para sair do jogo, pressione IB.', FIM_TEXTO
INSTRUCOES1     STR 'Bem vindo a gorillas. Pressione em IA para continuar.', FIM_TEXTO
BANANA          STR '(', FIM_TEXTO
VALOR1          STR 'VELOCIDADE: ', FIM_TEXTO
VALOR2          STR 'ANGULO: ', FIM_TEXTO
PONTOS          STR 'PONTUACAO: ', FIM_TEXTO
WINNER          STR 'WINNER !!!', FIM_TEXTO

G1              WORD 0                                                          ;GUARDA POSICAO DO GORILA DA ESQUERDA
G2              WORD 0                                                          ;GUARDA POSICAO DO GORILA DA DIREITA
L1              WORD 0                                                          ;GUARDAM VALORES DOS LIMITES DA AREA DO GORILA 2
L2              WORD 0
C1              WORD 0
C2              WORD 0
POSICAO_BANANA  WORD 0                                                          ;GUARDA POSICAO INICIAL DA BANANA
BANANA_ANTERIOR WORD 0                                                          ;GUARDA POSICAO DA BANANA ANTERIOR, COM O OBJETIVO DE A APAGAR
PONTUACAO       WORD 0                                                          ;GUARDA PONTUACAO
NEW_G1          WORD 0                                                          ;VARIAVEIS RESPONSAVEIS PELO CALCULO DA NOVA POSICAO DOS GORILAS
NEW_G2          WORD 0

;;;;;;;;;;GUARDAM VALORES QUE SAO UTILIZADOS NO CALCULO DA TRAJETORIA;;;;;;;;;;;
;;;; PARA MOTIVOS DE SIMPLIFICACAO, O REGISTO R4 E SEMPRE CONSIDERADO O TEMPO ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ANG             WORD 0                                                          ;GUARDA VALOR INSERIDO
VEL             WORD 0                                                          ;GUARDA VALOR INSERIDO

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;INTERRUPCOES;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ORIG FE00h

INT0              WORD I0
INT1              WORD I1
INT2              WORD I2
INT3              WORD I3
INT4              WORD I4
INT5              WORD I5
INT6              WORD I6
INT7              WORD I7
INT8              WORD I8
INT9              WORD I9
INTA              WORD INICIAR_JOGO
INTB              WORD SAIR_JOGO

ORIG FE0Fh
INTF              WORD TIMER

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;MAIN;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ORIG 0000h
                MOV R7,SP_INICIAL                                               ;INICIA PILHA
                MOV SP,R7

                MOV R7, 0

                MOV R1, INICIA                                                  ;INICIALIZA CURSOR
                MOV M[CONTROLO], R1


                MOV R1, INSTRUCOES1                                             ;ESCREVE AS PRIMEIRAS INSTRUCOES NA JANELA TEXTO
                PUSH R1
                CALL ESCREVER

                MOV R1, MASCARA_INI                                             ;MASCARA PERMITE APENAS INTA
                MOV M[MASCARA], R1
                ENI                                                             ;PERMITE INT ESPECIFICADAS NA MASCARA

INICIAR_LOOP:   MOV R1, M[FLAG_A]                                               ;SAI DO LOOP QUANDO INTA FOR PRESSIONADA
                INC R5                                                          ;INCREMENTA R5 INDEFINIDAMENTE - VALOR ALEATORIO
                CMP R1, 1h
                BR.NZ INICIAR_LOOP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;POSICAO GORILA 1;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; A PARTIR DO VALOR ALEATORIO, CRIAM-SE AS COORDENADAS X E Y (X LIMITADO ;;;;
;;;; ENTRE 0 E 31, Y LIMITADO ENTRE 4 E 22). COORDENADA Y OCUPA OS DOIS     ;;;;
;;;; ALGARISMOS DA ESQUERDA EM R7, COORDENADA X OCUPA OS DOIS DA DIREITA.   ;;;;
;;;; MOVE-SE ESSE REGISTO PARA UMA VARIAVEL (G1), FICANDO COM TODOS OS      ;;;;
;;;; REGISTOS LIVRES.                                                       ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;POSICAO GORILA 1;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                MOV R6, R5
                MOV R2, 31
                DIV R6, R2                                                      ;COLUNA RANDOM EM R2
                SHR R5, 8
                MOV R6, 18
                DIV R5, R6
                ADD R6, 4
                SHL R6, 8                                                       ;VALOR DE Y OCUPA OS DOIS DIGITOS MAIS SIGNIFICATIVOS DE R6
                ADD R6, R2
                MOV R7, R6                                                      ;COORDENADAS RANDOM DO 1 GORILA EM R7
                MOV M[G1], R7                                                   ;COORDENADAS DO 1 GORILA NA VARIAVEL G1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                CALL APAGAR_INST                                                ;APAGAR TUDO O QUE ESTA ESCRITO NA JANELA TEXTO

                MOV M[FLAG_A], R0

                MOV R7, 0                                                       ;ESCREVER A CONTINUACAO DAS INSTRUCOES (CURSOR EM 0000h)
                MOV R1, INSTRUCOES
                PUSH R1
                CALL ESCREVER

                MOV R5, 0                                                       ;R5 PASSA A 0 (LIMPA O VALOR ALEATORIO ANTERIOR)


INICIAR_JOGO1:  MOV R1, M[FLAG_A]                                               ;SAI DO LOOP QUANDO INTA FOR PRESSIONADA
                INC R5                                                          ;INCREMENTA R5 INDEFINIDAMENTE - VALOR ALEATORIO
                CMP R1, 1h
                BR.NZ INICIAR_JOGO1
                DSI                                                             ;DISABILITA TODAS AS INTERRUPCOES

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;POSICAO GORILA 2;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; SEMELHANTE AO CALCULO DA POSICAO DO GORILA 1, SENDO QUE DESTA VEZ O X  ;;;;
;;;; VARIA DE 40 A 71 (ADICIONA-SE 40 AO X QUE VARIAVA ENTRE 0 E 31). O Y   ;;;;
;;;; CONTINUA A VARIAR ENTRE 4 E 22.                                        ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                MOV R6, R5
                MOV R2, 31
                DIV R6, R2                                                      ; COLUNA RANDOM EM R2
                ADD R2, 40
                SHR R5, 8
                MOV R6, 18
                DIV R5, R6
                ADD R6, 4
                SHL R6, 8                                                       ;Y NOS DOIS DIGITOS MAIS SIGNIFICATIVOS DE R6
                ADD R6, R2
                MOV R7, R6                                                      ; COORDENADAS RANDOM DO GORILA 2 EM R7
                MOV M[G2], R7                                                   ; COORDENADAS DO GORILA 2 NA VARIAVEL G2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

NOVA_JOGADA:    MOV R1, MASCARA_JOGO                                            ;NOVA MASCARA (ESPECIFICA INT0 - INT9)
                MOV M[MASCARA], R1

                CALL APAGAR_INST                                                ;APAGA TUDO O QUE ESTA ESCRITO NA JANELA DE TEXTO

                CMP M[FLAG_2], R0                                                ;SE NAO FOR A PRIMEIRA JOGADA, AS COORDENADAS DOS GORILAS VAO SER ALTERADAS DE FORMA RANDOM
                JMP.Z JOG_1

;;;;;;;;;;;;;;;;;;;;CALCULO DAS NOVAS POSICOES DOS GORILAS;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;NOVA POSICAO GORILA 1;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                MOV R6, M[NEW_G1]                                               ;VALOR E ADICIONADA A R6 PARA QUE AS LINHAS TENHAM UMA MAIOR VARIACAO
                MOV R2, 31
                DIV R6, R2                                                      ;COLUNA RANDOM EM R2
                SHR R5, 8
                ADD R5, M[NEW_G2]                                               ;DE MODO A CRIAR UM MAIOR GRAU DE VARIACAO
                MOV R6, 18
                DIV R5, R6
                ADD R6, 4
                SHL R6, 8                                                       ;VALOR DE Y OCUPA OS DOIS DIGITOS MAIS SIGNIFICATIVOS DE R6
                ADD R6, R2
                MOV R7, R6                                                      ;COORDENADAS RANDOM DO 1 GORILA EM R7
                MOV M[G1], R7                                                   ;COORDENADAS DO 1 GORILA NA VARIAVEL G1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;NOVA POSICAO GORILA 2;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                MOV R6, M[NEW_G2]                                               ;CALCULADA DE FORMA SEMELHANTE A NOVA POSICAO DO GORILA 1
                MOV R2, 31
                DIV R6, R2                                                      ; COLUNA RANDOM EM R2
                ADD R2, 40
                SHR R5, 8
                ADD R5, M[NEW_G1]                                               ;DE MODO A CRIAR UM MAIOR GRAU DE VARIACAO
                MOV R6, 18
                DIV R5, R6
                ADD R6, 4
                SHL R6, 8                                                       ;Y NOS DOIS DIGITOS MAIS SIGNIFICATIVOS DE R6
                ADD R6, R2
                MOV R7, R6                                                      ; COORDENADAS RANDOM DO GORILA 2 EM R7
                MOV M[G2], R7                                                   ; COORDENADAS DO GORILA 2 NA VARIAVEL G2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



JOG_1:          CALL DESENHO_GORILAS                                            ;ESCREVE OS GORILAS NAS COORDENADAS GUARDADAS EM G1 E G2

                MOV R1, M[G2]                                                   ;GUARDA OS LIMITES DA AREA EM QUE O GORILA 2 SE ENCONTRA
                AND R1, FF00h
                MOV M[L1], R1                                                   ;L1 - LINHA SUPERIOR
                ADD R1, 0300h
                MOV M[L2], R1                                                   ;L2 - LINHA INFERIOR

                MOV R1, M[G2]
                AND R1, 00FFh
                MOV M[C1], R1                                                   ;C1 - COLUNA DA ESQUERDA
                ADD R1, 0004h
                MOV M[C2], R1                                                   ;C2 - COLUNA DA DIREITA

                CALL ESCREVER_VAL                                               ;ESCREVE 'VELOCIDADE' E 'ANGULO' NA JANELA DE TEXTO

                ENI                                                             ;PERMITE INTERRUPCOES ESPECIFICADAS NA MASCARA

                MOV M[FLAG_NUM], R0
                MOV M[FLAG_E], R0
                MOV R7, 000Ch                                                   ;VALOR DEPOIS MOVIDO PARA M[CONTROLO] NA FUNCAO ESCREVER_NUM
LOOP_VAL:       MOV R1, M[FLAG_NUM]                                             ;SAI DO LOOP QUANDO UMA DAS INT PERMITIDAS E ATIVADA
                INC M[NEW_G1]                                                   ;UMA DAS VARIAVEIS DE DEFINIRAO A POSICAO DOS GORILAS NA JOGADA SEGUINTE
                CMP R1, 0
                BR.Z LOOP_VAL

                PUSH R3
                CALL ESCREVER_NUM                                               ;ESCREVE R3 E COLOCA NA STACK O VALOR ASSOCIADO
                MOV M[FLAG_NUM], R0
                MOV R2, M[FLAG_E]                                               ;(M[FLAG_E] AUMENTA QUANDO UMA INTERRUPCAO E ATIVADA)
                CMP R2, 2                                                       ;QUANDO FOREM ESCRITOS 2 NUMEROS (R2 = 2), O CURSOR MOVE PARA R7 O VALOR 0108h
                BR.N LOOP_VAL                                                   ;SE R2 FOR MENOR QUE 2, VOLTA PARA O LOOP_VAL
                BR.P LOOP_VAL1                                                  ;SE R2 FOR MAIOR QUE 2 (SE R2 = 3 OU R2 = 4), SALTA PARA LOOP_VAL1
                MOV R7, 0108h                                                   ;INSTRUCAO ACONTECE QUANDO R2 = 2
LOOP_VAL1:      CMP R2, 4
                BR.Z JOGO                                                       ;QUANDO R2 = 4, SALTA PARA FUNCAO JOGO
                BR LOOP_VAL                                                     ;SE R2 NAO FOR 4, VOLTA PARA LOOP_VAL


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; STACK COM 4 VALORES, QUE DEPENDEM DAS INTERRUPCOES QUE FORAM ATIVADAS  ;;;;
;;;; OS DOIS PRIMEIROS CORRESPONDEM, RESPETIVAMENTE, AO PRIMEIRO E SEGUNDO  ;;;;
;;;; ALGARISMOS DO VALOR DA VELOCIDADE. OS DOIS SEGUINTES VALORES NA STACK  ;;;;
;;;; CORRESPONDEM, RESPETIVAMENTE, AO PRIMEIRO E SEGUNDO ALGARISMOS DO VALOR ;;;
;;;; DO ANGULO.                                                             ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


JOGO:           DSI                                                             ;APOS SEREM ESCRITOS OS 4 NUM (2 PARA VEL E 2 PARA ANG), SAO DESABILITADAS TODAS AS INT

                MOV R1, M[G1]
                ADD R1, 4
                MOV M[POSICAO_BANANA], R1                                       ;M[POSICAO_BANANA] GUARDA A POSICAO INICIAL DA BANANA. DEPENDE DA POSICAO DO GORILA 1
                MOV R7, M[POSICAO_BANANA]                                       ;(R7 E MOVIDO PARA M[CONTROLO] NA FUNCAO ESCREVER)
                MOV R1, BANANA
                PUSH R1
                CALL ESCREVER                                                   ;ESCREVE-SE A BANANA 4 COLUNAS A FRENTE DE M[G1] - POSICAO GORILA 1

                MOV R1, M[POSICAO_BANANA]                                       ;MOVE A POSICAO DA 1 BANANA PARA OUTRA VARIAVEL, DE MODO A QUE A ULTIMA TENHA SEMPRE A POSICAO DA ULTIMA BANANA ESCRITA
                MOV M[BANANA_ANTERIOR], R1

;;;;;;;;;;;;;RETIRAM-SE DA STACK OS VALORES DO ANGULO E VELOCIDADE;;;;;;;;;;;;;;
;;;; UM DOS VALORES CORRESPONDENTES AO ANGULO E MULTIPLICADO POR 10, UMA VEZ ;;;
;;;; QUE SERA O DIGITO MAIS SIGNIFICATIVO. SOMAM-SE DEPOIS OS DOIS VALORES  ;;;;
;;;; PARA OBTER O VALOR DO ANGULO. O MESMO PROCESSO E DEPOIS SEGUIDO PARA   ;;;;
;;;; OBTER O VALOR DA VELOCIDADE.                                           ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                POP R1                                                          ;RETIRA OS DOIS VALORES CORRESPONDENTES AO ANGULO
                POP R2
                MOV R3, 10
                MUL R3, R2
                ADD R1, R2
                CMP R1, 70                                                      ;ANGULO UTILIZADO NOS CALCULOS E LIMITADO ENTRE 0 E 70, IMPEDINDO A BANANA DE SER LANCADA PARA TRAS
                BR.N MENOR                                                      ;FOI UTILIZADO 70 E NAO 90 DEVIDO A BAIXA RESOLUCAO DA JANELA DE TEXTO
                MOV R1, 70
MENOR:          MOV M[ANG], R1                                                  ;MOVE PARA A VARIAVEL ANG O VALOR DO ANGULO

                POP R2
                POP R3
                MOV R4, 10
                MUL R4, R3
                ADD R2, R3
                MOV M[VEL], R2                                                  ;MOVE PARA A VARIAVEL VEL O VALOR DA VELOCIDADE


                MOV R1, M[ANG]                                                  ;CONVERTE O VALOR DO ANGULO PARA A NOTACAO UTILIZADA
                PUSH R1
                CALL CONVERT_SHIFT
                POP R1
                PUSH R1
                CALL CONVERT_ANG
                POP R7                                                          ;O VALOR DO ANGULO FICA GUARDADO EM R7

                MOV R3, M[VEL]
                PUSH R3
                CALL CONVERT_SHIFT
                POP R3                                                          ;MOVE-SE A VELOCIDADE, JA CONVERTIDA PARA A NOTACAO UTILIZADA, PARA R3

                MOV R1, MASCARA_T                                               ;NOVA MASCARA (SO ESPECIFICA INTF)
                MOV M[MASCARA], R1

POS_TRAJ:       MOV R1, 1                                                       ;INICIALIZACAO DO TEMPORIZADOR
                MOV M[TIMER_CTRL], R1
                MOV R1, 1                                                       ;VAO OCORRER INTERRUPCOES DE SEGUNDO A SEGUNDO
                MOV M[TIMER_COUNT], R1
                ENI                                                             ;PERMITE A INTF

LOOP_JOGO:      MOV R1, M[FLAG_TRAJETORIA]                                      ;SO SAI DO LOOP APOS 1 SEG (QUANDO INTF OCORRE)
                CMP R1, 0
                BR.Z LOOP_JOGO

                DSI                                                             ;DESABILITA TODAS AS INTERRUPCOES

                CALL TRAJETORIA                                                 ;CALCULA A POSICAO SEGUINTE DA BANANA

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;ESCREVER NOVA BANANA;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; R5 - VARIACAO DE X NO SEGUNDO CORRESPONDENTE ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; R6 - VARIACAO DE Y NO SEGUNDO CORRESPONDENTE ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                PUSH R7
                PUSH R1                                                         ;SALVAGUARDA DE REGISTOS
                PUSH R3

                MOV R1, M[POSICAO_BANANA]                                       ;POSICAO INICIAL INICIAL EM R1
                MOV R2, R1
                MOV R3, R1                                                      ;VARIACAO DO X EM R3 (JA COMO COORDENADA NA JANELA)
                SHR R1, 8                                                       ;8 BITS MAIS SIGNIFICATIVOS FICAM EM R1
                SHR R3, 8                                                       ;REPRESENTACAO DAS LINHAS (DA BANANA INICIAL) EM R3 E R1
                AND R2, 00FFh                                                   ;REPRESENTACAO DAS COLUNAS DA BANANA INICIAL

                SHR R5, 8                                                       ;ESCALA PARA ADAPTAR A VISUALIZACAO NA JANELA DE TEXTO
                SHR R6, 8
                CMP R6, 100000b                                                 ;VERIFICAR O SINAL DE R6
                BR.NP SUBTRAIR                                                  ;SE R6 FOR POSITIVO, SALTA PARA A LABEL SUBTRAIR
                SHL R6, 8                                                       ;MECANISMO PARA CALCULAR O VALOR ABSOLUTO DE R6 (COMPLEMENTO PARA DOIS)
                XOR R6, FFFFh
                SHR R6, 8
                ADD R6, 1
                ADD R3, R6                                                      ;SOMA DA LINHA INICIAL AO VALOR ABSOLUTO DE R6
                BR SEG
SUBTRAIR:       SUB R3, R6

SEG:            ADD R2, R5                                                      ;ADICIONA A VARIACAO DO X AO X INICIAL

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;APAGAR BANANA ANTERIOR;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                MOV R7, M[BANANA_ANTERIOR]                                      ;APAGA A BANANA ANTERIORMENTE ESCRITA
                MOV M[CONTROLO], R7
                MOV R1, ' '
                MOV M[ESCRITA], R1

;;;;;;;;;;;;;;COMPARACAO DAS COORDENADAS COM LIMITES ESTABELECIDOS;;;;;;;;;;;;;;

                SHL R3, 8                                                       ;COLOCA A POSICAO RESPETIVA AS LINHAS NA SUA POSICAO CORRESPONDENTE

                CMP R3, M[L1]                                                   ;SE A POSICAO DA BANANA SE ENCONTRA DENTRO DOS LIMITES DO GORILA 2
                JMP.N PASSA
                CMP R3, M[L2]
                JMP.P PASSA
                CMP R2, M[C1]
                JMP.N PASSA
                CMP R2, M[C2]
                JMP.P PASSA

                INC M[PONTUACAO]                                                ;A PONTUACAO DO JOGADOR AUMENTA

                POP R7

                JMP NOVA_JOGADA                                                 ;PASSA PARA UMA NOVA JOGADA


PASSA:          CMP R3, 1800h                                                   ;COMPARA A POSICAO DA BANANA COM OS LIMITES DA JANELA
                JMP.P NOVA_JOGADA
                CMP R2, 0050h
                JMP.P NOVA_JOGADA


                ADD R3, R2                                                      ;OBTEM AS COORDENADAS DA POSICAO DA BANANA NA NOTACAO PRETENDIDA
                MOV R7, R3

                MOV M[BANANA_ANTERIOR], R7                                      ;GUARDA O VALOR DA POSICAO DA BANANA, DE MODO A QUE ESTA SEJA APAGADA DA PROXIMA VEZ QUE SE PERCORRER O CICLO

                MOV R1, 1
                MOV M[FLAG_2], R1

                POP R3                                                          ;R3 VOLTA A TER O VALOR DA VELOCIDADE
                POP R1                                                          ;R1 VOLTA A TER

                MOV R1, BANANA                                                  ;ESCREVE-SE A BANANA NAS COORDENADAS EM R7
                PUSH R1
                CALL ESCREVER

                INC M[NEW_G2]                                                   ;UMA DAS VARIAVEIS QUE DEFINIRAO A POSICAO DOS GORILAS NA JOGADA SEGUINTE

                POP R7                                                          ;R7 VOLTA A TER O VALOR DO ANGULO

                MOV M[FLAG_TRAJETORIA], R0

                JMP POS_TRAJ

FIM:            BR FIM

;;;;;;;;;;;;;ROTINA PERCORRIDA APENAS QUANDO INTB E PRESSIONADA;;;;;;;;;;;;;;;;;
;;;; APAGA TODO O ECRA E ESCREVE A PONTUACAO OBTIDA (QUE SE ENCONTRA EM     ;;;;
;;;; M[PONTUACAO]). E ADICIONADO O VALOR DO CARACTER ASCII '0' DE MODO A QUE ;;;
;;;; R1 PASSE A CONTER O CARACTER ASCII DA PONTUACAO, PERMITINDO QUE ESTA SEJA ;
;;;; ESCRITA.                                                               ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FIM_JOGO:       CALL APAGAR_INST
                MOV R1, PONTOS
                MOV R7, 1235h
                PUSH R1
                CALL ESCREVER
                MOV R1, M[PONTUACAO]
                CMP R1, 9
                BR.P GANHAR
                ADD R1, '0'
                MOV M[ESCRITA], R1
                BR SAIR
GANHAR:         MOV R1, WINNER
                PUSH R1
                CALL ESCREVER
SAIR:           BR SAIR


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; OBJETIVO DA IMPLEMENTACAO DO TEMPORIZADOR: TODOS OS SEGUNDOS, OCORRE   ;;;;
;;;; UMA INTERRUPCAO QUE AUMENTA UMA DETERMINADA FLAG, QUE FAZ COM QUE SE SAIA ;
;;;; DE UM LOOP E SE CALCULEM AS COORDENADAS CORRESPONDENTES A POSICAO DA   ;;;;
;;;; BANANA EM FUNCAO DO TEMPO CONSIDERANDO A POSICAO INICIAL 0000. A ESSAS ;;;;
;;;; COORDENADAS, SAO DEPOIS SOMADAS AS COORDENDAS DA POSICAO INICIAL DA    ;;;;
;;;; BANANA. AO ESCREVER E APAGAR SUCESSIVAMENTE A BANANA, ESTA DESCREVE-RA A ;;
;;;; TRAJETORIA NA JANELA DE TEXTO DEPENDENDO DA POSICAO DA BANANA NA JANELA ;;;
;;;; DE TEXTO, O JOGADOR GANHARA OU NAO 1 PONTO, REGRESSANDO AO INICIO.     ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;FUNCAO TRAJETORIA;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

TRAJETORIA:     PUSH R0                                                         ;FUNCAO COORDENADA X (VALOR FICA EM R5)
                PUSH R2                                                         ;SALVAGUARDA DE CONTEXTO (PUSH DE )
                PUSH R3                                                         ;(PUSH DA VELOCIDADE)
                PUSH R4                                                         ;(PUSH DO TEMPO)
                PUSH R7                                                         ;(PUSH DO ANGULO)
                PUSH R1
                PUSH R2

                MOV R1, M[SP+4]                                                 ;MOVE-SE PARA R1 O TEMPO
                MOV R2, M[SP+5]                                                 ;MOVE-SE PARA R2 A VELOCIDADE
                PUSH R2
                PUSH R1
                CALL MULTIPLICA
                POP R1                                                          ;RESULTADO EM R1 (R1=VEL*T)
                POP R2                                                          ; R2 = VEL

                MOV R2, M[SP+3]                                                 ; R2 = ANG (EM RAD)
                PUSH R2
                CALL cos
                POP R2                                                          ;R2 = cos(ANG)

                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;R1 * R2 - (VEL*T*cos(ANG))
                POP R1                                                          ;R1 = VELtcos(ANG)
                POP R2
                MOV M[SP+7], R1                                                 ;GUARDA VALOR DE R1 NA STACK

                POP R2
                POP R1
                POP R7                                                          ;R7 = ANG
                POP R4                                                          ;R4 = TEMPO
                POP R3                                                          ;R3 = VEL
                POP R2
                POP R5                                                          ;R5 = V*T*cos(ANG)

                PUSH R0                                                         ;FUNCAO COORDENADA Y (RESULTADO EM R6)
                PUSH R2
                PUSH R3                                                         ;(PUSH DA VELOCIDADE)
                PUSH R4                                                         ;(PUSH DO TEMPO)
                PUSH R7                                                         ;(PUSH DO ANGULO)
                PUSH R1
                PUSH R2
                PUSH R3                                                         ;(PUSH DA VELOCIDADE)

                MOV R2, M[SP+6]                                                 ;R2 = VEL
                MOV R1, M[SP+5]                                                 ;R1 = T
                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(VEL*T)
                POP R1                                                          ;R1 = VEL*T
                POP R2

                MOV R2, M[SP+4]                                                 ;R2 = ANG
                PUSH R2
                CALL sin
                POP R2                                                          ;R2 = sin(ANG)

                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(VEL*T*sin(ANG))
                POP R3                                                          ;R3 = VEL*T*sin(ANG)
                POP R2

                MOV R2, M[SP+5]                                                 ;R2 = T
                MOV R1, M[SP+5]                                                 ;R1 = T
                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(T*T)
                POP R1                                                          ;R1 = T**2
                POP R2

                MOV R2, 100111001101b                                           ;APROX 9.8
                PUSH R1
                PUSH R2
                CALL MULTIPLICA                                                 ;(9.8 * T*T)
                POP R2                                                          ;R2 = g*T*T
                POP R1

                MOV R1, 10000000b                                               ;APROX 0.5
                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(g*T*T*(1/2))
                POP R1                                                          ;R1 = 1/2 * gT*T
                POP R2

                SUB R3, R1                                                      ;R3 = VELtsin(ANG) - 1/2 *g*T*T
                MOV M[SP+8], R3                                                 ;GUARDA VALOR DE R3 NA STACK

                POP R3                                                          ;R3 = VELOCIDADE
                POP R2
                POP R1
                POP R7                                                          ;R7 = ANG
                POP R4                                                          ;R4 = T
                POP R3                                                          ;R3 = VELOCIDADE
                POP R2
                POP R6                                                          ;R6 = VELTsin(ANG) - 1/2 *g*T*T

                RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;FUNCOES AUXILIARES DA PARTE 1 DO PROJETO;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


CONVERT_SHIFT:  PUSH R1                                                         ;CONVERTE NUM DA NOTACAO NORMAL(16/0) PARA A NOTACAO PRETENDIDA(8/8)
                MOV R1, M[SP+3]
                SHL R1, 8
                MOV M[SP+3], R1
                POP R1
                RET


CONVERT_ANG:    PUSH R1                                                         ;CONVERTE AND DE GRAUS PARA RADIANOS
                PUSH R2

                MOV R1, M[SP+4]                                                 ;R1 = ANG
                MOV R2, 110010010b                                              ;APROX PI/2
                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(ANG*PI/2)
                POP R1                                                          ;R1 = ANG*PI/2
                POP R2

                MOV R2, 11b                                                     ;APROX 1/90
                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(ANG*PI/2)*1/90 = ANG EM RADIANOS
                POP R1
                POP R2
                MOV M[SP+4], R1                                                 ;ANG EM RADIANOS GUARDADO NA STACK

                POP R2
                POP R1

                RET

MULTIPLICA:     PUSH R1                                                         ;FUNCAO MULTIPLICA E UMA FUNCAO BASE PARA TODO O PROJETO
                PUSH R2

                MOV R1, M[SP+5]
                MOV R2, M[SP+4]
                MUL R1, R2                                                      ;R1 = RESULTADO DA PARTE INTEIRA, R2 = RESULTADO DA PARTE DECIMAL
                SHR R2, 8                                                       ;PARTE DECIMAL PASSA PARA OS 8 BITS MENOS SIGNIFICATIVOS
                SHL R1, 8                                                       ;PARTE INTEIRA PASSA PARA OS 8 BITS MAIS SIGNIFICATIVOS
                ADD R1, R2                                                      ;OBTEMOS UM NUMERO CUJOS 8 BITS MAIS SIGNIFICATIVOS SAO A PARTE INTEIRA E OS RESTANTES SAO A PARTE DECIMAL
                MOV M[SP+4], R1                                                 ;GUARDAMOS O VALOR NA STACK

                POP R2
                POP R1

                RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;FUNCAO COSSENO;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cos:            PUSH R1                                                         ;SALVAGUARDA DE CONTEXTO
                PUSH R2
                PUSH R3

                MOV R1, M[SP+5]                                                 ;R1 = ANG
                MOV R2, M[SP+5]                                                 ;R2 = ANG
                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(ANG*ANG)
                POP R1                                                          ;R1 = ANG*ANG
                POP R2                                                          ;R2

                MOV R3, 10000000b                                               ;APROX 1/2
                PUSH R1
                PUSH R3
                CALL MULTIPLICA                                                 ;(ANG*ANG)*1/2
                POP R3                                                          ;R3 = ANG*ANG*1/2
                POP R1                                                          ;R1 = ANG*ANG

                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(ANG*ANG*ANG)
                POP R1                                                          ;R1 = ANG*ANG*ANG
                POP R2                                                          ;R2 = ANG

                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(ANG*ANG*ANG*ANG)
                POP R1                                                          ;R1 = ANG*ANG*ANG*ANG
                POP R2                                                          ;R2 = ANG
                MOV R2, 1011b                                                   ;APROX 1/24

                PUSH R2
                PUSH R1
                CALL MULTIPLICA                                                 ;(ANG*ANG*ANG*ANG*1/24)
                POP R1                                                          ;R1 = (ANG**4)/24
                POP R2                                                          ;R2= 1/24
                ADD R1, R3                                                      ;R1= (ANG**2)/2 + (ANG**4)/24
                MOV R2, 100000000b                                              ;APROX 1
                SUB R2, R1                                                      ;R2 = 1 - (ANG**2)/2 + (ANG**4)/24 = cos(ANG)
                MOV M[SP+5], R2                                                 ;GUARDA VALOR DE R2 NA PILHA

                POP R3
                POP R2
                POP R1

                RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;FUNCAO SENO;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

sin:            PUSH R1
                PUSH R2
                PUSH R3

                MOV R1, M[SP+5]                                                 ;R1 = ANG
                MOV R2, M[SP+5]                                                 ;R2 = ANG
                PUSH R2
                PUSH R1
                CALL MULTIPLICA
                POP R1                                                          ;R1 = ANG*ANG
                POP R2                                                          ;R2 = ANG

                PUSH R2
                PUSH R1
                CALL MULTIPLICA
                POP R1                                                          ;R1 = ANG*ANG*ANG
                POP R2                                                          ;R2 = ANG

                MOV R3, 100111b                                                 ;APROX 1/6 = 1/3!
                PUSH R1
                PUSH R3
                CALL MULTIPLICA
                POP R3                                                          ;R3 = ANG*ANG*ANG*1/9
                POP R1                                                          ;R1 = ANG**3

                PUSH R2
                PUSH R1
                CALL MULTIPLICA
                POP R1                                                          ;R1 = ANG**4
                POP R2                                                          ;R2 = ANG

                PUSH R2
                PUSH R1
                CALL MULTIPLICA
                POP R1                                                          ;R1 = ANG**5
                POP R2                                                          ;R2 = ANG

                MOV R2, 10b                                                     ;APROX 1/120
                PUSH R1
                PUSH R2
                CALL MULTIPLICA
                POP R2                                                          ;R2 = (ANG**5)/5!
                POP R1                                                          ;R1 = ANG**5

                ADD R2, R3                                                      ;R2 = ANG*3/3! + ANG*5/5!
                MOV R1, M[SP+5]                                                 ;R1 = ANG
                SUB R1, R2                                                      ;R1 = ANG - ANG*3/3! + ANG*5/5! = sin(ANG)
                MOV M[SP+5], R1

                POP R3
                POP R2
                POP R1

                RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;FUNCOES AUXILIARES DA PARTE 2 DO PROJETO;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;FUNCAO QUE PERCORRE UMA STRING E ESCREVE OS SEUS CARACTERES;;;;;;;;;;

ESCREVER:       MOV R2, M[SP+2]                                                 ;R2 PASSA A CONTER O ENDERECO DO INICIO DA STRING QUE VAI SER ESCRITA
ESCREVER_1:     MOV R1, M[R2]                                                   ;MOVE-SE PARA R1 O CONTEUDO DO ENDERECO
                CMP R1, 0                                                       ;COMPARA-SE O CONTEUDO COM ZERO
                BR.Z FIM_ESCREVER                                               ;SE FOR ZERO VAI PARA FIM_ESCREVER, SE NAO O FOR CONTINUA
                MOV M[CONTROLO], R7                                             ;MOVE-SE R7 (QUE CONTERA AS COORDENADAS ONDE QUEREMOS ESCREVER) PARA O CURSOR
                MOV M[ESCRITA], R1                                              ;ESCREVE-SE O CONTEUDO DO ENDERECO NO LOCAL APONTADO PELO CURSOR
                INC R2                                                          ;AUMENTA-SE EM 1 O ENDERECO (PASSA-SE PARA O ENDERECO QUE CONTEM O PROXIMO CARACTER DA STRING)
                INC R7                                                          ;AUMENTA-SE EM 1 AS COORDENADAS (PARA O PROXIMO CARACTER SER ESCRITO A FRENTE DO ANTERIOR)
                BR ESCREVER_1                                                   ;SALTA PARA A LABEL ESCREVER_1
FIM_ESCREVER:   RETN 1

;;;;;;;;;;;;;;;;;;;;;;;;FUNCAO QUE LIMPA O DISPLAY;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; NA FUNCAO SEGUINTE, MOVE-SE PARA R1 ' ' PORQUE QUEREMOS LIMPAR A JANELA ;;;
;;;; DESTA FORMA, ESCREVEMOS ' ' EM TODOS OS PIXEIS DA JANELA DE TEXTO       ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

APAGAR_INST:    MOV R7, 0000h                                                   ;FUNCAO QUE LIMPA TODA A JANELA DE TEXTO
                MOV R1, ' '                                                     ;MOVE-SE PARA R1 O CARACTER QUE VAI SER ESCRITO EM TODAS AS POSICOES NA JANELA
                MOV R2, 0050h                                                   ;MOVE-SE PARA R2 O LIMITE LATERAL DA JANELA
APAGAR_INST1:   MOV M[CONTROLO], R7                                             ;NO LOCAL INDICADO POR R7, ESCREVE-SE UM ESPAÇO
                MOV M[ESCRITA], R1
                INC R7                                                          ;AUMENTA-SE R7
                CMP R7, 1850h                                                   ;COMPARA-SE R7 COM O MAIOR VALOR QUE O CURSOR PODE TOMAR (UM DOS LIMITES DA JANELA)
                BR.Z APAGAR_INST2                                               ;SE FOR 0, SALTA PARA APAGAR_INST2
                CMP R7, R2                                                      ;COMPARA-SE COM R2
                BR.N APAGAR_INST1                                               ;SE FOR MENOR QUE R2, CONTINUA A PERCORRER A MESMA LINHA
                SUB R7, 0050h
                ADD R7, 0100h                                                   ;SE FOR MAIOR OU IGUAL A R2, R7 PASSA A TER AS COORDENADAS DO 1 PIXEL DA LINHA SEGUINTE
                ADD R2, 0100h                                                   ;ADICIONA A R2 0100h - R2 PASSA A SER O LIMITE LATERAL DA LINHA SEGUINTE
                BR APAGAR_INST1
APAGAR_INST2:   RET


;;;;;;;;;;;;;;;;;;;;;FUNCAO QUE DESENHA OS DOIS GORILAS;;;;;;;;;;;;;;;;;;;;;;;;;

;;;; DESENHO GORILA 1 ;;;;

DESENHO_GORILAS:MOV R7, M[G1]                                                   ;ESCREVE O GORILA DA ESQUERDA
                PUSH R7                                                         ;FICA NA STACK O M[G1] - COORDENADAS INICIAIS DA PRIMEIRA STRING DO GORILA 1
                MOV R1, GORILAL1
                PUSH R1
                CALL ESCREVER                                                   ;ESCREVE A PRIMEIRA STRING DO GORILA (GORILA1)
                POP R7                                                          ;R7 VOLTA A SER O INICIAL (R7 = M[G1])

                ADD R7, 0100h                                                   ;R7 APONTA PARA A LINHA SEGUINTE
                PUSH R7
                MOV R1, GORILAL2
                PUSH R1
                CALL ESCREVER                                                   ;ESCREVE A SEGUNDA STRING DO GORILA (GORILA2)
                POP R7                                                          ;R7 = M[G1] + 0100h

                ADD R7, 0100h                                                   ;R7 APONTA PARA A LINHA SEGUINTE
                PUSH R7
                MOV R1, GORILAL3
                PUSH R1
                CALL ESCREVER                                                   ;ESCREVE A TERCEIRA STRING DO GORILA (GORILA3)
                POP R7

;;;; DESENHO GORILA 2 ;;;;

                MOV R7, M[G2]                                                   ;DESENHO GORILA DA DIREITA - SEMELHANTE AO DA ESQUERDA, DIFERINDO APENAS NO VALOR MOVIDO PARA R7 INICIALMENTE
                PUSH R7                                                         ;NESTE CASO, FICA NA STACK M[G2] - COORDENADAS INICIAIS DA PRIMEIRA STRING DO GORILA 2
                MOV R1, GORILAL1
                PUSH R1
                CALL ESCREVER
                POP R7

                ADD R7, 0100h
                PUSH R7
                MOV R1, GORILAL2
                PUSH R1
                CALL ESCREVER
                POP R7

                ADD R7, 0100h
                PUSH R7
                MOV R1, GORILAL3
                PUSH R1
                CALL ESCREVER
                POP R7

                RET

;;;;;;;;;;;;FUNCAO QUE ESCREVE AS STRINGS 'VELOCIDADE' E 'ANGULO';;;;;;;;;;;;;;;

ESCREVER_VAL:   MOV R7, 0
                PUSH R7
                MOV R1, VALOR1
                PUSH R1
                CALL ESCREVER                                                   ;ESCREVE, COM COORDENADAS INICIAIS 0000h, A STRING COM ENDERECO VALOR1
                POP R7
                ADD R7, 0100h
                MOV R1, VALOR2
                PUSH R1
                CALL ESCREVER                                                   ;ESCREVE, COM COORDENADAS INICIAIS 0100h, A STRING COM ENDERECO VALOR2
                RET

;;;;;;;;;;;;;;;;;;;FUNCAO QUE RECEBE UMA STRING DE 1 CARACTER;;;;;;;;;;;;;;;;;;;

ESCREVER_NUM:   MOV R1, M[SP+2]                                                 ;ESCREVE NA POSICAO INDICADA POR R7 A STRING A QUE FOI FEITO PUSH ANTES DA FUNCAO SER CHAMADA
                MOV M[CONTROLO], R7
                MOV M[ESCRITA], R1
                INC R7                                                          ;AUMENTA R7 (PARA O PROXIMO CARACTER NAO SER ESCRITO POR CIMA DO ANTERIOR)
                SUB R1, '0'                                                     ;R1 FICA COM O VALOR DO NUMERO CORRESPONDENTE A STRING
                MOV M[SP+2], R1                                                 ;MOVE-SE ESSE VALOR PARA O LOCAL DA STACK ONDE ESTAVA A STRING INICIAL
                RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;TRATAMENTO DE INTERRUPCOES;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; AS INTERRUPCOES DA 0 A 9 TEM TRATAMENTOS SEMELHANTES, SENDO A UNICA    ;;;;
;;;; ALTERECAO A STRING QUE E MOVIDA PARA R3, OU SEJA, A STRING QUE QUEREMOS;;;;
;;;; QUE SEJA ESCRITA QUANDO PRESSIONARMOS UMA DETERMINADA INTERRUPCAO      ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


I0:             MOV R1, 1                                                       ;MOVE PARA M[FLAG_NUM] O VALOR 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]                                                   ;AUMENTA O VALOR DE M[FLAG_E]
                MOV R3, '0'                                                     ;MOVE PARA R3 A STRING QUE QUEREMOS QUE SEJA ESCRITA QUANDO PRESSIONARMOS I0
                RTI

I1:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '1'
                RTI

I2:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '2'
                RTI

I3:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '3'
                RTI

I4:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '4'
                RTI

I5:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '5'
                RTI

I6:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '6'
                RTI

I7:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '7'
                RTI

I8:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '8'
                RTI

I9:             MOV R1, 1
                MOV M[FLAG_NUM], R1
                INC M[FLAG_E]
                MOV R3, '9'
                RTI

;;;; TRATAMENTO INTA ;;;;

INICIAR_JOGO:   MOV R1, 1h                                                      ;COLOCA A 1 A M[FLAG]
                MOV M[FLAG_A], R1
                RTI

;;;; TRATAMENTO INTB ;;;;

SAIR_JOGO:      JMP FIM_JOGO

;;;; TRATAMENTO INTF ;;;;

TIMER:          ADD R4, 0008h                                                   ;AUMENTA R4 (REGISTO RESPONSAVEL PELO TEMPO)
                MOV R1, 1
                MOV M[FLAG_TRAJETORIA], R1                                      ;COLOCA A 1 M[FLAG_TRAJETORIA]
                RTI
