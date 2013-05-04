/*
 * message.c
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */


#include "message.h"

//crée un message
message * initMessage(int type, char* data){
    message * m;
    if((m=(message *)malloc(sizeof(message)))==NULL){
        return NULL;
    }
    reinitMessage(m, type, data);
    return m;
}

void reinitMessage(message* m, int type, char* data){
    m->type=type;
    strcpy(m->data, data);
}
