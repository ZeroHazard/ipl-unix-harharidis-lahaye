/*
 * joueurs.c
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */
#include "joueur.h"
#include "util.h"

joueur* initJoueurs(char* s, int fd_erreur){
    joueur* j;
    if((j= (joueur*) malloc(sizeof(joueur)))==NULL){
        afficher_erreur(fd_erreur, "joueur-malloc");
    }
    strcpy(j->pseudo,s);
    j->score=-1;
    return j;
}
