#include "header.h"

int main(int argc, char *argv[])
{
    int erreur = 0;
    char ligne[tailleLigne];
    char recu[tailleLigne];
    client c;
    SOCKET sock;
    SOCKADDR_IN sin;
    int port;
    if(argc <2 || argc > 3){
        printf("Usage: %s port,[file]\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);
    if(!erreur)
    {
        printf("Veuillez entrez votre pseudo de jeu\n");
        if(fgets(ligne, tailleLigne, stdin)==NULL){
            perror("problem fgets?\n");
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
            strcpy(c.pseudo,ligne);
            c.csocket = sock;
            send(sock, &c, sizeof(c), 0);
            recv(sock, recu, sizeof(recu), 0);
        }
        else
            printf("Impossible de se connecter\n");
        
        /* On ferme la socket précédemment ouverte */
        close(sock);
        printf("Fermeture du client terminée\n");
        return EXIT_SUCCESS;
    }
}
