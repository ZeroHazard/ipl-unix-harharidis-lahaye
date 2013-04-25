#include "memoire.h"
#include "util.h"


struct sembuf memOp;
int mem;

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

//écrit dans le mémoire
void ecritureMemoire(int qid, partie * p){
    partie * ptr;
    if((ptr = (partie *) shmat(qid,NULL,0))==(partie *) -1){
        perror("Une erreur lors de la lecture de la mémoire");
        exit(-1);
    }
    fprintf(stderr,"ecriture1\n");
	memOp.sem_num = 0;
	memOp.sem_flg = 0;
        fprintf(stderr,"ecriture2\n");
	memOp.sem_op = -1; // down
        fprintf(stderr,"ecriture3\n");
	SYS(semop(mem,&memOp,2)); // section critique
        fprintf(stderr,"ecriture4\n");
    memset(ptr, 0, sizeof(partie));
        fprintf(stderr,"ecriture5\n");
    memcpy(ptr,p, sizeof(partie));
        fprintf(stderr,"ecriture6\n");
	memOp.sem_op = 1; // up
        fprintf(stderr,"ecriture7\n");
	SYS(semop(mem,&memOp,2));// on en sort
        fprintf(stderr,"ecriture8\n");
    message * m = initMessage();
        fprintf(stderr,"ecriture9\n");
    m->type = MEMOIRE;
        fprintf(stderr,"ecriture10\n");
    writeToAllFils(p,m);
        fprintf(stderr,"ecriture11\n");
    free(m);
        fprintf(stderr,"ecriture12\n");
}
