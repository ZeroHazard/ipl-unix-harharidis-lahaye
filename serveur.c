#include "header.h"
#include "memoire.h"
#include "util.h"
#include <signal.h>
#include "partie.h"
#define JOUEURS_MAX 5
#define TEMPS 30
void suppressionJoueur(client* cl);
typedef struct tabClient {
	client* clients;
	int tailleLogique;
} tabClient;
void handler(int);
void ajoutJoueur(client*);
int boolean = 1;
int fd_error;
partie* part = NULL;
int main(int argc, char *argv[])
{
    int erreur = 0;
    int i;
    tabClient tabClient;
    client c;
    client* ptr;
    int port;
    int scoreR;
    if(argc <2 || argc > 3){
        printf("Usage: %s port,[file]\n", argv[0]);
        exit(1);
    }
    if(argc == 3){
        if((fd_error = open(argv[2], O_CREAT|O_WRONLY, 0700))==-1){
            afficher_erreur(fd_error,"serveur-erreur lors de l'ouverture du fichier\n");
            exit(1);
        }
    } else {
        fd_error = dup(2);
    }
    port = atoi(argv[1]);
    if((ptr = tabClient.clients = (client*)malloc(JOUEURS_MAX*sizeof(client)))==NULL){
        afficher_erreur(fd_error,"serveur-malloc\n");
        exit(1);
    }
    tabClient.tailleLogique = 0;
    /* Socket et contexte d'adressage du serveur */
    SOCKADDR_IN sin;
    SOCKET sock;
    socklen_t recsize = sizeof(sin);
    
    /* Socket et contexte d'adressage du client */
    SOCKADDR_IN csin;
    SOCKET csock = 0;
    socklen_t crecsize = sizeof(csin);
    
    int sock_err;
    
    
    if(!erreur)
    {
        /* Création d'une socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);
        
        /* Si la socket est valide */
        if(sock != INVALID_SOCKET)
        {
            //printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);
            
            /* Configuration */
            sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* Adresse IP automatique */
            sin.sin_family = AF_INET;                 /* Protocole familial (IP) */
            sin.sin_port = htons(port);               /* Listage du port */
            sock_err = bind(sock, (SOCKADDR*)&sin, recsize);
            
            /* Si la socket fonctionne */
            if(sock_err != SOCKET_ERROR)
            {
                /* Démarrage du listage (mode server) */
                sock_err = listen(sock, JOUEURS_MAX);
                //printf("Listage du port %d...\n", PORT);
                
                /* Si la socket fonctionne */
                if(sock_err != SOCKET_ERROR)
                {
                    /* Attente pendant laquelle le client se connecte */
                    printf("Patientez pendant que le client se connecte sur le port %d...\n", port);
                    struct sigaction sa;
                    sa.sa_handler = handler;
                    sigemptyset(&sa.sa_mask);
                    sa.sa_flags = 0;
                    sigaction(SIGALRM,&sa,NULL);
                    initMemoire(fd_error,1);
                    while(boolean == 1){
                        //Si c'est le premier inscrit on lance l'alarm
                        // La condition du while est un boolean qu'on mettra a fals dans la methode handler
                        //Gerer la deconnexion d'un joueur en inscription
                        if(tabClient.tailleLogique == 0){
                            alarm(TEMPS);
                        }
                        if((csock = accept(sock, (SOCKADDR*)&csin, &crecsize))<0){
                            break;
                        }
                        printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
                        if(recv(csock, &c, sizeof(c), 0)<0){
                            afficher_erreur(fd_error,"serveur-recv\n");
                        }
                        ptr = tabClient.clients+tabClient.tailleLogique;
                        strcpy(ptr->pseudo,c.pseudo);
                        ptr->csocket = csock;
                        ajoutJoueur(ptr);
                        printf("Vous avez bien été inscrit %s\n", ptr->pseudo);
                        tabClient.tailleLogique++;
                    }
                    //On regarde si tous le clients sont toujours connectés
                    for (i=0; i<tabClient.tailleLogique; i++) {
                        char ligne[1024]= "";
                        if((recv((tabClient.clients+i)->csocket, ligne, sizeof(ligne), MSG_DONTWAIT))==0){
                            suppressionJoueur(tabClient.clients+i);
                            printf("Deconnexion du joueur: %s\n", (tabClient.clients+i)->pseudo);
                            if(i==tabClient.tailleLogique){
                                ptr--;
                            }else{
                                client ptr3 = *(tabClient.clients+tabClient.tailleLogique-1);
                                *(tabClient.clients+i) = ptr3;
                                ptr--;
                                i--;
                            }
                            tabClient.tailleLogique--;
                        }
                    }
                    if(tabClient.tailleLogique >= 2){
                        printf("La partie commence!\n");
                        ecritureMemoireJoueurs(fd_error, part);
                        // implementation future du jeu
                        interdireLecture();
						for (i = 0; i < tabClient.tailleLogique; i++) {
							send((tabClient.clients + i)->csocket, "FIN",
                                 sizeof("FIN"), 0);
							recv((tabClient.clients + i)->csocket, &scoreR, sizeof(int),
                                 0);
							part->joueurs[i].score = scoreR;
							sleep(1);
						}
                        printf("Le serveur lit: %s\n", part->joueurs[0].pseudo);
                        printf("Le serveur lit: %d\n", part->joueurs[0].score);
                        printf("Le serveur lit: %s\n", part->joueurs[1].pseudo);
                        printf("Le serveur lit: %d\n", part->joueurs[1].score);
						ecritureMemoireJoueurs(fd_error,part);
                        autoriserLecture();
						sleep(1);
                    } else {
                        printf("Il n'y a pas assez de joueur pour commencer la partie\n");
                    }
                }
                else
                    afficher_erreur(fd_error,"serveur-listen\n");
            }
            else
                afficher_erreur(fd_error,"serveur-bind\n");
            
            /* Fermeture de la socket client et de la socket serveur */
            fermerMemoire(fd_error);
            close(csock);
            close(sock);
            printf("Fermeture du serveur terminée\n");
        }
        else
            afficher_erreur(fd_error,"serveur-socket\n");
    }
    
    return EXIT_SUCCESS;
}
void suppressionJoueur(client* cl){
    int i;
    for (i = 0; i<part->nombreJoueur; i++) {
        if(strcmp(cl->pseudo, part->joueurs[i].pseudo)==0){
            if(i < part->nombreJoueur-1){
                strcpy(part->joueurs[i].pseudo, part->joueurs[part->nombreJoueur-1].pseudo);
                part->joueurs[i].score = part->joueurs[part->nombreJoueur-1].score;
            }
            strcpy(part->joueurs[part->nombreJoueur-1].pseudo,"");
            part->joueurs[part->nombreJoueur-1].score = -1;
        }
    }
    part->nombreJoueur = part->nombreJoueur-1;
}

void ajoutJoueur(client* cl) {
	joueur* j;
	j = initJoueurs(cl->pseudo, fd_error);
	part->nombreJoueur = part->nombreJoueur + 1;
	part->joueurs[part->nombreJoueur - 1] = *j;
}

void handler(int null){
    boolean = 0;
}
