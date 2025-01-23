//
// Created by Thomas on 1/22/25.
//
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

#include "fp.h"
#include "lwp.h"
#include "util.h"


tid_t lwp_create(lwpfun function, void *argument){

}

void lwp_start(void){

}

void lwp_yield(void){
    
}

void lwp_exit(int exitval){

}

tid_t lwp_wait(int *status){

}

tid_t lwp_gettid(void){

}

thread tid2thread(tid_t tid){

}

void lwp_set_scheduler(tid_t tid){

}

scheduler lwp_get_scheduler(void){
    
}
