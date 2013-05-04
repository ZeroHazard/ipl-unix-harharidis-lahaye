#include "header.h"
#include "memoire.h"
#include "util.h"
#include "message.h"

void calculDuScore();
void closeSocket();
int score;
SOCKET sock;

int main(int argc, char *argv[]) {
	int erreur = 0;
	char ligne[tailleLigne];
	char recu[tailleLigne];
	client c;
	SOCKADDR_IN sin;
	int fd_error;
	int port;
	int k;
    int i;
    message mTuile;
    message mConf;
	partie* part;
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
	if (!erreur) {
		printf("Veuillez entrez votre pseudo de jeu\n");
		if (fgets(ligne, tailleLigne, stdin) == NULL ) {
			afficher_erreur(fd_error, "serveur-fgets\n");
			exit(1);
		}
		/* Création de la socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);
        
		/* Configuration de la connexion */
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
        
		/* Si le client arrive à se connecter */
		if (connect(sock, (SOCKADDR*) &sin, sizeof(sin)) != SOCKET_ERROR) {
			initMemoire(fd_error, 0);
			strcpy(c.pseudo, ligne);
			c.csocket = sock;
			send(sock, &c, sizeof(c), 0);
            for (i=0; i<20; i++) {
                // On recoit la tuile tiré par le serveur
                if((recv(sock, &mTuile, sizeof(mTuile), 0))==0){
                    closeSocket();
                    afficher_erreur(fd_error, "client-recv\n");
                }
                printf("La tuile est :%s\n", mTuile.data);
                strcpy(mConf.data,"Confirmation du client!");
                mConf.type = CONFCHOIXTUILE;
                send(sock, &mConf, sizeof(mConf), 0);
            }
            
			// Message pour signaler la fin de partie
			if((recv(sock, recu, sizeof(recu), 0))==0){
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
			printf("Nombre de joueurs :%d\n", part->nombreJoueur);
			for (k = 0; k < part->nombreJoueur; k++) {
				printf("Pseudo :%s\n", part->joueurs[k].pseudo);
				printf("Score :%d\n", part->joueurs[k].score);
			}
		} else
			printf("Impossible de se connecter\n");
        
		/* On ferme la socket précédemment ouverte */
		closeSocket();
		return EXIT_SUCCESS;
	}
}

void closeSocket(){
    close(sock);
    printf("Fermeture du client terminée\n");
}

void calculDuScore() {
	score = 10;
}

