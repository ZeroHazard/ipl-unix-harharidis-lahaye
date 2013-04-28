/*
 * partie.h
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */

#ifndef PARTIE_H_
#define PARTIE_H_
#define JOUEURS_MAX 5


#include "joueur.h"
#include "header.h"

typedef struct partie {
    int nombreJoueur; //le nombre de joueur actif
    joueur joueurs[JOUEURS_MAX];
}partie;

partie* initPartie(partie*);


#endif /* PARTIE_H_ */
