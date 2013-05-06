/********************************************************
 *  util.c
 *	dossier streams
 * 	définition d'une structure de joueur ainsi que d'une méthode de création de joueur
 * 	Maxime LAHAYE, Mathieu HARHARIDIS
 * 	avril 2013
 ********************************************************/

#ifndef JOUEURS_H_
#define JOUEURS_H_
#include "header.h"
typedef struct joueur{
    char pseudo[50];
    int score;
}joueur;
joueur* initJoueurs(char*, int);

#endif /* JOUEURS_H_ */
