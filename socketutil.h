#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include<stdlib.h>
#include <unistd.h>

struct sockaddr_in *createIPV4Address(char *pt,int port);

#endif