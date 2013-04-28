/*
 * @authors Harharidis Mathieu Lahaye Maxime
 *
 */

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
    //    part = initPartie(part);
    key = ftok("./", 50000);
    keyMutex = ftok("./", 50001);
    keyBD = ftok("./", 50002);
    keyClient = ftok("./", 50003);
    if(serveur == 1){ // le serveur
        if((shmid = shmget(key, sizeof(part), IPC_CREAT | 0666))<0){
            afficher_erreur(fd_erreur, "memoire-shmget\n");
        }
        mutex = semget(keyMutex, 1,  IPC_CREAT | 0660);
        bd = semget(keyBD, 1,  IPC_CREAT | 0660);
        semctl(mutex, 0, SETVAL, 1);
        semctl(bd, 0, SETVAL, 1);
    }else{ // le client
        if((shmid = shmget(key, sizeof(part), 0))<0){
            afficher_erreur(fd_erreur, "memoire-shmget\n");
        }
        mutex = semget(keyMutex, 1, 0);
        bd = semget(keyBD, 1, 0);
    }
    if((shmidClient = shmget(keyClient, sizeof(int), IPC_CREAT | 0666))<0){
        afficher_erreur(fd_erreur, "memoire-shmget\n");
    }
    rc = (int*)shmat(shmid, NULL, 0);
    if (*rc < 0) {
        afficher_erreur(fd_erreur, "memoire-shmat\n");
    }
    part = shmat(shmid, NULL, 0);
    if ((int) part < 0) {
        afficher_erreur(fd_erreur, "memoire-shmat\n");
    }
}


void ecritureMemoireJoueurs(int fd_erreur, partie* partParam){
    down(bd);
    *part = *partParam;
    printf("Le nombre de joueur est :%d\n",part->nombreJoueur);
    up(bd);
}

partie* lectureMemoire(int fd_error){
    down(mutex);
    *rc = *rc + 1;
    if(*rc == 1) down(bd);
    up(mutex);
    printf("Le nombre de joueur est :%d\n",part->nombreJoueur);
    printf("Le pseudo du joueur est :%s\n",(part->joueurs)[0].pseudo);
    printf("Le pseudo du joueur est :%s\n",(part->joueurs)[1].pseudo);
    down(mutex);
    *rc = *rc - 1;
    if(*rc == 0) up(bd);
    up(mutex);
    return part;
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
