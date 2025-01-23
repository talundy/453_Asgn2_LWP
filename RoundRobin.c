//
// Created by Thomas on 1/22/25.
//

#include "RoundRobin.h"
#include "lwp.h"

thread HEAD = NULL;

void rr_admit(thread new){
    if(new == NULL){
        return;
    }
    if(HEAD == NULL){
        HEAD = new;
        return;
    }
    thread p = HEAD->prev;
    HEAD->prev = new;
    new->prev = p;
    p->next = new;
    new->next = HEAD;
    return;
}

void rr_remove(thread victim){
}

thread rr_next(void){
}

int rr_qlen(void){
}