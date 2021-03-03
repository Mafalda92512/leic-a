/* Mafalda Serafim, n 92512 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*_____________________________ CONSTANTES ___________________________________*/

#define MAX_NOME 1024
#define MAX_EMAIL 512
#define MAX_NUM 64
#define HASHTABLE 537
/*____________________________________________________________________________*/


/*____________________________________________________________________________*/
/*______________________________ ESTRUTURAS __________________________________*/
/*____________________________________________________________________________*/

typedef struct Contacto{
  char *nome;
  char *local;
  char *dominio;
  char *num_tlm;
}*pContacto;

/* node da lista de contactos,que contem ponteiro para uma estrutura contacto */
typedef struct node_lst{
  pContacto p_c;
  struct node_lst *prev, *next;
}*link;

/* node de uma das listas da hashtable. contem ponteiro para um node da lista de
contactos */
typedef struct node_hash{
  link pLista;
  struct node_hash *prev, *next;
}*link_h;

typedef struct node_dom {
  char *dominio;
  int num_oc;
  struct node_dom *next;
}*link_dom;

/* lista de nodes, que sera a lista de contactos */
typedef struct Lista_contactos{
  link head;
  link tail;
}*Lst;

typedef struct Lista_dom{
  link_dom head;
}*Lst_dom;

/* estrutura que tera M ponteiros para a estrutura node_hash */
typedef struct Hashtable {
  link_h *heads;
  int M;
}*HT;

/*____________________________________________________________________________*/
/*____________________________________________________________________________*/



/*_________________________ DECLARACAO DE FUNCOES ____________________________*/

/* ________________________ FUNCOES COMANDO */
void Adiciona_Contacto(HT H, Lst L, Lst_dom D);
void Lista_Contactos(Lst L);
void Procura_Contacto(HT H);
void Apaga_Contacto(HT H, Lst L, Lst_dom D);
void Altera_email(HT H, Lst_dom D);
void Num_Ocorrencias(Lst_dom D);

/* ________________________ FUNCOES AUXULIARES */
HT Init_hashtable(int m);
Lst Init_lstcontactos();
Lst_dom Init_lstdom();
unsigned long calc_key(char *s, int m);
pContacto nome_existe(char *s, HT H);
void cria_contacto(char *n, char *e, char *num, Lst L, HT H, Lst_dom D);
void Liberta_mem(HT H, Lst L, Lst_dom D);
/*____________________________________________________________________________*/
/*____________________________________________________________________________*/
/*____________________________________________________________________________*/


/*_________________________________ MAIN _____________________________________*/

int main () {
  char comando = '\0';

  HT Hash_t = Init_hashtable(HASHTABLE);                                        /* inicializa a hashtable */
  Lst Lista_C = Init_lstcontactos();                                            /* inicializa a lista de contactos */
  Lst_dom Lista_D = Init_lstdom();

  while(comando != 'x') {
    switch(comando = getchar()) {                                               /* recebe um caracter que definira a funcao chamada */
      case 'a':
        Adiciona_Contacto(Hash_t, Lista_C, Lista_D);
        break;
      case 'l':
        Lista_Contactos(Lista_C);
        break;
      case 'p':
        Procura_Contacto(Hash_t);
        break;
      case 'r':
        Apaga_Contacto(Hash_t, Lista_C, Lista_D);
        break;
      case 'e':
        Altera_email(Hash_t, Lista_D);
        break;
      case 'c':
        Num_Ocorrencias(Lista_D);
        break;
      case 'x':
        Liberta_mem(Hash_t, Lista_C, Lista_D);
        break;
    }
  }
  return 0;
}
/*____________________________________________________________________________*/
/*____________________________________________________________________________*/

/*___________________________ FUNCOES COMANDO ________________________________*/

/* COMANDO a - funcao recebe um ponteiro para uma hashtable, um para uma lista
de contactos e outro para uma lista de dominios. A funcao guarda espaco para o
nome, email e numero. Apos receber tais parametros, verifica se o nome recebido
ja existe ou nao. Se nao existir, cria um novo contacto com os parametros
recebidos e insere o respetivo node na lista e na hashtable. Se ja existir, faz
print da mensagem de erro. */
void Adiciona_Contacto(HT H, Lst L, Lst_dom D) {
  char *novo_nome = (char*)malloc(sizeof(char)*MAX_NOME);
  char *novo_email = (char*)malloc(sizeof(char)*MAX_EMAIL);
  char *novo_num = (char*)malloc(sizeof(int)*MAX_NUM);

  scanf(" %s %s %s", novo_nome, novo_email, novo_num);
  if (nome_existe(novo_nome, H) == NULL) {
    cria_contacto(novo_nome, novo_email, novo_num, L, H, D);
  }
  else {
    puts("Nome existente.");
  }
  free(novo_nome);
  free(novo_email);
  free(novo_num);
}
/*____________________________________________________________________________*/

/* COMANDO l - Esta funcao recebe uma lista de contactos e faz print dos
parametros de cada um */
void Lista_Contactos(Lst L) {
  link x;
  for (x = L->head; x != NULL ; x = x->next){
    printf("%s %s@%s %s\n", x->p_c->nome, x->p_c->local, x->p_c->dominio,
    x->p_c->num_tlm);
  }

}
/*____________________________________________________________________________*/

/* COMANDO p - recebe uma hashtable. Guarda espaco para um nome, recebe-o e
verifica se existe um contacto com esse nome na hashtable. Se existir, percorre
a lista correspondente a chave do nome ate encontrar o node que leva ao contacto
pedido e faz print dos parametros do mesmo. Caso contrario, faz print da mensagem
de erro */
void Procura_Contacto(HT H) {
  pContacto pCont;
  char *novo_nome = (char*)malloc(sizeof(char)*MAX_NOME);

  scanf(" %s", novo_nome);
  pCont = nome_existe(novo_nome, H);
  if (pCont != NULL) {
    printf("%s %s@%s %s\n", pCont->nome, pCont->local, pCont->dominio, pCont->num_tlm);
  }
  else (printf("%s\n", "Nome inexistente."));
  free(novo_nome);
}
/*____________________________________________________________________________*/

/* COMANDO r - recebe um ponteiro para uma hashtable, um para uma lista de
contactos e outro para uma lista de dominios. Recebe um nome e verifica se
existe um contacto com o mesmo. se existir, percorre a lista da hashtable
correspondente a chave do nome recebido ate encontrar o ponteiro para o contacto
que queremos apagar, libertando o mesmo e diminuindo na lista de dominios o
numero de ocorremcias do mesmo. De seguida, elimina o node correspondente da
lista de contactos e da Hashtable (por essa mesma ordem). Se o nome nao existir,
faz-se print da mensagem de erro.*/
void Apaga_Contacto(HT H, Lst L, Lst_dom D) {
  int verifica = 0;
  link_h t;
  link_dom d;
  int key;
  char *novo_nome = (char*)malloc(sizeof(char)*MAX_NOME);

  scanf(" %s", novo_nome);
  key = calc_key(novo_nome, H->M);
  for (t = H->heads[key]; t != NULL; t = t->next){
    if (!strcmp(t->pLista->p_c->nome, novo_nome)) {
        for (d = D->head; d != NULL; d = d->next) {
          if(!strcmp(t->pLista->p_c->dominio, d->dominio)) {
            if (d->num_oc != 0) d->num_oc--;                                    /* diminui o num de ocorrencias do dominio */
            break;
          }
        }
        free(t->pLista->p_c->nome);                                             /* liberta a memoria alocada para o contacto */
        free(t->pLista->p_c->local);
        free(t->pLista->p_c->dominio);
        free(t->pLista->p_c->num_tlm);
        free(t->pLista->p_c);
        if (t->pLista->prev != NULL && t->pLista->next != NULL) {               /* elimina o node correspondente da lista de contactos */
            t->pLista->prev->next = t->pLista->next;
            t->pLista->next->prev = t->pLista->prev;
            free(t->pLista);
        }
        else if (t->pLista->prev == NULL && t->pLista->next == NULL) {
            free(t->pLista);
            L->head = NULL;
            L->tail = NULL;
        }
        else if (t->pLista->prev == NULL) {
            L->head = t->pLista->next;
            t->pLista->next->prev = NULL;
            free(t->pLista);
        }
        else {
            t->pLista->prev->next = NULL;
            L->tail = t->pLista->prev;
            free(t->pLista);
        }
        if (t->prev != NULL && t->next != NULL) {                               /* elimina o node correspondente da hashtable */
            t->prev->next = t->next;
            t->next->prev = t->prev;
            free(t);
        }
        else if (t->prev == NULL && t->next == NULL) {
            free(t);
            H->heads[key] = NULL;
        }
        else if (t->prev == NULL) {
            H->heads[key] = t->next;
            t->next->prev = NULL;
            free(t);
        }
        else {
            t->prev->next = NULL;
            free(t);
        }
        verifica = 1;
        break;
    }
  }
  if (verifica == 0 ) (puts("Nome inexistente."));
  free(novo_nome);
}
/*____________________________________________________________________________*/

/* COMANDO e - recebe um ponteiro para uma estrutura hashtable e um para uma
lista de dominios. recebe como input um nome e um email. se o nome existir,
diminui-se o numero de ocorrencias do dominio do mesmo e altera-se o local e o
dominio do contacto correspondente para os do email recebido, Aumentando o num
de ocorrencias do dominio deste ultimo. se nao existir, faz-se print da mensagem
de erro. */
void Altera_email(HT H, Lst_dom D) {
  link_dom d1;
  link_dom d2;
  link_dom d3;
  int existe_dom = 0;
  char *novo_nome = (char*)malloc(sizeof(char)*MAX_NOME);
  char *novo_email = (char*)malloc(sizeof(char)*MAX_EMAIL);
  pContacto pCont;
  char *token;

  scanf(" %s %s", novo_nome, novo_email);
  pCont = nome_existe(novo_nome, H);
  if (pCont != NULL) {
    for (d1 = D->head; d1 != NULL; d1 = d1->next) {
        if(!strcmp(pCont->dominio, d1->dominio)) {
            if (d1->num_oc != 0) d1->num_oc--;                                  /* diminui o num de ocorrencias do dominio inicial */
            break;
        }
    }
    token = strtok(novo_email, "@");
    strcpy(pCont->local, token);
    token = strtok(NULL, "@");
    strcpy(pCont->dominio ,token);

    for (d2 = D->head; d2 != NULL; d2 = d2->next) {                             /* aumenta o num de ocorrencias do novo dominio */
        if (!strcmp(d2->dominio, token)) {
            d2->num_oc++;
            existe_dom = 1;
            break;
        }
    }
    if (existe_dom == 0) {
        d3 = (link_dom)malloc(sizeof(struct node_dom));
        d3->dominio = malloc(sizeof(char)*(MAX_EMAIL-1));
        d3->next = D->head;
        D->head = d3;
        strcpy(d3->dominio, token);
        d3->num_oc = 1;
    }
  }
  else (puts("Nome inexistente."));
  free(novo_nome);
  free(novo_email);
}
/*____________________________________________________________________________*/

/* COMANDO c - recebe um ponteiro para uma lista de dominios. percorre todos os
nodes da lista, ate encontrar o dominio pretendido, fazendo print do num de
ocorrencias do mesmo. Se nao encontrar o dominio, considera-se que o num de
ocorencias e zero. */
void Num_Ocorrencias(Lst_dom D) {
  int existe_dom = 0;
  char *novo_dominio = (char*)malloc(sizeof(char)*MAX_EMAIL);
  link_dom x;

  scanf(" %s", novo_dominio);
  for (x= D->head; x != NULL ; x = x->next) {
    if (!strcmp(x->dominio, novo_dominio)) {
      existe_dom = 1;
      printf("%s%s%d\n", novo_dominio, ":", x->num_oc);
      break;
    }
  }
  if (existe_dom == 0) printf("%s%s%d\n", novo_dominio, ":", 0);
  free(novo_dominio);
}
/*____________________________________________________________________________*/

/*____________________________________________________________________________*/
/*____________________________________________________________________________*/


/*____________________________________________________________________________*/

/* funcao que inicializa a hashtable. recebe um inteiro (m). guarda espaco para o
ponteiro da hashtable. de seguida, guarda m espacos para o ponteiro heads e
coloca todos esses ponteiros a NULL. devolve um ponteiro para a hashtable
inicializada */
HT Init_hashtable(int m) {
  int i;
  HT hashtable = (HT)malloc(sizeof(HT));
  hashtable->heads = (link_h*)malloc(m*sizeof(link_h));
  hashtable->M = m;
  for (i = 0; i < hashtable->M ; i++) hashtable->heads[i] = NULL;
  return hashtable;
}
/*____________________________________________________________________________*/

/* funcao que inicializa uma lista de contactos. retorna ponteiro para uma lista
de contactos que inicialmente e NULL */
Lst Init_lstcontactos() {
  Lst lista_contactos = (Lst)malloc(sizeof(Lst));
  lista_contactos->head = NULL;
  lista_contactos->tail = NULL;
  return lista_contactos;
}
/*____________________________________________________________________________*/

/* funcao que inicializa uma lista de dominios. retorna ponteiro para uma lista
de dominios que inicialmente e NULL */
Lst_dom Init_lstdom() {
  Lst_dom lista_dominios = (Lst_dom)malloc(sizeof(Lst_dom));
  lista_dominios->head = NULL;
  return lista_dominios;
}
/*____________________________________________________________________________*/

/* funcao que calcula a chave para a lista de nodes da hashtable onde o contacto
vai ficar. recebe um ponteiro para uma string e um inteiro m.
devolve um inteiro. */
unsigned long calc_key(char *s, int m) {
  unsigned long hash = 5381;
  int c;
  while ((c = *s++))
    hash = 33*hash + c;
  return hash%m;
}
/*____________________________________________________________________________*/

/* funcao recebe um ponteiro para uma string e um para uma hashtable. calcula a
chave do nome e procura na lista correspondente da hashtable por um node que nos
leve ao contacto correspondente ao nome. se tal contacto existir, devolve o
ponteiro para o contacto. se nao existir, retorna NULL*/
pContacto nome_existe(char *s, HT H) {
  int key;
  link_h t;
  key = calc_key(s, H->M);

  if (H->heads[key] == NULL) return NULL;
  else {
    for (t = H->heads[key]; t != NULL; t = t->next){
      if (!strcmp(t->pLista->p_c->nome, s)) return t->pLista->p_c;
    }
    return NULL;
  }
}

/*____________________________________________________________________________*/

/* recebe tres ponteiros para strings, um para uma lista de contactos, um para
uma hashtable e um ultimo para uma lista de dominios. Cria um novo contacto e
dois novos nodes : um com um ponteiro para o contacto que ficara na lista de
contactos e outro com um ponteiro para um node dessa lista que vai ficar na
hashtable. acrescenta cada node no fim da respetiva lista. Ao criar uma nova
estrutura contacto, aumenta-se o numero de ocorrencias do dominio desta ou
acrescenta-se um novo node com o dominio pretendido na lista de dominios, com
numero de ocorrencias 1. */
void cria_contacto(char *n, char *e, char *num, Lst L, HT H, Lst_dom D){
  int key, existe_dom = 0;
  link_h h;
  link_dom d1;
  link_dom d2;
  char *token;
  pContacto pC1 = (pContacto)malloc(sizeof(struct Contacto));
  link p_node1 = (link)malloc(sizeof(struct node_lst));
  link_h p_node2 = (link_h)malloc(sizeof(struct node_hash));
  pC1->nome = malloc(sizeof(char) * (strlen(n)+1));
  pC1->num_tlm = malloc(sizeof(char)*(strlen(num)+1));
  pC1->local = malloc(sizeof(char)*(strlen(e)));                                /* porque o @ nunca fara parte do local ou dominio*/
  pC1->dominio = malloc(sizeof(char)*(strlen(e)));

  strcpy(pC1->nome, n);
  strcpy(pC1->num_tlm,num);
  token = strtok(e, "@");
  strcpy(pC1->local, token);
  token = strtok(NULL, "@");
  strcpy(pC1->dominio ,token);

  if (D->head == NULL) {                                                        /* caso a lista de dominios esteja vazia */
    d1 = (link_dom)malloc(sizeof(struct node_dom));                             /* adiciona o dominio a lista de dominios */
    d1->dominio = malloc(sizeof(char)*(strlen(e)));
    D->head = d1;
    strcpy(d1->dominio, token);
    d1->num_oc = 1;
    d1->next = NULL;
    existe_dom = 1;
  }
  else {
    for (d2 = D->head; d2 != NULL; d2 = d2->next) {                             /* caso o dominio ja esteja presente num dos nodes da lista*/
        if (!strcmp(d2->dominio, token)) {
            d2->num_oc++;                                                       /* aumenta o num de ocorrencias */
            existe_dom = 1;
            break;
        }
    }
  }
  if (existe_dom == 0) {                                                        /* se o dominio nao existir na lista */
    d1 = (link_dom)malloc(sizeof(struct node_dom));
    d1->dominio = malloc(sizeof(char)*(strlen(e)));                             /* cria-se um novo node */
    d1->next = D->head;
    D->head = d1;
    strcpy(d1->dominio, token);
    d1->num_oc = 1;
  }
  p_node1->p_c = pC1;                                                           /* adiciona estrutura contacto na lista de contactos */
  if (L->head == NULL) {
    L->head = p_node1;
    p_node1->next = NULL;
    p_node1->prev = NULL;
    L->tail = p_node1;
  }
  else {
    L->tail->next = p_node1;
    p_node1->next = NULL;
    p_node1->prev = L->tail;
    L->tail = L->tail->next;
  }
  key = calc_key(n, H->M);                                                      /* adiciona estrutura contacto na hashtable */
  if (H->heads[key] == NULL) {
    p_node2->pLista = p_node1;
    H->heads[key] = p_node2;
    p_node2->next = NULL;
    p_node2->prev = NULL;
  }
  else{
    for(h = H->heads[key] ; h->next != NULL; h = h->next);
    p_node2->pLista = p_node1;
    h->next = p_node2;
    p_node2->next = NULL;
    p_node2->prev = h;
  }
}
/*____________________________________________________________________________*/

/* funcao recebe um ponteiro para uma hashtable, um para uma lista de contactos
e outro para uma lista de dominios. As listas sao percorridas, libertando-se a
memoria ocupada por cada node destas, e por fim o ponteiro da mesma. faz-se
depois o mesmo para cada lista da hashtable, libertando-se o ponteiro desta no
final.*/
void Liberta_mem(HT H, Lst L, Lst_dom D) {
    link l, aux_l;
    link_dom d, aux_d;
    link_h h, aux_h;
    int i;

    for (l = L->head; l!= NULL; l = l->next, free(aux_l)) {
        free(l->p_c->nome);
        free(l->p_c->local);
        free(l->p_c->dominio);
        free(l->p_c->num_tlm);
        free(l->p_c);
        aux_l = l;
    }
    free(L);
    for (d = D->head; d!=NULL; d=d->next, free(aux_d)) {
        free(d->dominio);
        aux_d = d;
    }
    free(D);
    for (i=0; i < HASHTABLE; i++) {
        if (H->heads[i] != NULL) {
            for(h = H->heads[i]; h != NULL; h = h->next, free(aux_h)) {
                aux_h = h;
            }
        }
    }
    free(H->heads);
    free(H);
}
