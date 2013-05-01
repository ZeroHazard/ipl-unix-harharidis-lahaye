/*
 * message.h
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "partie.h"
#define BUFFERSIZE 512

typedef struct message {
    int type;
    char data[BUFFERSIZE];
}message;
void reinitMessage(message* m);

#endif /* MESSAGE_H_ */
