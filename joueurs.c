/*
 * joueurs.c
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */
#include "joueurs.h"

joueurs* initJoueurs(char* s){
	joueurs* joueur;
    if((joueur = (joueur*) malloc(sizeof(joueur)))==NULL){
        return NULL;
    }
    strcpy(joueur->pseudo,s);
    joueur->score=0;
    return joueur;
}

