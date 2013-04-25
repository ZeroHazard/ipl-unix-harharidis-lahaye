#include "memoire.h"
#include "util.h"

int initMemoire(int fd_erreur){
    
    int shmid;
    key_t key = 5678;
    char* shm;
    if((shmid = shmget(key, 30, IPC_CREAT | 0666)<0)){
        afficher_erreur(fd_erreur, "memoire-shmget");
    }
    
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1){
        afficher_erreur(fd_erreur, "memoire-shmat");
    }
    return shmid;
}