#include "util.h"

void afficher_erreur(int fd_erreur, char* message){
    write(fd_erreur, message, sizeof(message));
    exit(1);
}
