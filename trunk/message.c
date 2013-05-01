/*
 * message.c
 *
 *  Created on: Apr 25, 2013
 *      Author: afro
 */


#include "message.h"

//crée un message
message * initMessage(){
    message * m;
    if((m=(message *)malloc(sizeof(message)))==NULL){
        return NULL;
    }
    reinitMessage(m);
    return m;
}

void reinitMessage(message* m){
    m->type=-1;
    strncpy(m->data,"\0",1);
}
