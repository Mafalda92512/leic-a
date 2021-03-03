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
#include <dirent.h>
#include <sys/stat.h>
#include <signal.h>

#define MAXUSERS 100

typedef struct User{
    int fd;
}User; 

char as_ip[21], as_port[6], own_ip[21], own_port[6];  /* variaveis argumentos */
char request[4];                             /*com varios users deve ter de mudar*/
int verbose, fsport, asip, asport, max_sd;
fd_set readfds;

User users[MAXUSERS];

int n; //var global que mantem o numero de users

void appendOutput(char * sender, int uid, int tid);
int testInput(char *buffer);
void connection();
void storeInfo(int argc, char ** argv);
char * operation(char *buffer, int i, int uid, int tid, char * fop, char * filename);
char * list(char *dirname, int i);
char * retrieve(char *buffer, char *dirname, int i, char*filename);
char * upload(char *buffer, char *dirname, int i);
char * delete(char *buffer, char *dirname, int i, char*filename);
char * remove_user(char *buffer, char *dirname, int i);

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
void storeInfo(int argc, char ** argv){ /* guarda informacao argumentos em variaveis */
    
    for(int i = 0; i< argc; i++){
        if (strcmp(argv[i], "-q")==0){
            strcpy(own_port, argv[i+1]);
            fsport=1;
        }
        if(strcmp(argv[i], "-n") == 0){
            strcpy(as_ip, argv[i + 1]);
            asip=1;
        }
        if(strcmp(argv[i], "-p") == 0){
            strcpy(as_port, argv[i + 1]);
            asport=1;
        }
        if (strcmp(argv[i], "-v")==0) verbose=1;
    }
    if (fsport==0) strcpy(own_port, "59021");
    if (asport==0) strcpy(as_port, "58021");
    if (asip==0) strcpy(as_ip, "127.0.0.1");

}

void appendOutput(char * sender, int uid, int tid){ /* recebe string, altera string para formato desejado*/

    bzero(sender, sizeof(sender));

    sprintf(sender, "VLD %d %d\n", uid, tid);
    
}
int testInput(char *buffer){
    char op[4];

    sscanf(buffer, "%s", op);

    if (strcmp(op, "LST")==0) return 1;
    if (strcmp(op, "RTV")==0) return 2;
    if (strcmp(op, "UPL")==0) return 3;
    if (strcmp(op, "DEL")==0) return 4;
    if (strcmp(op, "REM")==0) return 5;
    else return -1;
}

char * operation(char *buffer, int sd, int uid, int tid, char * fop, char * filename){ 
    int op;
    char userdir[22];           // 'USERS/UID1' UID1 tem 5 digitos
    op=testInput(buffer);
    sprintf(userdir, "FILES/%d", uid); //userdir fica 'USERS/uid' do user que queremos

    switch(op){

        case 1:             //list
            return list(userdir, sd);
        case 2:
            return retrieve(buffer, userdir, sd, filename);
        case 3:
            return upload(buffer, userdir, sd);
        case 4:
            return delete(buffer, userdir, sd, filename);
        case 5:
            return remove_user(buffer, userdir, sd);
        default:
            return "ERR\n";
    }


}

char * list(char *dirname, int sd){
    int filecount=0, nleft=0, nwritten=0, fsize;
    char *ptr, *fullSend, fname[35], sz[11], file[30];
    DIR *d;
    FILE *fp;
    struct dirent *dir, *count;
    struct stat st;

    d=opendir(dirname);

    if (d){
        while ((count=readdir(d))!=NULL){
            filecount++;
        }
        filecount = filecount-2;
        fullSend = (char *) malloc(sizeof(char) * 60 * filecount);
        sprintf(fullSend, "RLS %d", filecount);
        rewinddir(d);
        while ((dir=readdir(d))!=NULL){

            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
                continue;
            sprintf(file, "%s/%s", dirname, dir->d_name);
            fp = fopen(file, "r");
            fseek(fp, 0, SEEK_END);
            fsize = ftell(fp);
            sprintf(sz, "%d", fsize);
            sprintf(sz, "%d", fsize);
            strcat(fullSend, " ");
            strcat(fullSend, dir->d_name);
            strcat(fullSend, " ");
            strcat(fullSend, sz);

        }
        
        strcat(fullSend, "\n");
        nleft = strlen(fullSend);
        ptr=fullSend;
        while(nleft>0){                                     //writes filename to user
            nwritten=write(sd, ptr, nleft); 
            if (nwritten<=0){
                perror("Error writing back to user\n");
                return "ERR\n";
            }
            nleft-=nwritten;
            ptr+=nwritten;
        }
        free(fullSend);
        closedir(d);
    }
    fullSend = (char *) malloc(sizeof(char) * 9);
    //else status EOF se nao ha files
    sprintf(fullSend, "RLS EOF\n");
    nleft = strlen(fullSend);
    ptr=fullSend;
    while(nleft>0){                                     //writes RLS EOF to user
        nwritten=write(sd, ptr, nleft); 
        if (nwritten<=0){
            perror("Error writing back to user\n");
            return "ERR\n";
        }
        nleft-=nwritten;
        ptr+=nwritten;
    }
    free(fullSend);
    return "NO FORMAT ERROR";
}

char * retrieve(char *buffer, char *dirname, int sd, char*filename){
    char sz[6], answer[10], file[30], fullSend[128], text[1024];
    int size, nbytes, n, nleft, m;
    char *ptr;
    DIR *d;
    struct dirent *dir;
    FILE * fp;
    struct stat st;
    int left;

    

    d=opendir(dirname);

    if (d){
        while ((dir=readdir(d))!=NULL){
            if (strcmp(dir->d_name, filename)==0){
                sprintf(file, "%s/%s", dirname, filename);
                fp = fopen(file, "r");
                fseek(fp, 0, SEEK_END);
                size = ftell(fp);
                rewind(fp);
                sprintf(sz, "%d", size);
                sprintf(fullSend, "RRT OK %d ", size);
                nbytes=strlen(fullSend);
                nleft=nbytes;
                ptr=fullSend;
                while (nleft>0){ 
                    n = write(sd,ptr, nleft);
                    if (n<=0) {
                        perror("Error writing to user.\n");
                        exit(1);    
                    }
                    nleft-=n;
                    ptr+=n;
                }
                size_t nbytes = 0;  
                while((nbytes = fread(text, sizeof(char), 1024, fp)) > 0) {
                    int offset = 0, sent;
                    while ((sent = send(sd, text + offset, nbytes, 0)) > 0) {
                            offset += sent;
                            nbytes -= sent;
                    }
                    bzero(text, sizeof(text));                   
                }
                strcpy(text, "\n");
                if(write(sd, text, 3) <= 0){
                    perror("Error writing to user.\n");
                    exit(1);
                }
                fclose(fp);
                closedir(d);
                return "NO FORMAT ERROR";
            }
        }
        sprintf(answer, "RRT EOF\n");       //If file is not available
        nleft = strlen(answer);
        ptr=answer;
        while(nleft>0){                                     //writes RTV EOF to user
            n=write(sd, ptr, nleft); 
            if (n<=0){
                perror("Error writing back to user\n");
                exit(1);
            }
            nleft-=n;
            ptr+=n;
        }
        closedir(d);
    }
    else{
        sprintf(answer, "RRT NOK\n");       //If there is no content available for this user
        nleft = strlen(answer);
        ptr=answer;
        while(nleft>0){                                     //writes RTV NOK to user
            n=write(sd, ptr, nleft); 
            if (n<=0){
                perror("Error writing back to user\n");
                exit(1);
            }
            nleft-=n;
            ptr+=n;
        }
    }
    closedir(d);
    return "NO FORMAT ERROR";
}

char * upload(char *buffer, char *dirname, int sd){
    char fname[25], sz[6], bigBuffer[1024], content[30], answer[13], *ptr, char_size[10];
    int m, size, nleft, flag;
    DIR *d;
    struct dirent *dir;
    int left;
    int bytes_size = 0;
    bzero(buffer,sizeof(buffer));
    m = read(sd, buffer, 40);
    if(m != 0){
        if(m == -1){
            perror("Error in reading from user.\n");
            exit(1);
        }
        sscanf(buffer, "%s %s %s", fname, char_size, content);
        int forward = strlen(fname) + strlen(char_size) +2;
        memcpy(content, buffer + forward, m - strlen(fname) - strlen(char_size) -2);
    }
    sscanf(char_size, "%d", &size);

    left = size - m + strlen(fname) + strlen(char_size) + 2;
    int chunk = strlen(content);
    int count;
    d=opendir(dirname);
    if (d){
        while ((dir=readdir(d))!=NULL){
            count++;
            if (strcmp(dir->d_name, fname)==0){
                strcpy(answer, "RUP DUP\n");
                flag = 1;
                break;
            } 
        }
        if (count == 17){

            closedir(d);
            strcpy(answer, "RUP FULL\n");
            nleft = strlen(answer);
            ptr=answer;
            while(nleft>0){                                     //writes RTV EOF to user
                n=write(sd, ptr, nleft); 
                if (n<=0){
                    perror("Error writing back to user\n");
                    exit(1);
                }
                nleft-=n;
                ptr+=n;
            }
            return "NO FORMAT ERROR";
        }
    }
    else {
        if(mkdir(dirname, 0777) == -1) {
            perror("ERROR IN CREATING DIRECTORY FILES");
            exit(1);
        }
    }
    if(flag == 1){

        while((m = read(sd, buffer, 40)) != 0){      //IF file exists clear socket and send DUP
            if(m == -1){
                perror("Error in reading from user.\n");
                exit(1);
            }
        }
        bzero(buffer, sizeof(buffer));
        strcpy(answer, "RUP DUP\n");
        nleft = strlen(answer);
        ptr=answer;
        while(nleft>0){                                
            n=write(sd, ptr, nleft); 
            if (n<=0){
                perror("Error writing back to user\n");
                exit(1);
            }
            nleft-=n;
            ptr+=n;
        }
        return "NO FORMAT ERROR";
    }
    char totalname[50];

    sprintf(totalname, "%s/%s", dirname, fname);

    FILE * file = fopen(totalname, "w");
    if(!file){
        perror("Error creating file.\n");
        exit(1);
    }
    fprintf(file, "%s", content);
    fclose(file);

    file = fopen(totalname, "a");
    if(!file){
        perror("Error creating file.\n");
        exit(1);
    }

    bzero(bigBuffer, sizeof(bigBuffer));
    while (left > 0) {
        m = read(sd, bigBuffer, 1024);
        if (m > 0) {
            if(m == -1){
                perror("Error in reading from user.\n");
                exit(1);
            }
            fprintf(file, "%s", bigBuffer);
            left = left - m;
            chunk = chunk + strlen(bigBuffer);
            bzero(bigBuffer, sizeof(bigBuffer));
        }
        else {
            m = read(sd, bigBuffer, 1024);
            if (m <= 0) break;
            else {
                if(m == -1){
                    perror("Error in reading from user.\n");
                    exit(1);
                }
                fprintf(file, "%s", bigBuffer);
                left = left - m;
                chunk = chunk + strlen(bigBuffer);
                bzero(bigBuffer, sizeof(bigBuffer));
            }
        }
    }

    fclose(file);
    strcpy(answer, "RUP OK\n");
    nleft = strlen(answer);
    ptr=answer;
    while(nleft>0){                                
        n=write(sd, ptr, nleft); 
        if (n<=0){
            perror("Error writing back to user\n");
            exit(1);
        }
        nleft-=n;
        ptr+=n;
    }
    return "NO FORMAT ERROR";
}

char * delete(char *buffer, char *dirname, int sd, char*filename){
    char answer[11], file[60];
    int size, nbytes, n, nleft, m;
    char *ptr;
    DIR *d;
    struct dirent *dir;
    
    d=opendir(dirname);

    if (d){
        while((dir=readdir(d)) !=NULL){
            if (strcmp(dir->d_name, filename)==0){
                sprintf(file, "%s/%s", dirname, filename);
                remove(file);
                sprintf(answer, "RDL OK\n");       //If delete was successfull
                nleft = strlen(answer);
                ptr=answer;
                while(nleft>0){                                     //writes RLD OK to user
                    n=write(sd, ptr, nleft); 
                    if (n<=0){
                        perror("Error writing back to user\n");
                        exit(1);
                    }
                    nleft-=n;
                    ptr+=n;
                }
                return "NO FORMAT ERROR";
                
            }
        }
            
        sprintf(answer, "RDL EOF\n");       //If file is not available
        nleft = strlen(answer);
        ptr=answer;
        while(nleft>0){                                     //writes RDL EOF to user
            n=write(sd, ptr, nleft); 
            if (n<=0){
                exit(1);
            }
            nleft-=n;
            ptr+=n;
            
        }
    }else{
        sprintf(answer, "RDL NOK\n");       //If there is no content available for this user
        nleft = strlen(answer);
        ptr=answer;
        while(nleft>0){                                     //writes RDL NOK to user
            n=write(sd, ptr, nleft); 
            if (n<=0){
                exit(1);
            }
            nleft-=n;
            ptr+=n;
        }
    
    }
    closedir(d);
    return "NO FORMAT ERROR";
}

char * remove_user(char *buffer, char *dirname, int sd){
    int size, nbytes, n, nleft, m, r, e;
    char *ptr, answer[14], path[30];
    DIR *d;
    struct dirent *dir;
    
    d=opendir(dirname);

    if (d){
        while((dir=readdir(d)) !=NULL){
            sprintf(path, "%s/%s", dirname, dir->d_name);
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
                continue;
            r=remove(path);
            if (r!=0){
                perror("Error removing a file");
                exit(1);
            }
        }
        e=rmdir(dirname);
        if (e!=0){
            perror("Error removing a directory\n");
            exit(1);
        }
        sprintf(answer, "RRM OK\n");       //If remove was successfull
        nleft = strlen(answer);
        ptr=answer;
        while(nleft>0){                                     //writes RRM OK to user
            n=write(sd, ptr, nleft); 
            if (n<=0){
                perror("Error writing back to user\n");
                exit(1);
            }
            nleft-=n;
            ptr+=n;
        }
        sd =0;
        return "NO FORMAT ERROR";
    }else{
        sprintf(answer, "RRM NOK\n");       //If there is no content available for this user
        nleft = strlen(answer);
        ptr=answer;
        while(nleft>0){                                     //writes RRM NOK to user
            n=write(sd, ptr, nleft); 
            if (n<=0){
                perror("Error writing back to user\n");
                exit(1);
            }
            nleft-=n;
            ptr+=n;
        }
        return "NO FORMAT ERROR";
    }

}

void connection(){  /* estabelece ligacao ao AS, troca mensagens */
    int asfd, ufd, newfd, errcode, result, maxfd, nready, len, sd, activity, m, nleft, nwritten, op, cliaddr, res2;
    unsigned int port;
    char buffer[128], sender[128], confirm[128],fop[4], answer[128], *ptr, filename[30], clientip[20];;
    struct addrinfo hints, ashints,uhints, *ures, *asres, *res;
    struct sockaddr_in addr, uaddr;  
    socklen_t uaddr_size;
    ssize_t n;
    socklen_t addrlen;

    mkdir("FILES", 0777);

/*-------------abrir a ligacao udp com o AS---------------------------------------*/
    asfd = socket(AF_INET, SOCK_DGRAM, 0); /* cria socket_cliente udp*/
    if(asfd == -1) {
        perror("SOCKET UDP ERROR");
        exit(1);
    }
    memset(&ashints, 0, sizeof(ashints)); /* limpa memoria */
    ashints.ai_family = AF_INET; 
    ashints.ai_socktype = SOCK_DGRAM;

    errcode= getaddrinfo(as_ip, as_port, &ashints,&asres); /* associa informacao endereco */
    if(errcode != 0) {
        perror("SOCKET UDP getaddrinfo ERROR");
        exit(1);
    }
    ufd = socket(AF_INET, SOCK_STREAM, 0);
    if(ufd == -1) {
        perror("Socket TCP setup error.\n");
        exit(1);
    }

    memset(&uhints, 0, sizeof(uhints));
    uhints.ai_family = AF_INET;
    uhints.ai_socktype = SOCK_STREAM;
    uhints.ai_flags = AI_PASSIVE;

    errcode= getaddrinfo(NULL, own_port, &uhints,&ures); /* associa informacao endereco */    
    if(errcode != 0) {
        perror("Socket TCP getaddrinfo error.\n");
        exit(1);
    }

    if (bind(ufd,ures->ai_addr, ures->ai_addrlen)==-1) {
        perror("Socket TCP bind error.\n");
        exit(1);
    }
   
    if (listen(ufd, 5)==-1) { /*estamos a espera de users*/
        perror("Socket TCP listen error.\n");
        exit(1);       
    }

    for(int i=0; i<MAXUSERS; i++){
        users[i].fd = -1;
    }

    while(1){
        addrlen=sizeof(cliaddr);
        
        FD_ZERO(&readfds);

        //add master socket to set -- ufd is master socket
        FD_SET(ufd, &readfds);
        max_sd = ufd;
        n=0;

        for(int i=0; i<MAXUSERS; i++){
            sd=users[i].fd;

            //if valid socket descriptor then add to read list
            if(sd > 0) {
                FD_SET( sd , &readfds);
            }
                
            //highest file descriptor number, need it for the select function
            if(sd + 1> max_sd) {
                max_sd = sd;
            }
                
        }
        max_sd = max_sd + 1;
        activity=select(max_sd, &readfds, NULL, NULL, NULL);
        if (activity<=0) perror("SELECT ERROR\n");
        for (; activity; activity--) {
            if (FD_ISSET(ufd, &readfds)) { // 
                if ((newfd = accept(ufd, (struct sockaddr*)&cliaddr, &cliaddr))<0){
                    perror("ACCEPT ERROR");
                    exit(EXIT_FAILURE);
                }
                
                users[n].fd=newfd;
                n=n+1;
            }

            for (int i=0; i < MAXUSERS; i++){
                sd=users[i].fd;
                if (FD_ISSET(sd, &readfds)){
                    users[i].fd = -1;

                    int uid, tid;
                    memset(buffer, 0, sizeof(buffer));      /*limpar o buffer para receber nova mensagem de outro user*/
                    
                    /*le mensagem do user*/
                    if(( m=read(sd,buffer,15))!=0){      //lemos os primeiros 15 carateres que incluem operacao, uid e tid
                        if (m==-1) {
                            perror("Erro a ler do user.\n");
                            exit(1);}
                    } else {
                        
                    }
                    sscanf(buffer, "%s %d %d", request, &uid, &tid);
                    uaddr_size = sizeof(struct sockaddr_in);
                    res2 = getpeername(sd, (struct sockaddr *)&uaddr, &uaddr_size);
                    strcpy(clientip, inet_ntoa(uaddr.sin_addr));
                    port = ntohs(uaddr.sin_port);
                    if (verbose) printf("%s %d %d IP:%s PORT:%u\n", request, uid, tid, clientip, port);
                    int pid = fork();
                    if(pid == 0){
                        memset(sender, 0, sizeof(sender));      /*limpar o sender para mandar pedido de validacao*/
                        appendOutput(sender, uid, tid);
                        /* envia string sender com validacao para AS */
                        m = sendto(asfd, sender, strlen(sender), 0, asres->ai_addr, asres->ai_addrlen); 
                        if(m == -1) {
                            perror("Erro no envio para o AS\n");
                            exit(1);
                        }
                        addrlen = sizeof(addr);   
                        
                        /* recebe string confirm com a confirmacao do AS */
                        m=recvfrom(asfd, confirm, 128, 0 ,(struct sockaddr*) &addr, &addrlen); 
                        if(m == -1) {
                            perror("Erro a receber do AS.\n");
                            exit(1);
                        }
                        confirm[m]='\0';
                        char cnf[5];
                        sscanf(confirm, "%s %d %d %s %s", cnf, &uid, &tid, fop, filename);     /*confirmacao do AS*/
                        if (strcmp(fop, "E")==0){
                            //se o fop for E o AS nao validou a operacao respondemos logo 'request INV'
                            strcpy(answer, request);
                            strcat(answer, " ");
                            strcat(answer, "INV\n");
                            nleft = strlen(answer);
                            ptr=answer;
                            while(nleft>0){
                                nwritten=write(sd, ptr, nleft); 
                                if (nwritten<=0){
                                    perror("Error writing back to user\n");
                                    exit(1);
                                }
                                nleft-=nwritten;
                                ptr+=nwritten;
                            }

                        }else{    
                            if (!strcmp("ERR\n", operation(buffer,sd, uid, tid, fop, filename))){
                                bzero(answer, sizeof(answer));
                                strcpy(answer, "ERR\n");
                                nleft = strlen(answer);
                                ptr=answer;
                                while(nleft>0){
                                    nwritten=write(sd, ptr, nleft); 
                                    if (nwritten<=0){
                                        exit(1);
                                    }
                                    nleft-=nwritten;
                                    ptr+=nwritten;
                                }
                            }
                        }
                        close(sd);
                        raise(SIGKILL);
                    }
                }
            }
        }

    }
    freeaddrinfo(asres);
    close(asfd);
}



int main(int argc, char *argv[]){

    storeInfo(argc, argv); 
    connection();

    return 0;
}
