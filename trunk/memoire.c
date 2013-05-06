/********************************************************
 *  memoire.c
 *	dossier streams
 * 	methode de gestion de la mémoires partagée
 * 	Maxime LAHAYE, Mathieu HARHARIDIS
 * 	avril 2013
 ********************************************************/

#include "memoire.h"
typedef int semaphore;

int mem;
key_t key;
key_t keyBD;
key_t keyMutex;
key_t keyClient;
int shmid;
int shmidClient;
partie* part;
semaphore mutex;
semaphore bd;
int* rc;
int* bool;

int down(int sem_id)
{
	struct sembuf sem_op;
	sem_op.sem_num=0;
	sem_op.sem_op=-1;
	sem_op.sem_flg=0;
	if(semop(sem_id, &sem_op, 1) == -1) {
		printf("Erreur lors du down \n");
		return -1;
	}
	else return 0;
}

int up(int sem_id)
{
	struct sembuf sem_op;
	sem_op.sem_num=0;
	sem_op.sem_op=+1;
	sem_op.sem_flg=0;
    
	if(semop(sem_id, &sem_op,1) == -1) {
		printf("Erreur lors du up\n");
		return -1;
	}
	else return 0;
}

void initMemoire(int fd_erreur, int serveur){
    key = ftok("./", 50000);
    keyMutex = ftok("./", 50001);
    keyBD = ftok("./", 50002);
    keyClient = ftok("./", 50003);
    if(serveur == 1){ // le serveur
        if((shmid = shmget(key, sizeof(part), IPC_CREAT | 0666))<0){
            afficher_erreur(fd_erreur, "memoire-shmget\n");
        }
        if((mutex = semget(keyMutex, 1,  IPC_CREAT | 0660))<0){
            afficher_erreur(fd_erreur, "memoire-semget");
        }
        if((bd = semget(keyBD, 1,  IPC_CREAT | 0660))<0){
            afficher_erreur(fd_erreur, "memoire-semget");
        }
        if((semctl(mutex, 0, SETVAL, 1))<0){
            afficher_erreur(fd_erreur, "memoire-semctl");
        }
        if((semctl(bd, 0, SETVAL, 1))<0){
            afficher_erreur(fd_erreur, "memoire-semctl");
        }
    }else{ // le client
        if((shmid = shmget(key, sizeof(part), 0))<0){
            afficher_erreur(fd_erreur, "memoire-shmget\n");
        }
        if((mutex = semget(keyMutex, 1, 0))<0){
            afficher_erreur(fd_erreur, "memoire-semget");
        }
        if((bd = semget(keyBD, 1, 0))<0){
            afficher_erreur(fd_erreur, "memoire-semget");
        }
    }
    if((shmidClient = shmget(keyClient, sizeof(int), IPC_CREAT | 0666))<0){
        afficher_erreur(fd_erreur, "memoire-shmget\n");
    }
    rc = (int*)shmat(shmid, NULL, 0);
    
    if (*rc < 0) {
        afficher_erreur(fd_erreur, "memoire-shmat\n");
    }
    part = shmat(shmid, NULL, 0);

    if ((int) part == -1) {
        afficher_erreur(fd_erreur, "memoire-shmat\n");
    }

}

// Ceci permet d'initialiser la partie et d'écrire tous les scores a la fin
void ecritureMemoireJoueurs(int fd_erreur, partie* partParam){
    down(bd);
    *part = *partParam;
    up(bd);
}

void interdireLecture(){
	down(bd);
	part->bool=1;
	up(bd);
}

void autoriserLecture(){
	down(bd);
	part->bool=0;
	up(bd);
}

partie* lectureMemoire(int fd_error){
	partie* partieARenvoyer;
	if(part->bool==0){
		down(mutex);
		*rc = *rc + 1;
		if(*rc == 1) down(bd);
		up(mutex);
		partieARenvoyer=part;
		down(mutex);
		*rc = *rc - 1;
		if(*rc == 0) up(bd);
		up(mutex);
		return partieARenvoyer;
	}
	return NULL;
}

void fermerMemoire(int fd_error){
    if((shmdt(part))<0){
        afficher_erreur(fd_error, "memoire-shmdt\n");
    }
    if((shmctl(shmid, IPC_RMID, NULL))<0){
        afficher_erreur(fd_error, "memoire-shmctlServeur");
    }
    if((shmdt(rc))<0){
        afficher_erreur(fd_error, "memoire-shmdt\n");
    }
    if((shmctl(shmidClient, IPC_RMID, NULL))<0){
        afficher_erreur(fd_error, "memoire-shmctlClient");
    }
    if((semctl(mutex, IPC_RMID, 0))<0){
        afficher_erreur(fd_error, "memoire_semctl");
    }
    if((semctl(bd, IPC_RMID, 0))<0){
        afficher_erreur(fd_error, "memoire_semctl");
    }
}
