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
        // Set HEAD to new, point to itself
        HEAD = new;
        new->next = new;
        new->prev = new;
        return;
    }
    // Insert new thread at HEAD->prev
    thread p = HEAD->prev;
    HEAD->prev = new;
    new->prev = p;
    p->next = new;
    new->next = HEAD;
    return;
}

void rr_remove(thread victim){
    if(victim != NULL){
        // Change prev and next pointers
        victim->prev->next = victim->next;
        victim->next->prev = victim->prev;
    }
}

thread rr_next(void){
    if(HEAD == NULL){
        return NULL;
    }
    // If HEAD is the only thread in the pool
    if((HEAD->next == HEAD) || (HEAD->next == NULL)){
        return HEAD;
    }
    // Assign HEAD->next as HEAD and return the former head
    HEAD = HEAD->next;  // revise later
    return HEAD->prev;
}

int rr_qlen(void){
}