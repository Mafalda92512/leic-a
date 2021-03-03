// Catarina Bento, n 93230 : Mafalda Serafim, n 92512

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include "fs.h"
#include "constants.h"
#include "lib/timer.h"
#include "sync.h"
#include "../client/tecnicofs-client-api.h"
#include "lib/inodes.h"

/*_______________________________ESTRUTURAS___________________________________*/

typedef struct sockArgs{
    int socket;
    uid_t UID;
    int nThread;
} sockArgs;

typedef struct fileInfo {
    char *fileName;
    int iNumber;
    int estado;
    int mode;
} *fileInfo;

/*____________________________________________________________________________*/

char* address = NULL, * global_outputFile = NULL;
int numberThreads = 100, server_fds, dim_server, pid_filho, numberClients, *dim_client;
pthread_rwlock_t commandsLock, vectorLock[INODE_TABLE_SIZE];
tecnicofs* fs;
pthread_t *workers;
sigset_t set;
struct sockaddr_un serverAddress;
struct ucred ucred;
FILE *outputFp;
TIMER_T startTime, stopTime;
// Tabela que refere se o ficheiro cujo iNumber e o indice da tabela esta aberto ou fechado, se for 0 esta fechado
// Serve para nao apagar um ficheiro aberto por outro cliente que nao o proprio
int globalFileVector[INODE_TABLE_SIZE];


/*_________________________________FUNCOES____________________________________*/

void changeVector(int ind, int value){
  pthread_rwlock_wrlock(&vectorLock[ind]);
  globalFileVector[ind] += value;
  pthread_rwlock_wrlock(&vectorLock[ind]);
}

void * trataCliente(void * sockArguments) {
  sigemptyset(&set);
  sigaddset(&set, SIGINT);                                                      // adicionar o sinal SIGINT ao set
  int s = pthread_sigmask(SIG_BLOCK, &set, NULL);                               // bloquear as threads escravas de tratarem o sinal
  if (s != 0) perror("pthread_sigmask");

  struct fileInfo OpenFiles[VECTOR_SIZE];
  int n = 0;
  char buffer[MAX_INPUT_SIZE];
  sockArgs* sock = (sockArgs*)sockArguments;

  n = recv(sock->socket, buffer, MAX_INPUT_SIZE, 0);                            // recebe informacao do cliente
  if(n<0) perror("Erro servidor no read.");

  for(int i; i < VECTOR_SIZE; i++) {                                            // inicializacao do vetor de ficheiros abertos
    OpenFiles[i].estado = 0; //vazio
    OpenFiles[i].fileName= NULL;
    OpenFiles[i].iNumber= -1;
    OpenFiles[i].mode = -1;
  }

  while ( n!= 0 ) {                                                             // ciclo enquanto recebe informacao do cliente
    char token, name[MAX_INPUT_SIZE], rename[MAX_INPUT_SIZE], permissions[2];
    int ownerPerm, fd, othersPerm, iNumber, perm, resultado = 0, permitido, len, mode;

    uid_t * owner = malloc(sizeof(uid_t));
    permission * owPerm = malloc(sizeof(permission));
    permission *otPerm = malloc(sizeof(permission));
    char * newContentFD = malloc(sizeof(char));
    char * charNula = NULL;

    pthread_rwlock_rdlock(&commandsLock);
    sscanf(buffer, "%c %s", &token, name);

    switch (token) {

        case 'c': // cria novo ficheiro
            sscanf(buffer, "%c %s %s", &token, name, permissions);
            pthread_rwlock_unlock(&commandsLock);
            perm = atoi(permissions);
            othersPerm = perm%10;
            ownerPerm = perm/10;

            if (create(fs, name, sock->UID, ownerPerm, othersPerm, -1) == -1) { // cria novo node
              resultado = -4;                                                   //caso já exista um ficheiro com esse nome
            }
            n = send(sock->socket, &resultado, sizeof(int), 0);                 // envia resultado ao cliente
            if(n<0) perror("Erro servidor no send.\n");
            n = recv(sock->socket, buffer, MAX_INPUT_SIZE, 0);                  // recebe nova informacao do cliente
            if(n<0) perror("Erro servidor no receive.\n");
            break;

        case 'd': // apaga ficheiro
            pthread_rwlock_unlock(&commandsLock);
            int searchResult = lookup(fs, name);
            if(searchResult == -1){                                             //Se não existir nenhum ficheiro com esse nome
                resultado = -5;
            }
            else if (globalFileVector[searchResult] != 0) resultado = -9;
            for(int i = 0; i<VECTOR_SIZE; i++){                                 // se o ficheiro estiver aberto, da erro
              if(OpenFiles[i].estado != 0){
                if(!strcmp(OpenFiles[i].fileName, name) && resultado != -5) resultado = -9;
               }
             }


            if(resultado == 0){                                                 // se nao ter erro, apaga o ficheiro da bst e da tabela
              delete(fs, name);
              inode_delete(searchResult);
            }
            n = send(sock->socket, &resultado, sizeof(int), 0);                 // envia resultado ao cliente
            if(n<0) perror("Erro servidor no send.\n");
            n = recv(sock->socket, buffer, MAX_INPUT_SIZE, 0);                  // recebe nova informacao do cliente
            if(n<0) perror("Erro servidor no receive.\n");
            break;

        case 'r': // renomeia ficheiro
            sscanf(buffer, "%c %s %s", &token, name, rename);
            pthread_rwlock_unlock(&commandsLock);

            int inumber = lookup(fs, name);
            if (inumber<0) resultado = -5;                                      // caso o ficheiro para ser renomeado nao exista
            searchResult = lookup(fs, rename);                                  //Caso ja exista um ficheiro com o nome "rename", nao sera possivel renomear
            if(searchResult>0) resultado = -4;

            //obter informacoes do ficheiro, caso nao haja erro
            if(resultado == 0) inode_get(lookup(fs, name), owner, owPerm, otPerm, charNula, 0);
            if(*owner != sock->UID) resultado = -6;

            if(resultado == 0){
                delete(fs, name);                                               // e apagado o ficheiro cujo nome queremos alterar
                create(fs, rename, sock->UID,ownerPerm, othersPerm, iNumber);   // e criado um ficheiro com o mesmo inumber e nome "rename"
                }
            n = send(sock->socket, &resultado, sizeof(int), 0);                 // envia resultado ao cliente
            if(n<0) perror("Erro servidor no send.\n");
            n = recv(sock->socket, buffer, MAX_INPUT_SIZE, 0);                  // recebe nova informacao do cliente
            if(n<0) perror("Erro servidor no receive.\n");
            break;

        case 'o': // abre ficheiro
            sscanf(buffer, "%c %s %d", &token, name, &mode);
            pthread_rwlock_unlock(&commandsLock);

            permitido = 0;
            iNumber = lookup(fs, name);
            inode_get(iNumber, owner, owPerm, otPerm, charNula, 0);
            int ownerPerm = (int) *owPerm;                                      // permissoes do owner do ficheiro
            int othersPerm = (int) *otPerm;                                     // permissoes dos outros utilizadores

            if(*owner == sock->UID){                                            // se o utilizador for o owner do ficheiro
              // tem permissao para abrir caso esta seja RW, ou seja igual ao modo com que quer abrir
              if (ownerPerm == mode || ownerPerm == 3 ) permitido = 1;
              else  resultado = -10;
            }
            else{                                                               // se o utilizador nao for o owner do ficheiro
              if (othersPerm == mode || ownerPerm == 3) permitido = 1;
              else resultado = -10;
            }

            if (permitido == 1){                                                // se o utilizador puder abrir o ficheiro
              for(int i = 0; i<VECTOR_SIZE; i++){
                if(OpenFiles[i].estado == 0){                                   // se existir espaco na tabela de ficheiros
                  // definir os parametros do ficheiro, no vetor de ficheiros abertos
                  OpenFiles[i].fileName = name;
                  OpenFiles[i].iNumber = iNumber;
                  OpenFiles[i].estado = 1;
                  OpenFiles[i].mode = mode;
                  globalFileVector[iNumber] += 1;
                  permitido = 2;
                  resultado= i;                                                 // o valor retornado, fd, sera o indice no vetor de ficheiros abertos
                  break;
                }
              }
              if (permitido != 2) resultado = -7;                               // caso nao haja espaco no vetor
            }
            n = send(sock->socket, &resultado, sizeof(int), 0);                 // envia resultado ao cliente
            if(n<0) perror("Erro servidor no send.\n");
            n = recv(sock->socket, buffer, MAX_INPUT_SIZE, 0);                  // recebe nova informacao do cliente
            if(n<0) perror("Erro servidor no receive.\n");
            break;

        case 'x': // fecha ficheiro
            sscanf(buffer, "%c %d", &token, &fd);
            pthread_rwlock_unlock(&commandsLock);

            if (OpenFiles[fd].estado == 0) resultado = -8;                      // se o ficheiro nao estiver aberto
            else {                                                              // caso o ficheiro esteja aberto, fechamos
              // define parametros de ficheiro fechado
              iNumber = OpenFiles[fd].iNumber;
              globalFileVector[iNumber] -= 1;
              OpenFiles[fd].estado = 0; //vazio
              OpenFiles[fd].fileName= NULL;
              OpenFiles[fd].iNumber= -1;
              OpenFiles[fd].mode = 0;
            }
            n = send(sock->socket, &resultado, sizeof(int), 0);                 // envia resultado ao cliente
            if(n<0) perror("Erro servidor no send.\n");
            n = recv(sock->socket, buffer, MAX_INPUT_SIZE, 0);                  // recebe nova informacao do cliente
            if(n<0) perror("Erro servidor no receive.\n");
            break;

        case 'l': // le ficheiro
            sscanf(buffer, "%c %d %d", &token, &fd, &len);
            pthread_rwlock_unlock(&commandsLock);

            char *fileContents = malloc(len*sizeof(char));
            char *bufferServer = malloc(len*sizeof(char));
            if (OpenFiles[fd].estado == 0) {                                    // se o ficheiro nao estiver aberto, da erro
              resultado = -8;
              fileContents = NULL;
            }
            else if (OpenFiles[fd].mode == 0 || OpenFiles[fd].mode == 1) {      // se o ficheiro nao estiver aberto num modo que permita a leitura
              resultado = -10 ;
              fileContents = NULL;
            }

            else {
              iNumber = OpenFiles[fd].iNumber;
              inode_get(iNumber,owner, owPerm, otPerm, fileContents, len-1);
              int ownerPerm = (int) *owPerm;
              int othersPerm = (int) *otPerm;
              if(*owner == sock->UID){                                          // se o utilizador for o owner do ficheiro
                if (ownerPerm == 2 || ownerPerm == 3) permitido = 1;            // tem permissao para ler caso esta seja RW ou READ
                else {
                  resultado = -10;
                }
              }
              else{                                                             // caso o utilizador nao seja o owner do ficheiro
                if (othersPerm == 2 || othersPerm == 3) permitido = 1;
                else {
                  resultado = -10;
                }
              }
              if (permitido == 1){
                strcpy(bufferServer, fileContents);                             // copiar o conteudo do ficheiro para o buffer a ser enviado
              }
            }
            n = send(sock->socket, &resultado, sizeof(resultado), 0);           // envia resultado ao cliente
            if(n<0) perror("Erro servidor no send.\n");
            if (resultado == 0) {
              n = send(sock->socket, bufferServer, MAX_INPUT_SIZE, 0);          // caso tenha conseguido ler o ficheiro, envia o que leu ao cliente
              if(n<0) perror("Erro servidor no send.\n");
            }
            n = recv(sock->socket, buffer, MAX_INPUT_SIZE, 0);                  // recebe nova informacao do cliente
            if(n<0) perror("Erro servidor no receive.\n");
            break;

        case 'w': // escreve no ficheiro
            sscanf(buffer, "%c %d %s", &token, &fd, newContentFD);
            pthread_rwlock_unlock(&commandsLock);

            if(OpenFiles[fd].estado == 0){                                      // se o ficheiro nao estiver aberto, da erro
              resultado = -8;
            }
            else if (OpenFiles[fd].mode == 0 || OpenFiles[fd].mode == 2) {      // se o ficheiro nao estiver aberto num modo que permita a escrita
              resultado = -10 ;
              fileContents = NULL;
            }

            else{
              inode_get(iNumber, owner, owPerm, otPerm, charNula, 0);
              int ownerPerm = (int) *owPerm;
              int othersPerm = (int) *otPerm;
              if(*owner == sock->UID){                                          // se o utilizador for o owner do ficheiro
                if (ownerPerm == 1 || ownerPerm == 3){                          // tem permissao para escrever caso esta seja RW ou WRITE
                  permitido = 1;
                }
                else {
                  resultado = -10;
                }
              }
              else{                                                             // caso o utilizador nao seja o owner do ficheiro
                if (othersPerm == 1 || ownerPerm == 3){
                  permitido = 1;
                }
                else{
                  resultado = -10;
                }
              }
              if (permitido == 1){
                permitido = 0;
                int iNumber = OpenFiles[fd].iNumber;                            // o inumber do ficheiro que esta aberto nesse indice do vetor
                inode_set(iNumber, newContentFD, strlen(newContentFD));         // mudar o conteudo do ficheiro (escreve no ficheiro)
              }
            }
            n = send(sock->socket, &resultado, sizeof(int), 0);                 // envia resultado ao cliente
            if(n<0) perror("Erro servidor no send.\n");
            n = recv(sock->socket, buffer, MAX_INPUT_SIZE, 0);                  // recebe nova informacao do cliente
            if(n<0) perror("Erro servidor no receive.\n");
            break;

        default: {
            pthread_rwlock_unlock(&commandsLock);
            fprintf(stderr, "Error: commands to apply\n");
            exit(EXIT_FAILURE);
        }
    }
  }
  free(sock);
  return NULL;
}

/*____________________________________________________________________________*/

static void displayUsage (const char* appName){
    printf("Usage: %s socket_name output_filepath numberBuckets\n", appName);
    exit(EXIT_FAILURE);
}

/*____________________________________________________________________________*/

static void parseArgs (long argc, char* const argv[]){
    if (argc != 4) {
        fprintf(stderr, "Invalid format:\n");
        displayUsage(argv[0]);
    }
    address = argv[1];                                                          // adress do socket
    global_outputFile = argv[2];                                                // ficheiro de output
}

/*____________________________________________________________________________*/

FILE *openOutputFile() {
    FILE *fp;
    fp = fopen(global_outputFile, "w");
    if (fp == NULL)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    return fp;
}

/*____________________________________________________________________________*/

int mount(char *address) {                                                      // funcao cria o socket e da bind ao mesmo
  if((server_fds = socket(AF_UNIX, SOCK_STREAM, 0)) == 0){                      // do tipo Stream
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
  }

  unlink(address);                                                              // elimina preventivamente o ficheiro ou socket existente
  bzero((char*)&serverAddress, sizeof(serverAddress));                          // Preencher a estrutura com o nome do socket e dimensao ocupada
  serverAddress.sun_family = AF_UNIX;                                           // dominio da socket e o UNIX
  strcpy(serverAddress.sun_path, address);                                      // copia address para o path do server

  dim_server = sizeof(struct sockaddr);                                         // define a dimensao do server

  if (bind(server_fds, (struct sockaddr*) &serverAddress, dim_server) < 0){     // da-se o nome ao socket
        perror("failed to atribute name to socket");
        exit(EXIT_FAILURE);
  }
  return 0;
}

/*____________________________________________________________________________*/

int listenClient(){                                                             // indica quantos clientes podem esperar pedidos de ligacao
  listen(server_fds, 5);
  return 0;
}

/*____________________________________________________________________________*/

void signalTreatment(){                                                         // funcao tratamento de sinal (Ctrl+c)
  for (int i = 0; i < numberClients; i++) {
        if (pthread_join(workers[i], NULL)) {                                   // join das threads escravas de cada cliente
            perror("Can't join thread");
        }
    }
  TIMER_READ(stopTime);                                                         // para a contagem do tempo
  fprintf(outputFp, "TecnicoFS completed in %.4f seconds.\n", TIMER_DIFF_SECONDS(startTime, stopTime));
  print_tecnicofs_tree(outputFp, fs);                                           // faz print da bst
  free(workers);
  free_tecnicofs(fs);
  pthread_rwlock_destroy(&commandsLock);
  exit(0);
}

/*____________________________________________________________________________*/

void acceptClient() {                                                           // aceita pedidos de ligacao de clientes e cria tarefas escravas para tratar dos mesmos
  workers = (pthread_t*) malloc(numberThreads * sizeof(pthread_t));
  TIMER_READ(startTime);                                                        //Comecar a contagem do tempo acumulado de execucao do servidor

  for(;;){
    int sockfd = accept(server_fds, NULL, NULL);                                // aceita pedido
    if(sockfd<0) perror("Conecção falhada");
    if(sockfd <0){
      perror("erro ao criar ligação dedicada - accept");
      close(sockfd);
    }
    else{
      socklen_t len = sizeof(struct ucred);
      // e criada uma nova estrutura que contem informacao necessaria para a tarefa escrava realizar os pedidos do cliente
      sockArgs *novoSocket = (sockArgs*) malloc(sizeof(struct sockArgs));
      novoSocket->socket = sockfd;
      if (getsockopt(sockfd, SOL_SOCKET, SO_PEERCRED, &ucred, &len) == -1) {    // obtem informacao do user, tal como UID, PID e GID
        perror("Erro ao obter UID");
        close(sockfd);
      }
      else {
        novoSocket->UID = ucred.uid;
        novoSocket->nThread = numberClients;
        // cria uma tarefa escrava para tratar do novo cliente
        int err = pthread_create(&workers[novoSocket->nThread], NULL, trataCliente, (void*) novoSocket);
        numberClients++;                                                        // aumenta numero de clientes
        if (err != 0){
            perror("Can't create thread");
            exit(EXIT_FAILURE);
        }
      }
    }
  }
}

/*___________________________________MAIN_____________________________________*/

int main(int argc, char* argv[]){
  fs = new_tecnicofs();
  parseArgs(argc, argv);
  outputFp = openOutputFile();
  signal(SIGINT, signalTreatment);

  for(int i = 0; i<INODE_TABLE_SIZE; i++){
    pthread_rwlock_init(&vectorLock[i], NULL);
  }
  pthread_rwlock_init(&commandsLock, NULL);
  inode_table_init();

  mount(address);
  listenClient();
  acceptClient();
  return 0;
}
