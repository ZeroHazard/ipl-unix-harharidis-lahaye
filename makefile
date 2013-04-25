CC=cc
all:client serveur
client : client.o util.o memoire.o
	$(CC) -o client client.o util.o memoire.o
serveur : serveur.o util.o memoire.o
	$(CC) -o serveur serveur.o util.o memoire.o
client.o : client.c header.h util.h memoire.h
	$(CC) -c  client.c
serveur.o : serveur.c header.h util.h memoire.h
	$(CC) -c  serveur.c
util.o : util.c util.h   
	$(CC) -c  util.c
memoire.o : memoire.c memoire.h util.h
	$(CC) -c memoire.c util.o
