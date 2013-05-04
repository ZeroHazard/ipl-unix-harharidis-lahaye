/********************************************************
 *  util.c
 *	dossier streams
 * 	methodes utilitaire
 * 	Maxime LAHAYE, Mathieu HARHARIDIS
 * 	avril 2013
 ********************************************************/
#include "util.h"

void afficher_erreur(int fd_erreur, char* message){
    write(fd_erreur, message, strlen(message));
    exit(1);
}
