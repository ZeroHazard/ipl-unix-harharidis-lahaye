/*
 * partie.c
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */
#include "partie.h"
#include "util.h"
int* tuiles = NULL;
int nbrTuiles = 40;

//création d'une partie
partie* initPartie(partie* p){
    if((p=(partie *)malloc(sizeof(partie)))==NULL){
        return NULL;
    }
    p->nombreJoueur=0;
    //ecritureMemoire(p->memoire,p);
    p->bool=0;
    return p;
}

void initTuiles(int fd_error){
    int i;
    int j = 1;
    int k = 1;
    if((tuiles = (int*)malloc(40*sizeof(int)))==NULL){
        afficher_erreur(fd_error, "partie-malloc");
    }
    for (i = 0; i < 40; i++) {
        *(tuiles + i) = j;
//        printf("%d  ", *(tuiles + i));
        if(j > 10 && j < 20){
            if(k< 2) {
                k++;
            }else {
                k = 1;
                j++;
            }
        } else {
            j++;
        }
    }
}

int tirerTuile(int fd_error){
//    printf("[");
//    for (int i=0; i<nbrTuiles; i++) {
//        printf("%d  ", *(tuiles + i));
//    }
//    printf("]\n");
    int position = rand() % nbrTuiles;
    int tuile = *(tuiles+position);
    *(tuiles+position) = *(tuiles+nbrTuiles-1);
    nbrTuiles--;
//    printf("[%d] %d  \n", position, tuile);
    return tuile;
}

