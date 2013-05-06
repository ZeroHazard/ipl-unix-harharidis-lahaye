CC=cc
all:client serveur
client : client.o util.o memoire.o partie.o joueur.o
	$(CC) -o client client.o util.o memoire.o partie.o joueur.o
serveur : serveur.o util.o memoire.o partie.o joueur.o
	$(CC) -o serveur serveur.o util.o memoire.o partie.o joueur.o
client.o : client.c header.h util.h memoire.h message.h
	$(CC) -c  client.c
serveur.o : serveur.c header.h util.h memoire.h message.h
	$(CC) -c  serveur.c
util.o : util.c util.h   
	$(CC) -c  util.c
memoire.o : memoire.c memoire.h util.h message.h
	$(CC) -c memoire.c 
joueur.o : joueur.c joueur.h	header.h
	$(CC) -c joueur.c
partie.o : partie.c partie.h joueur.h
	$(CC) -c partie.c	
	
