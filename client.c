#include "socketutil.h"
#include<pthread.h>

int PORT=2000;

void startListeningAndPrintMessagesOnNewThread(int fd);

void *printRead(void *arg);

void readConsoleEntriesAndSendToServer(int socketFD){
    char *line=NULL;
    size_t linesize=0;
    printf("type something to send to server\n");
    while(1){
        ssize_t charCount=getline(&line,&linesize,stdin);
        if(charCount>0){
            if(strcmp(line,"exit\n")==0) break;
        }
        send(socketFD,line,charCount,0);
    }
    close(socketFD);
}

void startListeningAndPrintMessagesOnNewThread(int fd){
    pthread_t id;
    pthread_create(&id,NULL,printRead,(void*)&fd);
}

void *printRead(void *arg){
    printf("hello");
    int fd=*((int*)arg);
    char *buffer=NULL;
    size_t bytesRead=1024;
    while(1){
        buffer=realloc(buffer,bytesRead);
        ssize_t charCount=recv(fd,buffer,bytesRead,0);
        if(charCount>0){
            buffer[charCount]='\0';
            printf("Response was %s\n ",buffer);
        }
        if(charCount==0) break;
    }    
    close(fd);
}

int main()
{
    char *ip="127.0.0.1";
    int socket_descriptor=socket(AF_INET,SOCK_STREAM,0);
    if(socket_descriptor==-1){
        perror("socket creation failed");
        exit(0);
    }
    struct sockaddr_in *server_address=createIPV4Address(ip,PORT);

    int status=connect(socket_descriptor,(struct sockaddr*)server_address,sizeof(*server_address));
    if(status<0){
        perror("connection not stablished");
        exit(EXIT_FAILURE);
    }
    else if(status==0) printf("connection successful\n");
    startListeningAndPrintMessagesOnNewThread(socket_descriptor);

    readConsoleEntriesAndSendToServer(socket_descriptor);

    close(socket_descriptor);
    return 0;
}