#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#define PORT "58001"

/* __________________________________________________DECLARACAO FUNCOES__________________________________________________ */

int checkin(char * s, int n, int type);
int testInput(char * sender);
void appendOutput(char * ready, int result);
void printList(char * full_input);
int getInst(char * entry);
int printOut(char * buffer, int wa);
void connection();
void storeInfo(int argc, char ** argv);

/* ______________________________________________________________________________________________________________________ */
/* __________________________________________________VARIAVEIS GLOBAIS___________________________________________________ */

char as_ip[50], as_port[6], fs_ip[50], fs_port[6];
char uid[6], pw[9], action[2], filename[128], RID[5], valcode[5], TID[5], useless[5];

/* ______________________________________________________________________________________________________________________ */
/* __________________________________________________FUNCOES AUXILIARES__________________________________________________ */

int checkin(char * s, int n, int type) { // type = 0 - numbers; type = 1 - numbers or char; type = 2 - char
    int i;
    if (strlen(s)==n) {
        for (int i = 0; i < n ; i++) {
            if ((type == 0) && (s[i] >= '0' && s[i] <= '9')) continue;
            else if ((type == 1) && ((s[i] >= '0' && s[i] <= '9') || 
                                    (s[i] >= 'a' && s[i] <= 'z') ||
                                    (s[i] >= 'A' && s[i] <= 'Z'))) continue;
            else if ((type == 2) && ( (s[i] >= 'a' && s[i] <= 'z') ||
                                    (s[i] >= 'A' && s[i] <= 'Z'))) continue;
            else return 0;
        }
    }
    else return 0;
    return 1;
}

int testInput(char * sender){ /* recebe string, devolve inteiro */
    char command[10], info1[10], info2[10];
    
    sscanf(sender, "%s %s %s", command, info1, info2);

    if (!strcmp("exit", command)) return 0;
    else if (!strcmp("login", command)) {
        if (checkin(info1, 5, 0) && checkin(info2, 8, 1)) {
            strcpy(uid, info1);
            strcpy(pw, info2);
            return 1;
        }
        else return 9;
    }
    else if (!strcmp("req", command)){
        if (checkin(info1, 1, 2)) {
            strcpy(action, info1);
            strcpy(filename, info2); 
            return 2;
        }
        else return 9;
    }
    else if(!strcmp("val", command)) {
        if (checkin(info1, 4, 0)) {
            strcpy(valcode, info1);
            return 3;
        }
        else return 9;
    }
    else if (!strcmp("list", command) || !strcmp("l", command)) {
        return 4;
    }
    else if (!strcmp("retrieve", command) || !strcmp("r", command)){ 
        strcpy(filename, info1);
        return 5;
    }
    else if (!strcmp("upload", command) || !strcmp("u", command)){ 
        strcpy(filename, info1);
        return 6;
    }
    else if (!strcmp("delete", command) || !strcmp("d", command)){
        strcpy(filename, info1);
        return 7;
    }
    else if (!strcmp("remove", command) || !strcmp("x", command)){
        return 8;
    }
    /*devolve 2 se o primeiro elemento = "reg", devolve 1 se primeiro elemento = "exit", caso contrario retorna 0 */
    
    return 0;
}

void printList(char * full_input){
    if (full_input[0] != 'R' || full_input[1] != 'L' || full_input[2] != 'S') {
        return;
    }
    
    if(full_input[4] == 'E' && full_input[5] == 'O'){
        printf("No Files Available!\n");
        return;
    }
    else if(full_input[4] == 'I'){
        printf("Validation error (bad TID)\n");
        return;
    } else if(full_input[4] == 'I'){
        printf("Badly formulated.\n");
        return;
    } else {
        char rls[4], namesize[1000], bof[50];
        int num;
        sscanf(full_input, "%s %d %s",rls, &num, namesize);
        printf("List: ");
        strcpy(bof, strtok(namesize, " "));
        for(int i = 1; i <= num; i++){
            printf("%d- %s\n", i, bof);
            strcpy(bof, strtok(namesize, " "));
        }
        return;
    }
}

void appendOutput(char * ready, int result){ /* recebe string, altera string para formato desejado */

    if (result==1) { /* login */
        strcpy(ready, "LOG ");
        strcat(ready, uid);
        strcat(ready, " ");
        strcat(ready, pw);
        strcat(ready, "\n");
    }
    else if (result==2) { /*req*/ 
        srand(time(NULL)); 
        int rid = rand() % 1000;
        int rid2 = rand() % 10;
        rid = rid + rid2*1000;
        sprintf(RID, "%d", rid);
        strcpy(ready, "REQ ");
        strcat(ready, uid);
        strcat(ready, " ");
        strcat(ready, RID);
        strcat(ready, " ");
        strcat(ready, action);
        if(!strcmp(action, "L") || !strcmp(action, "X")){
            strcat(ready, "\n");
            return;
        }
        strcat(ready, " ");
        strcat(ready, filename);
        strcat(ready, "\n");
    }
    else if (result==3) { /*val*/
        strcpy(ready, "AUT ");
        strcat(ready, uid);
        strcat(ready, " ");
        strcat(ready, RID);
        strcat(ready, " ");
        strcat(ready, valcode);
        strcat(ready, "\n");
    }
    else if (result==4) { /* list, -l*/
        strcpy(ready, "LST ");
        strcat(ready, uid);
        strcat(ready, " ");
        strcat(ready, TID);
        strcat(ready, "\n");
    }
    else if (result==5) { /* retrieve, r*/
        strcpy(ready, "RTV ");
        strcat(ready, uid);
        strcat(ready, " ");
        strcat(ready, TID);
        strcat(ready, " ");
        strcat(ready, filename);
        strcat(ready, "\n");
        
    }
    else if (result==6) { /*upload, u*/
        strcpy(ready, "UPL ");
        strcat(ready, uid);
        strcat(ready, " ");
        strcat(ready, TID);
        strcat(ready, " ");
        strcat(ready, filename);
        // o resto é feito fora
        
    }
    else if (result==7) { /*delete, d*/
        strcpy(ready, "DEL ");
        strcat(ready, uid);
        strcat(ready, " ");
        strcat(ready, TID);
        strcat(ready, " ");
        strcat(ready, filename);
        strcat(ready, "\n");
    }
    else if (result==8) { /* remove, x*/
        strcpy(ready, "REM ");
        strcat(ready, uid);
        strcat(ready, " ");
        strcat(ready, TID);
        strcat(ready, "\n");
    }
    
}

int getInst(char * entry){
    if(!strcmp(entry, "RLS")){
        return 1;
    }
    if(!strcmp(entry, "RRT")){
        return 2;
    }
    if(!strcmp(entry, "RUP")){
        return 3;
    }
    if(!strcmp(entry, "RDL")){
        return 4;
    }
    if(!strcmp(entry, "RRM")){
        return 5;
    }
}

int printOut(char * buffer, int wa){
    char tok[128], status[5], trash[6];
    int size;
    if(wa == 1){
        sscanf(buffer, "%s %s %d %s",trash, status, &size,  tok);
        if(!strcmp(status, "OK")){
            printf("File of size %d:\n", size);
            if (checkin(tok, sizeof(tok), 1)){
                printf("%s\n",tok);
            }
            return 1;
        } else {
            printf("Error: status %s", status);
            return 0;
        }
    } 
    else{
        printf("%s", buffer);
    }
    return 1;
}

/* ______________________________________________________________________________________________________________________ */
/* _______________________________________________________FUNCOES________________________________________________________ */

void connection(){
    int asfd, errcode, result, fsfd;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res, *resfs;
    struct sockaddr_in addr;
    char buffer[128], sender[128], sender_login[128], sender_req[128], sender_val[128];
    char *ptr;
    int nbytes, nleft;
    fd_set readfds;

    /*SOCKET TCP SET UP - CLIENT TCP*/

    asfd = socket(AF_INET, SOCK_STREAM, 0);
    if(asfd == -1) {
        perror("SOCKET ERROR");
        exit(1);
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    errcode= getaddrinfo(as_ip, as_port, &hints,&res); /* associa informacao endereco */    
    if(errcode != 0) {
        perror("SOCKET getaddrinfo ERROR");
        exit(1);
    }
    n = connect(asfd, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        perror("SOCKET connect ERROR");
        exit(1);
    }

    /*______________________________*/
    /*____________LOGIN_____________*/

    while(1) { 
        
        fgets(sender, sizeof(sender), stdin); /* le input terminal */
        result = testInput(sender); /* verifica input */
        
        if (result == 1) {
            memset(sender_login, 0, sizeof(sender_login));
            appendOutput(sender_login, result); /* altera formato string sender */
            nbytes=strlen(sender_login);
            nleft=nbytes;
            ptr=sender_login;
            while (nleft>0){    
                n = write(asfd,ptr, nleft);
                if (n<=0) {
                    perror("WRITE ERROR");
                    exit(1);
                }
                nleft-=n;
                ptr+=n;
            }
            n=read(asfd,buffer,128);
            if (n==-1) {
                perror("READ ERROR");
                exit(1);
            }
            if(strcmp(buffer, "RLO OK\n") == 0){ /* verifica mensagem recebida */
                printf("You are now logged in\n");
                break;
            }
            else if(strcmp(buffer, "RLO NOK\n") == 0) {
                printf("Login failed\n");
            } 
            else {
                printf("Received message has wrong format\n");
            }
        }
        else if (result == 0) { /* leu-se exit */
            freeaddrinfo(res);
            close(asfd);
            printf("Successfull Exit Operation\n");
            exit(0);    
        } 
        else if (result == 9) {
            printf("Wrong Input Format\n");
        }
        else {
            printf("Login required\n");
        }

    }
    
    /*______________________________*/
    /*___________REQUEST____________*/

    while(1) { 
        
        fgets(sender, sizeof(sender), stdin); /* le input terminal */
        result = testInput(sender); /* verifica input */
        
        if (result == 2) {
            appendOutput(sender_req, result); /* altera formato string sender */
            nbytes=strlen(sender_req);
            nleft=nbytes;
            ptr=sender_req;
            while (nleft>0){    
                n = write(asfd,ptr, nleft);
                if (n<=0) {
                    perror("WRITE ERROR");
                    exit(1);
                }
                nleft-=n;
                ptr+=n;
            }
            memset(buffer, 0, sizeof(buffer));
            n=read(asfd,buffer,128);
            if (n==-1) {
                perror("READ ERROR");
                exit(1);
            }
            buffer[n]='\0';
            if(strcmp(buffer, "RRQ OK\n") == 0){ /* verifica mensagem recebida */
                printf("Successfull request\n");
                printf("Check your personal device\n"); 
                break;
            }
            else if(strcmp(buffer, "RRQ NOK\n") == 0) {
                printf("Insuccessfull request\n");
            }
            else {
                printf("Received message has wrong format\n");
            } 
               
        }
        else if (result == 0) { /* leu-se exit */
            freeaddrinfo(res);
            close(asfd);
            printf("Successfull Exit Operation\n");
            exit(0);    
        } 
        else if (result == 1){
            printf("Already logged in.\n");
        } 
        else if (result == 9) {
            printf("Wrong Input Format\n");
        }
        else {
            printf("Need authentication\n");
        }

    }
    
    /*______________________________*/
    /*____________VALCODE___________*/
    
    while(1) {
        
        fgets(sender, sizeof(sender), stdin); /* le input terminal */
        result = testInput(sender); /* verifica input */
        
        if (result == 3) {
            appendOutput(sender_val, result); /* altera formato string sender */
            nbytes=strlen(sender_val);
            nleft=nbytes;
            ptr=sender_val;
            while (nleft>0){    
                n = write(asfd,ptr, nleft);
                if (n<=0) {
                    perror("WRITE ERROR");
                    exit(1);
                }
                nleft-=n;
                ptr+=n;
            }
            memset(buffer, 0, sizeof(buffer));//nao temos a certeza se precisamos disto
            n=read(asfd,buffer,128);
            if (n==-1) {
                perror("READ ERROR");
                exit(1);
            }
            buffer[n]='\0';

            if(buffer[0] == 'R' && buffer[1] == 'A' && buffer[2] == 'U'){ /* verifica mensagem recebida */
                sscanf(buffer, "%s %s", useless, TID );
                if(buffer[4] == '0') {
                    printf("Wrong Validation Code\n");
                }
                else if (checkin(TID, 4, 0)) {
                    printf("Authenticated %s\n", TID);
                    break; 
                }
                else printf("Wrong TID format\n");

            }
            else {
                printf("Received message has wrong format\n");
            }
        }
        else if (result == 0) { /* leu-se exit */
            freeaddrinfo(res);
            close(asfd);
            printf("Successfull Exit Operation\n");
            exit(0);    
        }
        else if (result == 1){
            printf("Already logged in.\n");
        } 
        else if (result == 9) {
            printf("Wrong Input Format\n");
        }
        else {
            printf("Need authentication\n");
        }

    }

    /*______________________________*/
    /*___________USER - FS__________*/
    
    while(1){
        fgets(sender, sizeof(sender), stdin); /* le input terminal */
        result = testInput(sender); /* verifica input */
        if(result >= 4 && result != 6){ // list, retrieve, delete, remove
            memset(sender_val, 0, sizeof(sender_val));
            appendOutput(sender_val, result);

            /*SOCKET TCP SET UP - CLIENT TCP*/

            fsfd = socket(AF_INET, SOCK_STREAM, 0);
            if(fsfd == -1) {
                perror("SOCKET ERROR");
                exit(1);
            }
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            errcode= getaddrinfo(fs_ip, fs_port, &hints,&resfs); /* associa informacao endereco */    
            if(errcode != 0) {
                perror("SOCKET getaddrinfo ERROR");
                exit(1);
            }
            n = connect(fsfd, resfs->ai_addr, resfs->ai_addrlen);
            if(n == -1) {
                perror("SOCKET connect ERROR");
                exit(1);
            }

            /*______________________________*/

            nbytes=strlen(sender_val);
            nleft=nbytes;
            ptr=sender_val;
            while (nleft>0){    
                n = write(fsfd,ptr, nleft);
                if (n<=0) {
                    perror("WRITE ERROR");
                    exit(1);
                }
                nleft-=n;
                ptr+=n;
            }
            memset(buffer, 0, sizeof(buffer));//nao temos a certeza se precisamos disto
            char *full_input;
            int count = 1, total_bits;
            int type;
            if (n=read(fsfd,buffer, 128)>0) {
                char init[4], rest[11], pos[128];
                full_input = (char*) malloc(sizeof(buffer) * count);
                if (!full_input) {
                    perror("Error allocating memory.\n");
                    exit(1);
                }
                total_bits = n;
                sscanf(buffer, "%s %s\n", init, rest);
                type = getInst(init);
                strcpy(full_input, buffer);
                if(type == 2 && printOut(buffer, 1) != 1) continue;
                count++;
            }
            if (n==-1) {
                perror("READ ERROR");
                exit(1);
            }
            if ((n < 128) && (type != 2)){    
                if(type == 1) printList(full_input);
                else if(type > 2){
                    char stat[5], rest[6];
                    sscanf(full_input, "%s %s", rest, stat);    
                    printf("Status of operation is %s\n", stat);
                }
                freeaddrinfo(resfs);
                close(fsfd);
                if (type == 5) {
                    close(asfd);
                    exit(0);
                }
                continue;
            }
            
            memset(buffer, 0, sizeof(buffer));
            while (n=read(fsfd,buffer, 128) != 0) { // if there is more to read
                if (n==-1) {
                    perror("Error reading from user.\n");
                    exit(1);
                }
                if(type == 1) full_input = (char *) realloc(full_input, sizeof(buffer) * count);
                if (!full_input) {
                    perror("Error allocating memory.\n");
                    exit(1);
                }
                if(type == 2) printOut(buffer, 2);
                if(type == 1) strcat(full_input, buffer);
                memset(buffer, 0, sizeof(buffer));
                total_bits += n;
                count ++;
            }
            if (n==-1) {
                perror("READ ERROR");
                exit(1);
            }
            if(type == 1){
                printList(full_input);
            }
            freeaddrinfo(resfs);
            close(fsfd);
        } 
        else if (result == 2){
            appendOutput(sender_req, result); /* altera formato string sender */
            nbytes=strlen(sender_req);
            nleft=nbytes;
            ptr=sender_req;
            while (nleft>0){    
                n = write(asfd,ptr, nleft);
                if (n<=0) {
                    perror("WRITE ERROR");
                    exit(1);
                }
                nleft-=n;
                ptr+=n;
            }
            memset(buffer, 0, sizeof(buffer));
            n=read(asfd,buffer,128);
            if (n==-1) {
                perror("READ ERROR");
                exit(1);
            }
            buffer[n]='\0';
            if(strcmp(buffer, "RRQ OK\n") == 0){ /* verifica mensagem recebida */
                printf("Successfull request\n");
                printf("Check your personal device\n");
            }
        }
        else if (result == 0) {   /* leu-se exit */
            freeaddrinfo(res);
            close(asfd);
            printf("Successfull Exit Operation\n");
            exit(0);    
        } 
        else if (result == 1){
            printf("Already logged in.\n");
        } 
        else if (result == 3) {
            appendOutput(sender_val, result); /* altera formato string sender */
            nbytes=strlen(sender_val);
            nleft=nbytes;
            ptr=sender_val;
            while (nleft>0){    
                n = write(asfd,ptr, nleft);
                if (n<=0) {
                    perror("WRITE ERROR");
                    exit(1);
                }
                nleft-=n;
                ptr+=n;
            }
            memset(buffer, 0, sizeof(buffer));//nao temos a certeza se precisamos disto
            n=read(asfd,buffer,128);
            if (n==-1) {
                perror("READ ERROR");
                exit(1);
            }
            buffer[n]='\0';
            if(buffer[0] == 'R' && buffer[1] == 'A' && buffer[2] == 'U'){ /* verifica mensagem recebida */
                memset(TID, 0, sizeof(TID));
                sscanf(buffer, "%s %s", useless, TID );
                if(!strcmp(TID, "0")) {
                    printf("TID received = 0\n");
                }
                if (checkin(TID, 4, 0)) {
                    printf("Authenticated %s\n", TID);
                }
                else printf("Wrong TID Format");
            }
        }
        else if (result == 6){
            int size;
            char * fullSend, *text;
            char sz[6];
            FILE * fp;
            fsfd = socket(AF_INET, SOCK_STREAM, 0);
            if(fsfd == -1) {
                perror("SOCKET ERROR");
                exit(1);
            }
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;

            errcode= getaddrinfo(fs_ip, fs_port, &hints,&resfs); /* associa informacao endereco */    
            if(errcode != 0) {
                perror("SOCKET getaddrinfo ERROR");
                exit(1);
            }

            n = connect(fsfd, resfs->ai_addr, resfs->ai_addrlen);
            if(n == -1) {
                perror("SOCKET connect FAILURE");
                exit(1);
            }
            fp = fopen(filename, "r");
            fseek(fp, 0L, SEEK_END);
            size = ftell(fp);
            rewind(fp);
            sprintf(sz, "%d", size);
            text = malloc(size * sizeof(char));
            fullSend = malloc((128 + size) * sizeof(char));
            if(text){
                fread(text, 1, size, fp);
            }
            appendOutput(fullSend, result); /* altera formato string sender */
            strcat(fullSend, " ");
            strcat(fullSend, sz);
            strcat(fullSend, " ");
            strcat(fullSend, text);
            nbytes=strlen(fullSend);
            nleft=nbytes;
            ptr=fullSend;
            while (nleft>0){ 
                n = write(fsfd,ptr, nleft);
                if (n<=0) {
                    perror("WRITE ERROR");
                    exit(1);    
                }
                nleft-=n;
                ptr+=n;
            }
            memset(buffer, 0, sizeof(buffer));//nao temos a certeza se precisamos disto
            
            n=read(fsfd,buffer,128);
            if (n==-1) {
                perror("READ ERROR");
                exit(1);
            }
            if(buffer[4] == 'O'){
                printf("Sucessfully uploaded %s!\n", filename);
            } else if(buffer[4] == 'F'){
                printf("Maximum number of Files uploaded!\n");
            } else if(buffer[4] == 'I'){
                printf("Validation Error in TID!\n");
            } else if(buffer[4] == 'E'){
                printf("Request not correctly formulated\n");
            } else if(buffer[4] == 'D'){
                printf("File already exists!\n");
            }
            free(fullSend);
            free(text);
            freeaddrinfo(resfs);
            close(fsfd);
        }
    }

    freeaddrinfo(res);
    close(asfd);
    
}

void storeInfo(int argc, char ** argv){ /* guarda informacao argumentos em variaveis */
    
    for(int i = 0; i< argc; i++){
        if(strcmp(argv[i], "-n") == 0) strcpy(as_ip, argv[i + 1]);
        if(strcmp(argv[i], "-p") == 0) strcpy(as_port, argv[i + 1]);
        if(strcmp(argv[i], "-m") == 0) strcpy(fs_ip, argv[i + 1]);
        if(strcmp(argv[i], "-q") == 0) strcpy(fs_port, argv[i + 1]);

    }
    if(!as_ip) strcpy(as_ip, "127.0.0.1");
    if(!fs_ip) strcpy(fs_ip, "127.0.0.1");

}

/* ______________________________________________________________________________________________________________________ */
/* ___________________________________________________________MAIN_______________________________________________________ */

int main(int argc, char *argv[]){

    storeInfo(argc, argv); 
    connection();

    return 0;
}

/* ______________________________________________________________________________________________________________________ */

