/********************************************************
 *  serveur.c
 *	programme principal du serveur
 * 	methodes utilitaire
 * 	Maxime LAHAYE, Mathieu HARHARIDIS
 * 	avril 2013
 ********************************************************/

#include "header.h"
#include "memoire.h"
#include "util.h"
#include <signal.h>
#include "partie.h"
#include "partie.h"
#include "message.h"
#define JOUEURS_MAX 5
#define TEMPS 10
void suppressionJoueur(client* cl);
typedef struct tabClient {
	client* clients;
	int tailleLogique;
} tabClient;
void handler(int);
void int_handler(int null);
void closeSocket();
void jeu();
void ajoutJoueur(client*);
int boolean = 1;
int fd_error;
fd_set setClient;
partie* part = NULL;
tabClient tabClients;
void envoiClients(message m);
client* ptr;
SOCKET csock = 0;
SOCKET sock;
message mFin;

int main(int argc, char *argv[]) {
	int erreur = 0;
	int i;
	client c;
	int port;
	message scoreR;
	if (argc < 2 || argc > 3) {
		printf("Usage: %s port,[file]\n", argv[0]);
		exit(1);
	}
	if (argc == 3) {
		if ((fd_error = open(argv[2], O_CREAT | O_WRONLY, 0700)) == -1) {
			afficher_erreur(fd_error,
					"serveur-erreur lors de l'ouverture du fichier\n");
			exit(1);
		}
	} else {
		fd_error = dup(2);
	}
	port = atoi(argv[1]);

	while(1){
		boolean=1;
		FD_ZERO(&setClient);
		if ((ptr = tabClients.clients = (client*) malloc(
				JOUEURS_MAX * sizeof(client))) == NULL ) {
			afficher_erreur(fd_error, "serveur-malloc\n");
			exit(1);
		}
		tabClients.tailleLogique = 0;
		/* Socket et contexte d'adressage du serveur */
		SOCKADDR_IN sin;
		socklen_t recsize = sizeof(sin);

		/* Socket et contexte d'adressage du client */
		SOCKADDR_IN csin;
		socklen_t crecsize = sizeof(csin);

		int sock_err;

		if (!erreur) {
			/* Création d'une socket */
			sock = socket(AF_INET, SOCK_STREAM, 0);

			/* Si la socket est valide */
			if (sock != INVALID_SOCKET) {
				//printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

				/* Configuration */
				sin.sin_addr.s_addr = htonl(INADDR_ANY ); /* Adresse IP automatique */
				sin.sin_family = AF_INET; /* Protocole familial (IP) */
				sin.sin_port = htons(port); /* Listage du port */
				sock_err = bind(sock, (SOCKADDR*) &sin, recsize);

				/* Si la socket fonctionne */
				if (sock_err != SOCKET_ERROR) {
					/* Démarrage du listage (mode server) */
					sock_err = listen(sock, JOUEURS_MAX);
					//printf("Listage du port %d...\n", PORT);

					/* Si la socket fonctionne */
					if (sock_err != SOCKET_ERROR) {
						/* Attente pendant laquelle le client se connecte */
						printf(
								"Patientez pendant que le client se connecte sur le port %d...\n",
								port);
						signal(SIGINT, int_handler);
						struct sigaction sa;
						sa.sa_handler = handler;
						sigemptyset(&sa.sa_mask);
						sa.sa_flags = 0;
						sigaction(SIGALRM, &sa, NULL );
						initMemoire(fd_error, 1);
						while (boolean == 1) {
							if ((csock = accept(sock, (SOCKADDR*) &csin, &crecsize))
									< 0) {
								break;
							}
							// Le serveur envoie un message de confirmation de la connexion
							send(csock, "Vous etes bien connecté",
									sizeof("Vous etes bien connecté"), 0);
							printf(
									"Un client se connecte avec la socket %d de %s:%d\n",
									csock, inet_ntoa(csin.sin_addr),
									htons(csin.sin_port));
							// Le serveur recoit la structure client de celui qui s'est connecté
							if (recv(csock, &c, sizeof(c), 0) < 0) {
								closeSocket();
								afficher_erreur(fd_error, "serveur-recv\n");
							}
							ptr = tabClients.clients + tabClients.tailleLogique;
							strcpy(ptr->pseudo, c.pseudo);
							ptr->csocket = csock;
							FD_SET(csock, &setClient);
							ajoutJoueur(ptr);
							if (tabClients.tailleLogique == 0) {
								alarm(TEMPS);
							}
							printf("Vous avez bien été inscrit %s\n", ptr->pseudo);
							tabClients.tailleLogique++;
						}
						//On regarde si tous le clients sont toujours connectés
						for (i = 0; i < tabClients.tailleLogique; i++) {
							char ligne[1024] = "";
							if ((recv((tabClients.clients + i)->csocket, ligne,
									sizeof(ligne), MSG_DONTWAIT)) == 0) {
								suppressionJoueur(tabClients.clients + i);
								printf("Deconnexion du joueur: %s\n",
										(tabClients.clients + i)->pseudo);
								if (i == tabClients.tailleLogique) {
									ptr--;
								} else {
									client ptr3 = *(tabClients.clients
											+ tabClients.tailleLogique - 1);
									*(tabClients.clients + i) = ptr3;
									ptr--;
									i--;
								}
								tabClients.tailleLogique--;
							}
						}
						if (tabClients.tailleLogique >= 2) {
							printf("La partie commence!\n");
							ecritureMemoireJoueurs(fd_error, part);
							jeu();
							interdireLecture();
							mFin.type = FINPARTIE;
							strcpy(mFin.data,
									"Fin car vous êtes le dernier joueur, vous êtes le gagnant !");
							for (i = 0; i < tabClients.tailleLogique; i++) {
								if ((send((tabClients.clients + i)->csocket, &mFin,
										sizeof(message), 0)) < 0) {
									closeSocket();
									afficher_erreur(fd_error, "serveur-send");
								}
								int ret;
								if ((ret = recv((tabClients.clients + i)->csocket,
										&scoreR, sizeof(message), 0)) < 0) {
									closeSocket();
									afficher_erreur(fd_error, "serveur-recv");
								}
								printf("\n");
								printf("%s\n", scoreR.data);
								part->joueurs[i].score = atoi(scoreR.data);
								sleep(1);
							}
							ecritureMemoireJoueurs(fd_error, part);
							autoriserLecture();
							sleep(1);
						} else {
							printf(
									"Il n'y a pas assez de joueur pour commencer la partie\n");
						}
					} else
						afficher_erreur(fd_error, "serveur-listen\n");
				} else
					afficher_erreur(fd_error,
							"Le port est déjà occupé par un autre serveur\n");
			} else
				afficher_erreur(fd_error, "serveur-socket\n");

		}
		/* Fermeture de la socket client et de la socket serveur */
		sleep(1);
		closeSocket();
		printf("FIN SERVEUR\n");
	}
	return EXIT_SUCCESS;
}

void closeSocket() {
	int i;
	fermerMemoire(fd_error);
	for (i = 0; i < tabClients.tailleLogique; i++) {
		close((tabClients.clients + i)->csocket);
	}
	close(sock);
	printf("Fermeture du serveur terminée\n");

}

void jeu() {
	int i;
	int tuileEncours = 0;
	message mTuile;
	message mConf;
	message debut;

	// envoi du message de debut de partie
	sprintf(debut.data, "La partie commence ! \n");
	debut.type = DEBUTPARTIE;
	envoiClients(debut);

	// initialisation des tuiles
	initTuiles(fd_error);

	for (i = 0; i < 20; i++) {
		tuileEncours = tirerTuile(fd_error);
		mTuile.type = TUILE;
		if ((sprintf(mTuile.data, "%d", tuileEncours)) < 0) {
			closeSocket();
			afficher_erreur(fd_error, "serveur-jeu-sprintf\n");
		}
		printf("SERVEUR= %s\n", mTuile.data);
		envoiClients(mTuile);
		int retval;
		int maxValSock;
		int j;
		int k;
		int nbrJoueurs = tabClients.tailleLogique;
		fd_set copySet;
		for (j = 0; j < tabClients.tailleLogique; j++) {
			if (maxValSock < tabClients.clients[j].csocket) {
				maxValSock = tabClients.clients[j].csocket;
			}
		}
		while (1) {
			copySet = setClient;
			if ((select(maxValSock + 1, &copySet, NULL, NULL, NULL)) == -1) {
				closeSocket();
				afficher_erreur(fd_error, "serveur-jeu-select\n");
			}
			int l;
			for (k = 0; k < tabClients.tailleLogique; k++) {
				client cl = tabClients.clients[k];
				if (FD_ISSET(cl.csocket, &copySet)) {
					if ((recv(cl.csocket, &mConf, sizeof(mConf), MSG_DONTWAIT))
							== 0) {
						printf("Deconnexion du joueur: %s\n",
								(tabClients.clients + k)->pseudo);
						if (k == tabClients.tailleLogique) {
							ptr--;
						} else {
							client ptr3 = *(tabClients.clients
									+ tabClients.tailleLogique - 1);
							*(tabClients.clients + k) = ptr3;
							ptr--;
							k--;
						}
						tabClients.tailleLogique--;
						nbrJoueurs--;
						if (tabClients.tailleLogique < 2) {
							printf(
									"Il n'y a plus assez de joueurs pour continuer la partie\n");
							return;
						}
						break;
					} else {
						nbrJoueurs--;
					}
				}
			}
			if (nbrJoueurs == 0) {
				break;
			}
		}

	}
}

void envoiClients(message m) {
	int i;
	for (i = 0; i < tabClients.tailleLogique; i++) {
		if ((send((tabClients.clients + i)->csocket, &m, sizeof(m), 0)) < 0) {
			closeSocket();
			afficher_erreur(fd_error, "serveur-envoiClient-send");
		}
	}
}

void suppressionJoueur(client* cl) {
	int i;
	for (i = 0; i < part->nombreJoueur; i++) {
		if (strcmp(cl->pseudo, part->joueurs[i].pseudo) == 0) {
			if (i < part->nombreJoueur - 1) {
				strcpy(part->joueurs[i].pseudo,
						part->joueurs[part->nombreJoueur - 1].pseudo);
				part->joueurs[i].score =
						part->joueurs[part->nombreJoueur - 1].score;
			}
			strcpy(part->joueurs[part->nombreJoueur - 1].pseudo, "");
			part->joueurs[part->nombreJoueur - 1].score = -1;
		}
	}
	part->nombreJoueur = part->nombreJoueur - 1;
}

void ajoutJoueur(client* cl) {
	joueur* j;
	j = initJoueurs(cl->pseudo, fd_error);
	part->nombreJoueur = part->nombreJoueur + 1;
	part->joueurs[part->nombreJoueur - 1] = *j;
}

void handler(int null) {
	boolean = 0;
}

void int_handler(int null) {
	closeSocket();
	exit(1);
}
