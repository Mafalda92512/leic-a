#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>


/* __________________________________________________DECLARACAO FUNCOES__________________________________________________ */

int max(int a, int b);
void appendOutput(char * sender);
int checkin(char * s, int n, int type);
int testInput(char * sender);
void connection();
void storeInfo(int argc, char ** argv);

/* ______________________________________________________________________________________________________________________ */
/* __________________________________________________VARIAVEIS GLOBAIS___________________________________________________ */

char as_ip[21], as_port[6], own_ip[21], own_port[6];  
char uid[6], pw[9];

/* ______________________________________________________________________________________________________________________ */
/* __________________________________________________FUNCOES AUXILIARES__________________________________________________ */

int max(int a, int b){ /* recebe dois inteiros, devolve maior inteiro + 1 */

    if(a > b){
        return a + 1;
    }
    else{
        return b + 1;
    }
}

void appendOutput(char * sender){ /* recebe string, altera string para formato desejado */

    sender[0] = 'R';
    sender[1] = 'E';
    sender[2] = 'G';
    
    strtok(sender, "\n");
    strcat(sender, " ");
    strcat(sender, own_ip);
    strcat(sender, " ");
    strcat(sender, own_port);
    strcat(sender, "\n");
    
}

int checkin(char * s, int n, int type) { // type = 0 - numbers; type = 1 - numbers or char
    int i;
    if (strlen(s)==n) {
        for (int i = 0; i < n ; i++) {
            if ((type == 0) && (s[i] >= '0' && s[i] <= '9')) continue;
            else if ((type == 1) && ((s[i] >= '0' && s[i] <= '9') || 
                                    (s[i] >= 'a' && s[i] <= 'z') ||
                                    (s[i] >= 'A' && s[i] <= 'Z'))) continue;
            else return 0;
        }
    }
    else return 0;
    return 1;
}

int testInput(char * sender){ /* recebe string, devolve inteiro */
    char test[10];
    
    sscanf(sender, "%s %s %s", test, uid, pw);

    /*devolve 2 se o primeiro elemento = "reg", devolve 1 se primeiro elemento = "exit", caso contrario retorna 0 */
    if (strcmp(test, "reg") == 0) {
        if (checkin(uid, 5, 0) && checkin(pw, 8, 1)) return 2;
    }
    else if (strcmp(test, "exit") == 0) return 1;
    return 0;
}

/* ______________________________________________________________________________________________________________________ */
/* _______________________________________________________FUNCOES________________________________________________________ */

void connection(){  /* estabelece ligacao ao AS, troca mensagens */
    int fd, errcode, result, maxfd, nready, fd2;
    char buffer[128], sender[128];
    char vlc[3], userid[5], vlcode[6], type[10], fname[128];
    struct addrinfo hints, *res, hints2, *res2;
    struct sockaddr_in addr;  
    ssize_t n;
    socklen_t addrlen;
    fd_set readfds, writefds;
    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = 5;

    /*SOCKET UDP SET UP - CLIENT UDP*/
    fd = socket(AF_INET, SOCK_DGRAM, 0); /* cria socket_cliente*/
    if(fd == -1) {
        perror("SOCKET ERROR");
        exit(1);
    }

    memset(&hints, 0, sizeof(hints)); /* limpa memoria */
    hints.ai_family = AF_INET; 
    hints.ai_socktype = SOCK_DGRAM;

    errcode= getaddrinfo(as_ip, as_port, &hints,&res); /* associa informacao endereco */
    if(errcode != 0) {
        perror("SOCKET getaddrinfo ERROR");
        exit(1);
    }
    /*______________________________*/
    /*___________REGISTO____________*/

    while(1) { // so sai do while quando se verifica o registo
        
        fgets(sender, sizeof(sender), stdin); /* le input terminal */
        result = testInput(sender); /* verifica input */
        
        if (result == 0) { /* o input do terminal nao esta no formato correto */
            printf("Wrong input format\n");
            continue;
        }
        else if (result == 2) {
            appendOutput(sender); /* altera formato string sender */
            if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,(char*)&tv,sizeof(tv)) < 0){ /* timer 5 seconds */
                printf("setsockopt failed\n");
            } 
            if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO,(char*)&tv,sizeof(tv))<0) {
                printf("setsockopt failed\n");
            }
            n = sendto(fd, sender, strlen(sender), 0, res->ai_addr, res->ai_addrlen); /* envia string sender para AS */
            if(n < 0) {
                perror("sendto ERROR");
                exit(1);
            }
            addrlen = sizeof(addr);
            memset(buffer, 0, sizeof(buffer));
            n=recvfrom(fd, buffer, 128, 0 ,(struct sockaddr*) &addr, &addrlen); /* recebe string buffer AS*/
            if(n < 0) {
                perror("recvfrom ERROR");
                exit(1);
            }
            buffer[n]='\0';
            if(strcmp(buffer, "RRG OK\n") == 0){ /* verifica mensagem recebida */
                printf("Registration Successful\n");
                break;
            }
            else if(strcmp(buffer, "RRG NOK\n") == 0){ /* verifica mensagem recebida */
                printf("Registration Not Successful\n");
            }
            else {
                printf("Received Message Has Wrong Format\n");
            }
        }
        else if (result == 1){ /* leu-se exit, ainda nao se fez registo */
            printf("Successfull Exit Operation\n");
            freeaddrinfo(res);
            close(fd);
            exit(0);   
        }
    }
    /*______________________________*/
    /*SOCKET UDP SET UP - SERVER UDP*/

    fd2 = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd2 == -1) {
        perror("SOCKET FAILURE");
        exit(1);
    }
    memset(&hints2, 0, sizeof(hints2)); /* limpa memoria */
    hints2.ai_family = AF_INET; 
    hints2.ai_socktype = SOCK_DGRAM;
    hints2.ai_flags = AI_PASSIVE;

    errcode= getaddrinfo(NULL, own_port, &hints2,&res2); /* associa informacao endereco */
    if(errcode != 0) {
        perror("SOCKET getaddrinfo FAILURE");
        exit(1);
    }
    n = bind(fd2, res2->ai_addr, res2->ai_addrlen);
    if(n == -1) {
        perror("UDP bind ERROR");
        exit(1);
    }
    /*______________________________*/
    /*____________SELECT____________*/

    maxfd = max(fd2, fd);
    while(1){
        int send = 0;
        FD_ZERO(&readfds); /* limpa memoria readfs */
        /* select */
        FD_SET(0, &readfds); /* ler do stdin */
        FD_SET(fd2, &readfds); /* ler do socket */
        
        nready = select(maxfd, &readfds, &writefds, NULL, NULL);
        if(nready <= 0) {
            perror("SELECT ERROR");
            break;
        }
        if (setsockopt(fd2, SOL_SOCKET, SO_RCVTIMEO,(char*)&tv,sizeof(tv)) < 0){ /* timer 5 seconds */
                printf("setsockopt failed\n");
            } 
        if (setsockopt(fd2, SOL_SOCKET, SO_SNDTIMEO,(char*)&tv,sizeof(tv))<0) {
            printf("setsockopt failed\n");
        }

        for(;nready;nready--){
            if(FD_ISSET(0, &readfds)){ /* vai ler do stdin */
                memset(sender, 0, sizeof(sender));
                fgets(sender, sizeof(sender), stdin); /* le do stdin */
                if(strcmp(sender, "exit\n") == 0){ /* se ler "exit" */
                    memset(sender, 0, sizeof(sender));
                    sprintf(sender, "%s %s %s%s", "UNR" , uid, pw, "\n"); /* Depois desta linha o programa fecha o fd e abre um socket em fd como cliente */
                    close(fd2);
                    n = sendto(fd, sender, strlen(sender), 0, res->ai_addr, res->ai_addrlen); /* envia mensagem ao AS para dar unregister */
                    if(n == -1) {
                        perror("sendto ERROR");
                        exit(1);
                    }
                    memset(buffer, 0, sizeof(buffer));
                    n=recvfrom(fd, buffer, 128, 0 ,(struct sockaddr*) &addr, &addrlen); /* recebe confirmacao unregister */
                    if(n < 0) {
                        perror("recvfrom ERROR");
                        exit(1);
                    }
                    if (strcmp(buffer, "RUN OK\n") == 0) {
                        printf("Successfull Exit Operation\n");
                        freeaddrinfo(res);
                        close(fd);
                        exit(0); 
                    }
                    else if (strcmp(buffer, "RUN NOK\n") == 0) {
                        printf("User Was Not Unregistered. Closing Program\n");
                        freeaddrinfo(res);
                        close(fd);
                        exit(0);
                    }
                    else if(strcmp(buffer, "ERR\n") == 0) {
                        printf("Message Sent Was In An Unexpected Format\n");
                        freeaddrinfo(res);
                        close(fd);
                        exit(0);
                    }
                    else {
                        printf("Message Received Was In An Unexpected Format\n");
                        freeaddrinfo(res);
                        close(fd);
                        exit(0);
                    }
                }
                else { /* caso leia algo nao aplicavel */
                    printf("User Already Registered. Wrong Input Format\n");
                }
            }
            if(FD_ISSET(fd2, &readfds)){ /* vai ler do socket (as)*/
                struct sockaddr_in addr;
                socklen_t addrlen;
                memset(buffer, 0, sizeof(buffer));
                n=recvfrom(fd2, buffer, 128, 0 ,(struct sockaddr*) &addr, &addrlen); /* recebe codigo de validacao */
                if(n < 0) {
                    perror("recvfrom ERROR");
                    exit(1);
                }
                buffer[n] = '\0';
                if(!strcmp(buffer, sender)) continue; /* talvez nao seja preciso */
                sscanf(buffer, "%s %s %s %s %s\n", vlc, userid, vlcode, type, fname);
                if(strcmp(type, "U")==0) strcpy(type, "upload");
                else if(strcmp(type, "L")==0) strcpy(type, "list");
                else if(strcmp(type, "R")==0) strcpy(type, "retrieve");
                else if(strcmp(type, "D")==0) strcpy(type, "delete");
                else if(strcmp(type, "X")==0) strcpy(type, "remove");
                else {
                    printf("Unexistent File Operation\n");
                    strcpy(userid,"-1");
                }
                memset(sender, 0, sizeof(sender));
                strcpy(sender, "RVC ");
                strcat(sender, uid);
                if (strcmp(uid, userid) == 0) {
                    if (strcmp(type, "list") && strcmp(type, "remove")) {
                        printf("VLC=%s %s: %s\n",vlcode, type, fname);                       
                    }
                    else{
                        printf("VLC=%s %s\n",vlcode, type); 
                    }
                    strcat(sender, " OK\n");
                    addrlen = sizeof(addr);
                    n = sendto(fd2, sender, strlen(sender), 0, (struct sockaddr*) &addr, addrlen);
                    send = 1; 
                }
                else {
                    strcat(sender, " NOK\n");
                    addrlen = sizeof(addr);
                    n = sendto(fd2, sender, strlen(sender), 0, (struct sockaddr*) &addr, addrlen);
                    send = 1;
                }                  
            }           
        }        
    }
    /*______________________________*/

}

void storeInfo(int argc, char ** argv){ /* guarda informacao argumentos em variaveis */
    
    for(int i = 0; i< argc; i++){
        if(i == 1) strcpy(own_ip, argv[i]);
        if(strcmp(argv[i], "-d") == 0) strcpy(own_port, argv[i + 1]);
        if(strcmp(argv[i], "-n") == 0) strcpy(as_ip, argv[i + 1]);
        if(strcmp(argv[i], "-p") == 0) strcpy(as_port, argv[i + 1]);
    }

}

/* ______________________________________________________________________________________________________________________ */
/* ___________________________________________________________MAIN_______________________________________________________ */

int main(int argc, char *argv[]){

    storeInfo(argc, argv); 
    connection();

    return 0;
}

/* ______________________________________________________________________________________________________________________ */
