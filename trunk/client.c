#include "header.h"
#include "memoire.h"
#include "util.h"
#include "message.h"
#include <signal.h>
#include <sys/time.h>

void calculDuScore();
void closeSocket();
void positionnerTuile(int);
int score;
int tabPosition[20] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1 };
int tabScore[20] = { 0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70,
		85, 100, 150, 300 };
SOCKET sock;
void int_handler(int null);
int fd_error;
message mTuile;
//int position=1;

int main(int argc, char *argv[]) {
	int erreur = 0;
	char ligne[tailleLigne];
	char recu[tailleLigne];
	client c;
	SOCKADDR_IN sin;
	int port;
	int i;

	message mConf;
	partie* part;
	if (argc < 2 || argc > 3) {
		printf("Usage: %s port,[file]\n", argv[0]);
		exit(1);
	}
	if (argc == 3) {
		if ((fd_error = open(argv[2], O_CREAT | O_WRONLY, 0700)) == -1) {
			afficher_erreur(fd_error,
					"client-erreur lors de l'ouverture du fichier\n");
			exit(1);
		}
	} else {
		fd_error = dup(2);
	}
	signal(SIGINT, int_handler);
	port = atoi(argv[1]);
	if (!erreur) {
		printf("Veuillez entrez votre pseudo de jeu\n");
		if (fgets(ligne, tailleLigne, stdin) == NULL ) {
			closeSocket();
			afficher_erreur(fd_error, "client-fgets-pseudo\n");
			exit(1);
		}
		/* Création de la socket */
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			closeSocket();
			afficher_erreur(fd_error, "client-socket");
		}

		/* Configuration de la connexion */
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		char conf[100];
		char debut[100];
		fd_set setServ;
		FD_SET(sock, &setServ);
		struct timeval tv;
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		/* Si le client arrive à se connecter */
		printf("Connexion ... \n");
		if (connect(sock, (SOCKADDR*) &sin, sizeof(sin)) != SOCKET_ERROR) {
			initMemoire(fd_error, 0);
			strcpy(c.pseudo, ligne);
			c.csocket = sock;
			if ((select(sock + 1, &setServ, NULL, NULL, &tv)) == -1) {
				afficher_erreur(fd_error, "serveur-jeu-select\n");
			}
			fd_set copySet = setServ;
			if (FD_ISSET(sock, &copySet)) {
				printf("Kikoo\n");
				if ((recv(sock, &conf, sizeof(conf), 0)) == 0) {
					closeSocket();
					afficher_erreur(fd_error, "client-recv\n");
				}
			} else {
				printf("Partie en cours \n");
				closeSocket();
				return EXIT_SUCCESS;
			}
			printf("%s\n", conf);
			if ((send(sock, &c, sizeof(c), 0)) < 0) {
				closeSocket();
				afficher_erreur(fd_error, "client-send");
			}
			for (i = 0; i < 20; i++) {
				// On recoit la tuile tiré par le serveur
				if ((recv(sock, &mTuile, sizeof(mTuile), 0)) == 0) {
					closeSocket();
					afficher_erreur(fd_error, "client-recv\n");
				}
				positionnerTuile(0);
				strcpy(mConf.data, "Confirmation du client! \n");
				mConf.type = CONFCHOIXTUILE;
				if ((send(sock, &mConf, sizeof(mConf), 0)) < 0) {
					closeSocket();
					afficher_erreur(fd_error, "client-send");
				}
			}

			// Message pour signaler la fin de partie
			if ((recv(sock, recu, sizeof(recu), 0)) == 0) {
				closeSocket();
				afficher_erreur(fd_error, "client-recv\n");
			}
			calculDuScore();
			send(sock, &score, sizeof(int), 0);
			// Message pour faire lire le client dans la memoire partagee
			// pour afficher les scores
			while (1) {
				if ((part = lectureMemoire(fd_error)) != NULL ) {
					break;
				} else {
					sleep(1);
				}
			}
			int k;
			/*
			 printf("Tableau de tuiles \n");
			 for(k=0;k<20;k++){
			 printf("P:%d T:%d \n",k+1,tabPosition[k]);
			 }
			 */
			printf("FIN\n");
			printf("TABLEAU DES SCORES\n");
			for (k = 0; k < part->nombreJoueur; k++) {
				printf("Pseudo :%s\t Score :%d\n", part->joueurs[k].pseudo,
						part->joueurs[k].score);
			}
		} else
			printf("Impossible de se connecter\n");

		/* On ferme la socket précédemment ouverte */
		closeSocket();
		return EXIT_SUCCESS;
	}
}

void calculDuScore() {
	int i;
	int serie = 1;
	int valPrec = tabPosition[0];
	if (valPrec == 31) {
		valPrec = tabPosition[1];
	}
	int valCourante;
	for (i = 1; i < 20; i++) {
		valCourante = tabPosition[i];
		if (valCourante == 31) {
			valCourante = valPrec;
		}
		if (valCourante >= valPrec) {
			serie++;
		} else {
			score += tabScore[serie - 1];
			serie = 1;
		}
		valPrec = valCourante;
	}
	score += tabScore[serie - 1];
}

void positionnerTuile(int bool) {
	char ligne[tailleLigne];
	if (bool == 1) {
		printf(
				"La case est déjà prise, veuillez entrer une autre position ! :) \n");
	} else {
		printf(
				"La tuile est :%s\n Veuillez entrer la position à laquelle vous voulez insérer la tuile\n",
				mTuile.data);
	}
	if (fgets(ligne, tailleLigne, stdin) == NULL ) {
		afficher_erreur(fd_error, "serveur-fgets-position\n");
		exit(1);
	}
	int position;
	// penser à peut-être utiliser strtol a la place
	position = atoi(ligne);
	int numTuile;
	numTuile = atoi(mTuile.data);
	if (tabPosition[position - 1] == -1) {
		// position-1  car la table commence à 0
		tabPosition[position - 1] = numTuile;
		printf("Position : %d numTuile : %d \n", position, numTuile);
	} else if (tabPosition[position] == -1) {
		tabPosition[position] = numTuile;
		printf(
				"La case était déjà prise, la tuile a été positionné à la case suivante \n");
		printf("Position : %d numTuile : %d \n", position, numTuile);
	} else {
		positionnerTuile(1);
	}
	position++;
}

void int_handler(int null) {
	closeSocket();
	exit(1);
}

void closeSocket() {
	close(sock);
	printf("Fermeture du client terminée\n");
}
