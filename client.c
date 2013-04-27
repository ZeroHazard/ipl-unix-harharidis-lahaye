#include "header.h"
#include "memoire.h"
#include "util.h"

int main(int argc, char *argv[])
{
    int erreur = 0;
    char ligne[tailleLigne];
    char recu[tailleLigne];
    client c;
    SOCKET sock;
    SOCKADDR_IN sin;
    int fd_error;
    int port;
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
    if(!erreur)
    {
        printf("Veuillez entrez votre pseudo de jeu\n");
        if(fgets(ligne, tailleLigne, stdin)==NULL){
            afficher_erreur(fd_error,"serveur-fgets\n");
            exit(1);
        }
        /* Création de la socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);
        
        /* Configuration de la connexion */
        sin.sin_addr.s_addr = inet_addr("127.0.0.1");
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        
        /* Si le client arrive à se connecter */
        if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR){
            initMemoire(fd_error,0);
            strcpy(c.pseudo,ligne);
            c.csocket = sock;
            send(sock, &c, sizeof(c), 0);
            recv(sock, recu, sizeof(recu), 0);
            lectureMemoire(fd_error);
        }
        else
            printf("Impossible de se connecter\n");
        
        /* On ferme la socket précédemment ouverte */
        fermerMemoire(fd_error);
        close(sock);
        printf("Fermeture du client terminée\n");
        return EXIT_SUCCESS;
    }
}

