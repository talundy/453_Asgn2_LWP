//
// Created by Thomas on 1/22/25.
//
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include "lwp.h"
#include "fp.h"
#include "schedulers.h"

extern void swap_rfiles(rfile* old, rfile* new);

/*
Creates a new lightweight process which executes the given function
with the given argument.
lwp create() returns the (lightweight) thread id of the new thread
or NO THREAD if the thread cannot be created
*/
tid_t lwp_create(lwpfun function, void *argument){
    // create thread
    // use mmap to make space for the thread's memory
    // void* mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
    // add thread to scheduler
    // function(argument) for the thread
}


/*
Starts the LWP system. Converts the calling thread into a LWP
and lwp yield()s to whichever thread the scheduler chooses.
*/
void lwp_start(void){

}

/*
Yields control to another LWP. Which one depends on the sched-
uler. Saves the current LWP’s context, picks the next one, restores
that thread’s context, and returns. If there is no next thread, ter-
minates the program.
*/
void lwp_yield(void){
    
}

/*
Terminates the current LWP and yields to whichever thread the
scheduler chooses. lwp_exit() does not return.
*/
void lwp_exit(int exitval){

}


/*
Waits for a thread to terminate, deallocates its resources, and re-
ports its termination status if status is non-NULL.
Returns the tid of the terminated thread or NO THREAD.
*/
tid_t lwp_wait(int *status){

}


/*
Returns the tid of the calling LWP or NO THREAD if not called by a LWP.
*/
tid_t lwp_gettid(void){

}

/*
Returns the thread corresponding to the given thread ID, or NULL
if the ID is invalid
*/
thread tid2thread(tid_t tid){

}

/*
Causes the LWP package to use the given scheduler to choose the
next process to run. Transfers all threads from the old scheduler
to the new one in next() order. If scheduler is NULL the library
should return to round-robin scheduling
*/
void lwp_set_scheduler(scheduler sched){
    if((void *)tid == NULL){
        // return to round-robin scheduling
        struct scheduler rr_publish = {
                NULL, NULL, rr_admit, rr_remove, rr_next, rr_qlen
        };
        current_scheduler = rr_publish;
        return;
    }
    // Initialize the scheduler if applicable
    if(sched->init != NULL){
        sched->init();
    }
    // Transfer threads from old scheduler to new scheduler
    thread next_thread = current_scheduler->next();
    current_scheduler->remove(next_thread);
    HEAD = next_thread;     // set global HEAD to next_thread
    sched->admit(next_thread);

    while(next_thread != NULL){
        next_thread = current_scheduler->next();
        sched->admit(next_thread);
        // we need to remove() so that this NULL check works
        current_scheduler->remove(next_thread);
    }
    // shutdown current scheduler if applicable
    if(current_scheduler->shutdown != NULL){
        current_scheduler->shutdown();
    }
    // replace old scheduler with new scheduler
    current_scheduler = sched;
}

/*
Returns the pointer to the current scheduler.
*/
scheduler lwp_get_scheduler(void){
    // global pointer
    return current_scheduler;
}
