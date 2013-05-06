/********************************************************
 *  partie.h
 *	dossier streams
 * 	Contient une structure partie ainsi que la défnition des fonctions de partie.c
 * 	Maxime LAHAYE, Mathieu HARHARIDIS
 * 	avril 2013
 ********************************************************/

#ifndef PARTIE_H_
#define PARTIE_H_
#define JOUEURS_MAX 5


#include "joueur.h"
#include "header.h"

typedef struct partie {
    int nombreJoueur; //le nombre de joueur actif
    joueur joueurs[JOUEURS_MAX];
    int bool;
}partie;

partie* initPartie(partie*);
void initTuiles(int);
int tirerTuile(int);


#endif /* PARTIE_H_ */
