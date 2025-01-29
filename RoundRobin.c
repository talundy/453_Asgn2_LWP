//
// Created by Thomas on 1/22/25.
//
#include <stdlib.h>
#include <unistd.h>
#include "RoundRobin.h"
#include "lwp.h"

// this is the head for all thread pools
thread HEAD = NULL;
void rr_admit(thread new);
void rr_remove(thread victim);
void rr_next();
void rr_qlen();
/* Initialize the scheduler to the default */


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
        // If victim is the only thread in the pool
        if((victim->next == victim) && (victim->prev == victim)){
            // Preferred convention for a single thread pool
            HEAD = NULL;
            return;
        } else if ((victim->next == NULL) && (victim->prev == NULL)){
            // Convention is this should never happen
            HEAD = NULL;
            return;
        }
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
    return HEAD->next; // convention is the HEAD current lwp
}

int rr_qlen(void){
    int count = 0;
    if(HEAD == NULL){
        return 0;
    }
    count++;
    thread thr = HEAD->next;
    while(thr != HEAD){
        thr = thr->next;
        count++;
    }
    return count;
}