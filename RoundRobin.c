//
// Created by Thomas on 1/22/25.
//

#include "RoundRobin.h"
#include "lwp.h"



void rr_admit(thread new){
}

void rr_remove(thread victim){
    if(victim != NULL){
        // Change prev and next pointers
        victim->prev->next = victim->next;
        victim->next->prev = victim->prev;
    }
}

thread rr_next(void){

}

int rr_qlen(void){
}