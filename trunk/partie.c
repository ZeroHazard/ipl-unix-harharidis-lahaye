/*
 * partie.c
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */
#include "partie.h"

//création d'une partie
partie* initPartie(partie* p){
    int i,j;
    if((p=(partie *)malloc(sizeof(partie)))==NULL){
        return NULL;
    }
    p->nombreJoueur=0;
    //ecritureMemoire(p->memoire,p);
    return p;
}

