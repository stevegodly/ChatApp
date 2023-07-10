#include "socketutil.h"
#include<stdbool.h>
#include<pthread.h>


struct addressSocket{
    int fd;
    struct sockaddr_in address;
    int error;
    bool succcessful;
};

struct addressSocket *acceptSocket(int socketFD);

void sendReceivedMessageToTheOtherClients(char *buffer,int socketFD);

struct addressSocket acceptedSocket[10];
int acceptCounter=0;

void *receiveAndPrintIncomingData(void *arg);

void receiveAndPrintIncomingDataSeperateThread(struct addressSocket *pSocket);

void startAcceptingIncomingConnections(int socketFD){
    while(true){
        struct addressSocket *clientSocket=acceptSocket(socketFD);
        if(clientSocket->fd<0){
            perror("Couldn't establish connection with client");
            exit(EXIT_FAILURE);
        }
        acceptedSocket[acceptCounter++]=*clientSocket;

        receiveAndPrintIncomingDataSeperateThread(clientSocket);
    }
}

void receiveAndPrintIncomingDataSeperateThread(struct addressSocket *pSocket){
    pthread_t id;
    pthread_create(&id,NULL,receiveAndPrintIncomingData,(void*)&(pSocket->fd));
}

void *receiveAndPrintIncomingData(void *arg){
    int fd=*((int*)arg);
    char *line=NULL;
    size_t errorgth=1024;
    while(1){
        line=realloc(line,errorgth);
        ssize_t charCount=recv(fd,line,errorgth,0);
        if(charCount>0){
            line[charCount]='\0';
            printf("Response was %s\n",line); 
            sendReceivedMessageToTheOtherClients(line,fd);
        }
        if(charCount==0) break;
    }
    close(fd);    
}

void sendReceivedMessageToTheOtherClients(char *buffer,int socketFD){
    for(int i=0;i<acceptCounter;acceptCounter++){
        if(acceptedSocket[i]->fd!=socketFD){
            send(acceptedSocket[i]->fd,buffer,strlen(buffer),0);
        }
    }
}

struct addressSocket *acceptSocket(int socketFD){
    struct sockaddr_in client_address;
    int client_error=sizeof(client_address);
    int client_fd=accept(socketFD,(struct sockaddr*)&client_address,&client_error);
    
    struct addressSocket *acceptedSocket=malloc(sizeof(struct addressSocket));
    acceptedSocket->fd=client_fd;
    acceptedSocket->address=client_address;
    acceptedSocket->error=client_fd;
    if(client_fd) {
        acceptedSocket->succcessful=true;
        acceptedSocket->error=0;
    }    
    else {
        acceptedSocket->succcessful=false;
        acceptedSocket->error=client_fd;
    }

    return acceptedSocket;
}

int main()
{
    int socketFD=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in *serverAddress = createIPV4Address("",2000);
    if(bind(socketFD,(struct sockaddr*)serverAddress,sizeof(*serverAddress))<0){
        perror("binding falied");
        exit(EXIT_FAILURE);
    }
    printf("socket binded successfully\n");

    if(listen(socketFD,10)<0) {
        perror("listening falied");
        exit(EXIT_FAILURE);
    }
    printf("server is listening\n");

    startAcceptingIncomingConnections(socketFD);

    shutdown(socketFD,SHUT_RDWR);
    return 0;
}