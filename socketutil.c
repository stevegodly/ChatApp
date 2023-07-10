#include "socketutil.h"

struct sockaddr_in *createIPV4Address(char *pt,int port){
    struct sockaddr_in *server_address=malloc(sizeof(struct sockaddr_in));
    
    if(strlen(pt)==0) server_address->sin_addr.s_addr=INADDR_ANY;

    else inet_pton(AF_INET,pt,&server_address->sin_addr.s_addr);
    server_address->sin_family=AF_INET;
    server_address->sin_port=htons(port);
    return server_address;
} 