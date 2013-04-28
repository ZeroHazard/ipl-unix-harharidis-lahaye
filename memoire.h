/********************************************************
 *  memoire.h
 *	dossier streams
 * 	entete du fichier memoire.c
 * 	Maxime LAHAYE, Mathieu HARHARIDIS
 * 	avril 2013
 ********************************************************/
#ifndef ipl_lahaye_harharidis_memoire_h
#define ipl_lahaye_harharidis_memoire_h
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define MEMOIRE 19

#include "partie.h"
#include "util.h"
#include "joueur.h"
void initMemoire();
#endif


void interdireLecture();
void autoriserLecture();

partie* lectureMemoire(int);
void ecritureMemoireJoueurs(int, partie*);
void removeMemoire(int, int);
void fermerMemoire(int);
