#ifndef ipl_lahaye_harharidis_memoire_h
#define ipl_lahaye_harharidis_memoire_h
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define MEMOIRE 19

#include "partie.h"

int initMemoire();
#endif

partie* lectureMemoire(int qid, int);
void ecritureMemoire(int, partie *, int);
void removeMemoire(int, int);
