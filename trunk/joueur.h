/*
 * joueurs.h
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */

#ifndef JOUEURS_H_
#define JOUEURS_H_
#include "header.h"
typedef struct joueur{
    char pseudo[50];
    int score;
}joueur;
joueur* initJoueurs(char*, int);

#endif /* JOUEURS_H_ */
