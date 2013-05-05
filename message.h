/*
 * message.h
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
#define FINPARTIE 0
#define FINTOUR 1
#define DECONNEXION 2
#define TUILE 3
#define CONFCHOIXTUILE 4
#define SCORE 5
#define DEBUTPARTIE 6;


#include "partie.h"
#define BUFFERSIZE 512

typedef struct message {
    int type;
    char data[BUFFERSIZE];
}message;
void reinitMessage(message*, int, char*);
message * initMessage(int,char*);
#endif /* MESSAGE_H_ */
