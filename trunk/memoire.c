#include "memoire.h"
#include "util.h"


struct sembuf memOp;
int mem;

int initMemoire(int fd_erreur){
    
    int shmid;
    key_t key = 5678;
    char* shm;
    partie* p;
    if((shmid = shmget(key, sizeof(p), IPC_CREAT | 0666)<0)){
        afficher_erreur(fd_erreur, "memoire-shmget");
    }
    
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1){
        afficher_erreur(fd_erreur, "memoire-shmat");
    }
    return shmid;
}

//écrit dans le mémoire
void ecritureMemoire(int qid, partie * p, int fd_erreur){
    partie* ptr;
    key_t key = 5678;
    int shmid;
    if((shmid = shmget(key, sizeof(p), IPC_CREAT | 0666)<0)){
        afficher_erreur(fd_erreur, "memoire-shmget");
    }
    if((ptr = shmat(shmid, NULL, 0)) == (void*) -1){
        perror("Une erreur lors de la lecture de la mémoire");
        exit(-1);
    }
    p->nombreJoueur = 2;
    *ptr = *p;
    //    partie * ptr;
    //    if((ptr = (partie *) shmat(qid,NULL,0))==(partie *) -1){
    //        perror("Une erreur lors de la lecture de la mémoire");
    //        exit(-1);
    //    }
    //    fprintf(stderr,"ecriture1\n");
    //	memOp.sem_num = 0;
    //	memOp.sem_flg = 0;
    //        fprintf(stderr,"ecriture2\n");
    //	memOp.sem_op = -1; // down
    //        fprintf(stderr,"ecriture3\n");
    //	SYS(semop(mem,&memOp,2)); // section critique
    //        fprintf(stderr,"ecriture4\n");
    //    memset(ptr, 0, sizeof(partie));
    //        fprintf(stderr,"ecriture5\n");
    //    memcpy(ptr,p, sizeof(partie));
    //        fprintf(stderr,"ecriture6\n");
    //	memOp.sem_op = 1; // up
    //        fprintf(stderr,"ecriture7\n");
    //	SYS(semop(mem,&memOp,2));// on en sort
    //        fprintf(stderr,"ecriture8\n");
    //    message * m = initMessage();
    //        fprintf(stderr,"ecriture9\n");
    //    m->type = MEMOIRE;
    //        fprintf(stderr,"ecriture10\n");
    //    writeToAllFils(p,m);
    //        fprintf(stderr,"ecriture11\n");
    //    free(m);
    //        fprintf(stderr,"ecriture12\n");
}

partie* lectureMemoire(int qid, int fd_error){
    partie * ptr;
    key_t key = 5678;
    
    int id;
    if((id = shmget(key, sizeof(partie), 0444))<0){
        afficher_erreur(fd_error, "memoire-shmget");
    }
    if((ptr = (partie*) shmat(id, NULL, 0))){
        afficher_erreur(fd_error, "memoire-shmat");
    }
    return ptr;
    //	memOp.sem_num = 0;
    //	memOp.sem_flg = 0;
    //	sop.sem_num = 0;
    //	sop.sem_op = -1; // down
    //	sop.sem_flg = 0;
    //	SYS(semop(mutex,&sop,2)); // zone critique acces a rc
    //	rc++;
    //	if(rc == 1){
    //		memOp.sem_op = -1; // down
    //		SYS(semop(mem,&memOp,2));
    //	}
    //	sop.sem_op = 1; //up
    //	SYS(semop(mutex,&sop,2)); // zone non critique
    //
    //    if ((ptr = (partie *) shmat(qid, NULL, 0)) == (partie *)-1){
    //        perror("ipctool_shared_get.shmat:" );
    //        return NULL;
    //    }
    //	sop.sem_op = -1; // down
    //	SYS(semop(mutex,&sop,2)); // zone critique acces a rc
    //	rc--;
    //	if(rc == 0){
    //		memOp.sem_op = 1; // up
    //		SYS(semop(mem,&memOp,2));
    //	}
    //	sop.sem_op = 1; //up
    //	SYS(semop(mutex,&sop,2)); // zone non critique
    //    return ptr;
}