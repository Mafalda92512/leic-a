// Catarina Bento, n 93230 : Mafalda Serafim, n 92512

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include "../server/constants.h"
#include "tecnicofs-client-api.h"
#include "tecnicofs-api-constants.h"

int client_fds;
struct sockaddr_un serverAddress;

/*__________________________________FUNCOES___________________________________*/

int tfsMount(char * address){
  if((client_fds = socket(AF_UNIX, SOCK_STREAM, 0)) == 0){                      // criar socket no lado do cliente
        perror("socket failed");
        exit(EXIT_FAILURE);
  }

  // Preencher a estrutura com o nome do socket e dimensao ocupada
  bzero((char *)&serverAddress, sizeof(serverAddress));

  serverAddress.sun_family = AF_UNIX;                                           // dominio da socket e o UNIX
  strcpy(serverAddress.sun_path, address);                                      // copia address para o path do server

  // define a dimensao do server
  int dim_server = strlen(serverAddress.sun_path)+sizeof(serverAddress.sun_family);
  // tenta conectar-se com o cliente
  if((connect(client_fds, (const struct sockaddr*)&serverAddress, dim_server) < 0)){
        int error = errno;
        perror("socket failed");
        if (error == EISCONN) return -1;
        return -3;
  }
  return 0;

}

/*____________________________________________________________________________*/

int tfsUnmount(char * address){
  int c = close(client_fds); // close da socket usada para comunicar com o servidor
  if (c == -1) perror("socket close failed");
  return 0;
}

/*____________________________________________________________________________*/

int tfsCreate(char *filename, permission ownerPermissions, permission othersPermissions){
  char bufferCreate[MAX_INPUT_SIZE];
  int Permissions = ownerPermissions*10 + othersPermissions;
  int resposta;

  sprintf(bufferCreate, "c %s %d", filename, Permissions);
  int n = strlen(bufferCreate)+1;
  if(send(client_fds, bufferCreate, n, 0)!=n){                                  // envia "c filename permissions"
    int error = errno;
    if(error == ENOTCONN) return -2;
    perror("erro no write.");
  }
  n = recv(client_fds, &resposta, sizeof(int), 0);
  int error = errno;
  if(error == ENOTCONN) return -2;
  if(n<0) perror("erro no receive");
  return resposta;
}

/*____________________________________________________________________________*/

int tfsDelete(char *filename) {
  char bufferDelete[MAX_INPUT_SIZE];
  int resposta;

  sprintf(bufferDelete, "d %s", filename);
  int n = strlen(bufferDelete) + 1;
  if(send(client_fds, bufferDelete, n, 0)!=n){                                  // envia "d filename"
    int error = errno;
    if(error == ENOTCONN) return -2;
    perror("erro no write.");
  }
  n = recv(client_fds, &resposta, sizeof(int), 0);
  int error = errno;
  if(error == ENOTCONN) return -2;
  if(n<0) perror("erro no receive");
  return resposta;
}

/*____________________________________________________________________________*/

int tfsRename(char *filenameOld, char *filenameNew){
  char bufferRename[MAX_INPUT_SIZE];
  int resposta;

  sprintf(bufferRename, "r %s %s", filenameOld, filenameNew);
  int n = strlen(bufferRename) + 1;
  if(send(client_fds, bufferRename, n, 0)!=n){                                  // envia "r filenameOld filenameNew"
    int error = errno;
    if(error == ENOTCONN) return -2;
    perror("erro no write.");
  }
  n = recv(client_fds, &resposta, sizeof(int), 0);
  int error = errno;
  if(error == ENOTCONN) return -2;
  if(n<0) perror("erro no receive");
  return resposta;
}

/*____________________________________________________________________________*/

int tfsOpen(char * filename, permission mode){
  char bufferOpen[MAX_INPUT_SIZE];
  int resposta;

  sprintf(bufferOpen, "o %s %d", filename, mode);
  int n = strlen(bufferOpen) + 1;
  if(send(client_fds, bufferOpen, n, 0)!=n){                                    // envia "o filename mode"
    int error = errno;
    if(error == ENOTCONN) return -2;
    perror("erro no write.");
  }
  n = recv(client_fds, &resposta, sizeof(int), 0);
  int error = errno;
  if(error == ENOTCONN) return -2;
  if(n<0) perror("erro no receive");
  return resposta;
}

/*____________________________________________________________________________*/

int tfsClose(int fd) {
  char bufferClose[MAX_INPUT_SIZE];
  int resposta;

  sprintf(bufferClose, "x %d", fd);
  int n = strlen(bufferClose) + 1;
  if(send(client_fds, bufferClose, n, 0)!=n){                                   // envia "x fd"
    int error = errno;
    if(error == ENOTCONN) return -2;
    perror("erro no write.");
  }
  n = recv(client_fds, &resposta, sizeof(int), 0);
  int error = errno;
  if(error == ENOTCONN) return -2;
  if(n<0) perror("erro no receive");
  return resposta;
}

/*____________________________________________________________________________*/

int tfsRead(int fd, char*buffer, int len) {
  char bufferRead[MAX_INPUT_SIZE];
  char bufferReceive[MAX_INPUT_SIZE];
  int resposta;

  sprintf(bufferRead, "l %d %d", fd, len);
  int n = strlen(bufferRead) + 1;
  if(send(client_fds, bufferRead,  n, 0)!=n){                                   // envia "l fd len"
  int error = errno;
  if(error == ENOTCONN) return -2;
    perror("erro no write.");
  }

  n = recv(client_fds, &resposta, sizeof(resposta), 0);
  int error = errno;
  if(error == ENOTCONN) return -2;
  if(n<0) perror("erro no receive");

  if (resposta == 0) {                                                          // caso nao existam erros, recebe o que leu do ficheiro
    n = recv(client_fds, bufferReceive, MAX_INPUT_SIZE, 0);
    error = errno;
    if(error == ENOTCONN) return -2;
    if(n<0) perror("erro no receive");
    return strlen(bufferReceive);
  }
  return resposta;
}

/*____________________________________________________________________________*/

int tfsWrite(int fd, char*buffer, int len) {
  char bufferWrite[MAX_INPUT_SIZE];
  int resposta;

  sprintf(bufferWrite, "w %d %s", fd, buffer);
  int n = strlen(bufferWrite) + 1;
  if(send(client_fds, bufferWrite, n, 0)!=n){                                   // envia "w fd buffer"
    int error = errno;
    if(error == ENOTCONN) return -2;
    perror("erro no write.");
  }
  n = recv(client_fds, &resposta, sizeof(int), 0);
  int error = errno;
  if(error == ENOTCONN) return -2;
  if(n<0) perror("erro no receive");
  return resposta;

}
