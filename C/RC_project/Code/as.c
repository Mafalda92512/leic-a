#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <ftw.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#define PORT "58011"
#define MAXUSERS 100

void connection();
void storeInfo(int argc, char ** argv);
char as_port[6], fs_ip[21], fs_port[6];
char sender[128];

typedef struct user{
    int fd;
}user;

user users[MAXUSERS];


int max(int a, int b){ /* recebe dois inteiros, devolve maior inteiro + 1 */
    if(a > b){
        return a;
    }
    else{
        return b;
    }
}

void createFile(char *path_file, char *to_write) {
    FILE * filedescriptor = fopen(path_file, "w");
    fprintf(filedescriptor, "%s", to_write);
    fclose(filedescriptor);

}   

int addUser(int uid, char * pw, char * ip, char * port){
    char directory[15];
    sprintf(directory, "USERS/%d", uid);
    mkdir(directory, 0777);

    char path_pass[50], path_reg[50], path_vc[50], path_rid[50]; 
    char reg_info[50], vc[4], rid[4];
    strcpy(vc, "0\n");
    strcpy(rid, "0\n");

    // cria ficheiros pass, reg, valcode e rid
    sprintf(path_pass, "USERS/%d/%d_pass.txt", uid, uid); 
    sprintf(path_reg, "USERS/%d/%d_reg.txt", uid, uid);
    sprintf(path_vc, "USERS/%d/%d_vc.txt", uid, uid);
    sprintf(path_rid, "USERS/%d/%d_rid.txt", uid, uid);

    sprintf(reg_info, "%s %s\n", ip, port);
    createFile(path_pass, pw);
    createFile(path_reg, reg_info);
    createFile(path_vc, vc);
    createFile(path_rid, rid);
    return 1;
}

char * removeUser(int uid, char * pass){
    char filepathpass[30], filepathtid[30], filepathvc[30] , filepathreg[30], filepathrid[30], filepass[10];
    char filepathlogin[30];
    sprintf(filepathpass, "USERS/%d/%d_pass.txt", uid, uid);
    sprintf(filepathtid, "USERS/%d/%d_tid.txt", uid, uid);
    sprintf(filepathvc, "USERS/%d/%d_vc.txt", uid, uid);
    sprintf(filepathrid, "USERS/%d/%d_rid.txt", uid, uid);
    sprintf(filepathreg, "USERS/%d/%d_reg.txt", uid, uid);
    sprintf(filepathlogin, "USERS/%d/%d_login.txt", uid, uid);

    FILE * str = fopen(filepathpass, "r");
    if (str == NULL) return "RUN NOK\n"; // se o user nao existir

    fscanf(str, "%s", filepass);
    fclose(str);
    if(!strcmp(filepass, pass)){ // e a pass do user corresponder
        remove(filepathvc); // estes ficheiros podem ou nao existir
        remove(filepathrid);
        remove(filepathtid);
        remove(filepathpass);
        remove(filepathlogin);
        if(remove(filepathreg) != 0){ // este ficheiro tem que existir
            return "RUN NOK\n";
        }
        return "RUN OK\n";
    }
    else {
        return "RUN NOK\n";
    }
}

char * pdinput(char * buffer){
    if(buffer[0] == 'R' && buffer[1] == 'E' && buffer[2] == 'G'){
        int uid, res;
        char pw[10], ip[21], port[7], useless[5]; 
        sscanf(buffer, "%s %d %s %s %s", useless, &uid, pw, ip, port);
        int t = addUser(uid, pw, ip, port);
        memset(buffer, 0, sizeof(buffer));
        if (t == 1){
            strcpy(buffer, "RRG OK\n");
        } else {
            strcpy(buffer, "RRG NOK\n");
        }
        return buffer;
    }
    else if(buffer[0] == 'U' && buffer[1] == 'N' && buffer[2] == 'R'){
        int uid, res;
        char pw[9], useless[4];
        sscanf(buffer, "%s %d %s", useless, &uid, pw);
        return removeUser(uid, pw);
    }
}

char * isRegistered(int uid, char * pass) {
    char password[6];
    char path[35];
    FILE *fptr;
    sprintf(path, "USERS/%d/%d_pass.txt", uid, uid);
    fptr = fopen(path, "r");

    if (fptr != NULL) {
        char read[6];
        fscanf(fptr, "%s", read);
        if (!strcmp(pass, read) ){
            char path_file[35];
            sprintf(path_file, "USERS/%d/%d_login.txt", uid, uid);
            FILE * filedescriptor = fopen(path_file, "w");
            fclose(filedescriptor);
            fclose(fptr);
            return "RLO OK\n";
        }
        else {
            fclose(fptr);
            return "RLO NOK\n";
        }
        fclose(fptr);
    }
    return "ERR\n";
}

char* validRequest(int uid, int rid, char *fop, char *fname, int socket, struct sockaddr_in addr, int addrlen, struct addrinfo* res) {
    char send_to_pd[128], buffer[128], VC[5], UID[6];
    int n;
    char charuid[6], directory[11], path[35];
    FILE *fptr, *fptr2;
    sprintf(path, "USERS/%d/%d_pass.txt", uid, uid);
    fptr = fopen(path, "r");
    memset(sender, 0, sizeof(sender));

    if (fptr != NULL) { // existe ficheiro com a pass
        sprintf(path, "USERS/%d/%d_login.txt", uid, uid);
        fptr2 = fopen(path, "r");
        if (fptr2 != NULL) { // existe ficheiro login
            fclose(fptr2);
            fclose(fptr);
            sprintf(directory, "USERS/%d", uid);
            srand(time(NULL));
            int valcode = rand() % 1000;
            int valcode2 =0;
            while (valcode2 == 0) {
                valcode2 = rand() % 10;
            }
            valcode = valcode + valcode2*1000;
            sprintf(path, "USERS/%d/%d_vc.txt", uid, uid);
            FILE *altervc = fopen(path, "w");
            fprintf(altervc, "%d\n", valcode);
            fclose(altervc);

            sprintf(path, "USERS/%d/%d_rid.txt", uid, uid);
            FILE *alterrid = fopen(path, "w");

            if (fop[0] == 'R' || fop[0] == 'U' || fop[0] == 'D') {
                sprintf(send_to_pd, "VLC %d %d %s %s\n", uid, valcode, fop, fname);
                fprintf(alterrid, "%d %s %s\n", rid, fop, fname);
                fclose(alterrid);
            }
            else if (fop[0] != 'L' && fop[0] != 'X') {
                strcpy(sender, "RRQ EFOP\n");
                close(socket);
                return sender;
            }
            else {
                sprintf(send_to_pd, "VLC %d %d %s\n", uid, valcode, fop);
                fprintf(alterrid, "%d %s\n", rid, fop);
                fclose(alterrid);
            }
            n = sendto(socket, send_to_pd, strlen(send_to_pd), 0, res->ai_addr, res->ai_addrlen);
            if (n==-1) {
                strcpy(sender, "RRQ EPD\n");
                close(socket);
                return sender;
            }
            memset(buffer, 0, sizeof(buffer));
            n=recvfrom(socket, buffer, 128, 0 ,(struct sockaddr*) &addr, &addrlen); /* recebe codigo validacao */
            if(n == -1) {
                strcpy(sender, "RRQ EPD\n");
                close(socket);
                return sender;
            }
            char compare1[30], compare2[30];
            sprintf(compare1, "RVC %d OK\n", uid);
            sprintf(compare2, "RVC %d NOK\n", uid);
            if (!strcmp(buffer, compare1)) {
                strcpy(sender, "RRQ OK\n");
            }
            else if (!strcmp(buffer, compare2)) {
                strcpy(sender, "RRQ EPD\n");
            }
            else {
                strcpy(sender, "ERR\n");
            }
            close(socket);
            return sender;
        }
        else {
            strcpy(sender, "RRQ ELOG\n");
            fclose(fptr);
            close(socket);
            return sender;
        }
    }
    else {
        strcpy(sender, "RRQ EUSER\n");
        close(socket);
        return sender;
    }
}

char * authenticate(int uid, int rid, int vc) {
    char TID[5];
    int tid = 0;
    char path[35];
    char filename[50];
    FILE *fptr;
    sprintf(path, "USERS/%d/%d_pass.txt", uid, uid);
    fptr = fopen(path, "r");
    memset(sender, 0, sizeof(sender));
    if (fptr != NULL) { // existe ficheiro com a pass
        sprintf(path, "USERS/%d/%d_vc.txt", uid, uid);
        FILE *altervc = fopen(path, "r");
        if (altervc != NULL) { 
            int valcode;
            fscanf(altervc, "%d\n", &valcode);
            fclose(altervc);
            if (valcode == vc) { // vc correspondem
                sprintf(path, "USERS/%d/%d_rid.txt", uid, uid);
                FILE *alterrid = fopen(path, "r");
                if (alterrid != NULL) {
                    int readrid;
                    char fop[2], filename[30];
                    fscanf(alterrid, "%d %s %s\n", &readrid, fop, filename);
                    fclose(alterrid);
                    if (rid == readrid) { // rids correspondem
                        do{
                            srand(time(NULL));
                            tid = rand() % 1000;
                            int tid2 = rand() % 10;
                            tid = tid + tid2 * 1000;
                        }while(tid < 1000);
                        
                        char to_write[20];
                        if (fop[0] == 'R' || fop[0] == 'U' || fop[0] == 'D') {
                            sprintf(to_write, "%d %s %s\n", tid, fop, filename);
                        }
                        else {
                            sprintf(to_write, "%d %s\n", tid, fop);
                        }
                        sprintf(path, "USERS/%d/%d_tid.txt", uid, uid);
                        FILE * tidfile = fopen(path, "w");
                        fprintf(tidfile,"%s" , to_write);
                        fclose(tidfile);
                    }
                }
            }
        }    
    }
    
    sprintf(sender, "RAU %d\n", tid);
    return sender;
}

char * userinput(char * buffer) {
    int uid;
    char command[4];
    if (buffer[0] == 'L' && buffer[1]=='O' && buffer[2] == 'G') {
        char pass[8];
        sscanf(buffer, "%s %d %s", command, &uid, pass);
        return isRegistered(uid, pass);
    }
    else if (buffer[0] == 'R' && buffer[1]=='E' && buffer[2] == 'Q') {
        int rid, udpclientfd, errcode;
        char fop[2], fname[128], pd_ip[30], pd_port[7], path[50];
        struct addrinfo hints, *res, hintst, *rest;
        struct sockaddr_in addr;
        int addrlen;
        sscanf(buffer, "%s %d %d %s %s", command, &uid, &rid, fop, fname);
        //UDP set up as client ai_assive esta mal?
        udpclientfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(udpclientfd == -1) {
            perror("SOCKET ERROR");
            exit(1);
        }
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;

        sprintf(path,"USERS/%d/%d_reg.txt", uid, uid);
        FILE* reg = fopen(path, "r");
        fscanf(reg, "%s %s", pd_ip, pd_port);
        errcode = getaddrinfo(pd_ip, pd_port, &hints, &res);
        if(errcode != 0) {
            perror("getaddrinfo ERROR");
            exit(1);
        }
        addrlen = sizeof(addr);
        // verify if buffer is correctly formatted, if not, return ERR
        return validRequest(uid, rid, fop, fname, udpclientfd, addr, addrlen, res);
    }
    else if (buffer[0] == 'A' && buffer[1]=='U' && buffer[2] == 'T') {
        int rid, vc;
        sscanf(buffer, "%s %d %d %d", command, &uid, &rid, &vc); 
        return authenticate(uid, rid, vc);
    }
}

int remove_user_directory(int uid) {
    char filepathlogin[30];
    sprintf(filepathlogin, "USERS/%d/%d_login.txt", uid, uid);
    remove(filepathlogin);
    return 1;
}

char * fsInput(char * input){
    int uid;
    char tid[8], useless[5], path[30], output[100], usertid[8], ops[3], filename[30];
    sscanf(input, "%s %d %s", useless, &uid , tid);
    sprintf(path,"USERS/%d/%d_tid.txt", uid, uid);
    memset(input, 0, sizeof(input));
    FILE * fp = fopen(path, "r");
    if(!fp){
        sprintf(input,"CNF %d %s E\n", uid, tid);
        fclose(fp);
        return input;
    }

    fscanf(fp, "%s %s %s", usertid, ops, filename);
    if(strcmp(tid, usertid)){
        sprintf(input,"CNF %d %s E\n", uid, tid);
        return input;
    }
    if(!strcmp(ops, "R") || !strcmp(ops, "U") || !strcmp(ops, "D")){
        sprintf(input,"CNF %d %s %s %s\n", uid, tid, ops, filename);
        return input;
    } else {
        sprintf(input,"CNF %d %s %s\n", uid, tid, ops);
        return input;
    }
    if (!(remove_user_directory(uid))) sprintf(input,"CNF %d %s E\n", uid, tid);
    fclose(fp);  
    return input;
}

void connection(){
    int udpfd, tcpuser, errcode, maxfd, lentcp, sd, j= 0;
    ssize_t n;
    struct addrinfo hints, *res, hintst, *rest;
    fd_set readfds;
    struct sockaddr_in addrtcp;


    if(mkdir("USERS", 0777) == -1) {
        perror("ERROR IN CREATING DIRECTORY USERS");
        exit(1);
    }

    //UDP SET UP (FS)
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(udpfd == -1) {
        perror("UDP SOCKET FAILURE");
        exit(1);
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    errcode = getaddrinfo(NULL, as_port, &hints, &res);
    if(errcode != 0) {
        perror("UDP getaddrinfo FAILURE");
        exit(1);
    }
    n = bind(udpfd, res->ai_addr, res->ai_addrlen); //server
    if(n==-1) {
        perror("UDP bind FAILURE");
        exit(1);
    }
    // SET UP TCP (USER)
    tcpuser = socket(AF_INET, SOCK_STREAM, 0);
    if(tcpuser == -1) {
        perror("TCP SOCKET FAILURE");
        exit(1);
    }
    memset(&hintst, 0, sizeof(hintst));
    hintst.ai_family = AF_INET;
    hintst.ai_socktype = SOCK_STREAM;
    hintst.ai_flags = AI_PASSIVE;

    errcode = getaddrinfo(NULL, as_port, &hintst, &rest);
    if(errcode != 0) {
        perror("TCP getaddrinfo FAILURE");
        exit(1);
    }
    n = bind(tcpuser, rest->ai_addr, rest->ai_addrlen);
    if(n==-1) {
        perror("TCP bind FAILURE");
        exit(1);
    }
    if(listen(tcpuser, 5) == -1) {
        perror("TCP listen FAILURE");
        exit(1);
    }
    // esta a ficar a espera ate que um user se ligue, e portanto nao faz registos antes disso
    for(int i=0; i<MAXUSERS; i++){
        users[i].fd = -1;
    }

    maxfd = max(tcpuser, udpfd);
    maxfd += 1;
    while(1){
        int nready, nbytes, nleft;
        char buffer[128];
        char senduser[128];
        FD_ZERO(&readfds);

        FD_SET(tcpuser, &readfds);
        FD_SET(udpfd, &readfds);
        maxfd = max(udpfd, tcpuser);

        for(int i=0; i<MAXUSERS; i++){
            sd=users[i].fd;

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);
            //highest file descriptor number, need it for the select function
            if(sd + 1 > maxfd)
                maxfd = sd;
        }
        maxfd = maxfd + 1;
        nready = select(maxfd, &readfds, NULL, NULL, NULL);
        if(nready <= 0) {
            perror("SELECT ERROR");
            exit(1);
        }
        for(; nready; nready--){
            if(FD_ISSET(udpfd, &readfds)){ //le do pd || fs
                struct sockaddr_in addr;
                int len;
                memset(&addr, 0, sizeof(addr));
                memset(buffer, 0, sizeof(buffer));
                len = sizeof(addr);
                n=recvfrom(udpfd, buffer, 128, 0 ,(struct sockaddr*) &addr, &len); /* recebe codigo validacao */


                if(n == -1) {
                    perror("SOCKET recvfrom ERROR");
                    exit(1);
                }
                buffer[n] = '\0';
                int pd = fork();
                if(pd == 0){
                    char sendpd[128];
                    memset(sendpd, 0, sizeof(sendpd));
                    if (buffer[0] == 'V' && buffer[1] == 'L' && buffer[2] == 'D') { // esta a ler do fs
                        strcpy(sendpd, fsInput(buffer));

                    }
                    else {
                        strcpy(sendpd, pdinput(buffer));
                    }
                    
                    n = sendto(udpfd, sendpd, strlen(sendpd), 0, (struct sockaddr*) &addr, len);
                    if (n==-1) {
                        perror("SOCKET sendto ERROR");
                        exit(1);
                    }
                    raise(SIGKILL);
                }
                sleep(1);
                
            }
            if(FD_ISSET(tcpuser, &readfds)) { //le user
                memset(buffer, 0, sizeof(buffer));
                if (j < MAXUSERS) {
                    lentcp = sizeof(addrtcp);
                    int connfd = accept(tcpuser, (struct sockaddr*) &addrtcp, &lentcp);
                    if (connfd < 0) {
                        perror("TCP accept FAILURE");
                        exit(1);
                    }
                    users[j].fd = connfd;
                    j = j+1;
                }
                else {
                    continue;
                }
            }
            for(int i=0; i< j; i++) {
                if (users[i].fd <= 0) {
                    continue;
                }
                else {
                    sd = users[i].fd;
                    if(FD_ISSET(sd, &readfds)) {
                        memset(buffer, 0, sizeof(buffer));
                        n=read(sd,buffer,128);
                        if (n==-1) {
                            perror("READ ERROR");
                            exit(1);
                        }
                        else if(n==0) continue;
                        int pd = fork();
                        if (pd == 0){
                            buffer[n]='\0';
                            char *ptr;
                            memset(senduser, 0, sizeof(senduser));
                            strcpy(senduser, userinput(buffer));
                            nbytes=strlen(senduser);
                            nleft=nbytes;
                            ptr=senduser;
                            while (nleft>0){    
                                n = write(sd,ptr, nleft);
                                if (n<=0) {
                                    perror("WRITE ERROR");
                                    exit(1);
                                }
                                nleft-=n;
                                ptr+=n;
                            } 
                            raise(SIGKILL);
                        }
                    }
                }      
            }
        }
    }
    close(udpfd);
    close(tcpuser);   
}

void storeInfo(int argc, char ** argv){ /* guarda informacao argumentos em variaveis */
    
    for(int i = 0; i< argc; i++){
        if(strcmp(argv[i], "-p") == 0) strcpy(as_port, argv[i + 1]);
    }

}

int main(int argc, char *argv[]){
    storeInfo(argc, argv);
    
    connection();

    return 0;
}
