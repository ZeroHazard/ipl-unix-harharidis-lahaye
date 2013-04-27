#include "memoire.h"


struct sembuf memOp;
int mem;
key_t key;
int shmid;
partie* part;



void initMemoire(int fd_erreur, int serveur){
//    part = initPartie(part);
    key = ftok("./", 42);
    if(serveur == 1){
        if((shmid = shmget(key, sizeof(part), IPC_CREAT | 0666))<0){
            afficher_erreur(fd_erreur, "memoire-shmget\n");
            part->nombreJoueur = 0;
        }
    }else{
        if((shmid = shmget(key, sizeof(part), 0))<0){
            afficher_erreur(fd_erreur, "memoire-shmget\n");
        }
    }
    part = shmat(shmid, NULL, 0);
    if ((int) part < 0) {
        afficher_erreur(fd_erreur, "memoire-shmat\n");
    }
}

//écrit dans la mémoire
void ecritureMemoire(int fd_erreur, client* cl){
    joueurs* j;
    j = initJoueurs(cl->pseudo, fd_erreur);
    part->nombreJoueur = part->nombreJoueur+1;
    part->joueurs[part->nombreJoueur-1] = *j;
    printf("SERVEUR-Le nombre de joueur est :%d\n",part->nombreJoueur);
    printf("SERVEUR-Le pseudo du joueur est :%s\n",(part->joueurs)[0].pseudo);
    printf("SERVEUR-Le pseudo du joueur est :%s\n",(part->joueurs)[1].pseudo);

//    printf("SERVEUR-Le nombre de joueur est de : %d\n", part->nombreJoueur);
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

partie* lectureMemoire(int fd_error){
    printf("Le nombre de joueur est :%d\n",part->nombreJoueur);
    printf("Le pseudo du joueur est :%s\n",(part->joueurs)[0].pseudo);
    printf("Le pseudo du joueur est :%s\n",(part->joueurs)[1].pseudo);
    return part;
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
    perror("BLABLA");
}

void fermerMemoire(int fd_error){
    if((shmdt(part))<0){
        afficher_erreur(fd_error, "memoire-shmdt\n");
    }
    if((shmctl(shmid, IPC_RMID, NULL))<0){
        afficher_erreur(fd_error, "memoire-shmctl");
    }
}
