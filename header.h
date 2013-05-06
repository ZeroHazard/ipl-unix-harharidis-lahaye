/********************************************************
 *  header.h
 *	dossier streams
 * 	Le fichier header du client et du serveur contenant une structure de client
 * 	Maxime LAHAYE, Mathieu HARHARIDIS
 * 	avril 2013
 ********************************************************/

#ifndef Unix_header_h
#define Unix_header_h
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#define tailleLigne 1024
//#define SYS(call, fd_erreur, message) ((call)<0)? (erreur(fd_erreur, message)):0

#include <stdio.h>
#include <stdlib.h>

typedef struct client{
    char pseudo[50];
    SOCKET csocket;
}client;

#endif
