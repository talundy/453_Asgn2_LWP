//
// Created by Thomas on 1/22/25.
//
#include <stdlib.h>
#include <unistd.h>
#include "RoundRobin.h"
#include "lwp.h"

// this is the head for all thread pools
thread HEAD = NULL;
/* Initialize the scheduler to the default */


/* Initialize the scheduler to the default */
// removed because current_scheduler is defined here as a struct
// and defined as a pointer in lwp.c
//struct scheduler current_scheduler = {
//    NULL, NULL, rr_admit, rr_remove, rr_next, rr_qlen
//};

void rr_admit(thread new){
    if(new == NULL){
        return;
    }
    if(HEAD == NULL){
        // Set HEAD to new, point to itself
        HEAD = new;
        new->sched_one = new;
        new->sched_two = new;
        return;
    }
    // Insert new thread at HEAD->prev
    thread p = HEAD->sched_two;
    HEAD->sched_two = new;
    new->sched_two = p;
    p->sched_one = new;
    new->sched_one = HEAD;
    return;
}

void rr_remove(thread victim){
    if(victim != NULL){
        // If victim is the only thread in the pool
        if((victim->sched_one == victim) && (victim->sched_two == victim)){
            // Preferred convention for a single thread pool
            HEAD = NULL;
            return;
        } else if ((victim->sched_one == NULL) && (victim->sched_two == NULL)){
            // Convention is this should never happen
            HEAD = NULL;
            return;
        }
        // Change prev and next pointers
        victim->sched_two->sched_one = victim->sched_one;
        victim->sched_one->sched_two = victim->sched_two;
    }
}

thread rr_next(void){
    if(HEAD == NULL){
        return NULL;
    }
    // If HEAD is the only thread in the pool
    if((HEAD->sched_one == HEAD) || (HEAD->sched_one == NULL)){
        return HEAD;
    }
    return HEAD->sched_one; // convention is the HEAD current lwp
}

int rr_qlen(void){
    int count = 0;
    if(HEAD == NULL){
        return 0;
    }
    count++;
    thread thr = HEAD->sched_one;
    while(thr != HEAD){
        thr = thr->sched_one;
        count++;
    }
    return count;
}