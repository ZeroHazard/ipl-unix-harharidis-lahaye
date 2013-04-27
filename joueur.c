/*
 * joueurs.c
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */
#include "joueur.h"
#include "util.h"

joueurs* initJoueurs(char* s, int fd_erreur){
    joueurs* joueur;
    if((joueur = (joueurs*) malloc(sizeof(joueurs)))==NULL){
        afficher_erreur(fd_erreur, "joueur-malloc");
    }
    strcpy(joueur->pseudo,s);
    joueur->score=0;
    return joueur;
}
