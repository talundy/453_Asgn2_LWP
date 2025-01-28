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
#include "RoundRobin.h"


// extern void swap_rfiles(rfile* old, rfile* new);
struct scheduler cur_sched;
scheduler current_scheduler;
tid_t next_tid;
long pg_size;




/*
Creates a new lightweight process which executes the given function
with the given argument.
lwp create() returns the (lightweight) thread id of the new thread
or NO THREAD if the thread cannot be created
*/
tid_t lwp_create(lwpfun function, void *argument){
    if(current_scheduler == NULL || current_scheduler->rr_qlen() == 0){
        lwp_start();
    }
    // create thread
    // use mmap to make space for the thread's memory
    thread new = (thread)mmap(NULL, pg_size, PROT_READ|PROT_WRITE, 
            MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
    if(new == MAP_FAILED){
        fprintf(stderr, "New thread memory allocation failed.\n");
    }
    new->tid = next_id;
    new->stack = (unsigned long*)new/*+ TODO: ALIGNMENT*/;
    new->stacksize = pg_size - sizeof(struct threadinfo_st) /* - TODO: ALIGNMENT*/;
    new->state.fxsave = FPU_INIT;
    new->status = LWP_LIVE;
    //TODO: lib_one, lib_two, sched_one, sched_two, exited are configured
    // in RoundRobin.h
    // void* mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
    // add thread to scheduler
    current_scheduler->admit(new);
    // function(argument) for the thread
    next_id += 1;
    return new->tid;
}


/*
Starts the LWP system. Converts the calling thread into a LWP
and lwp yield()s to whichever thread the scheduler chooses.
*/
void lwp_start(void){
    //TODO: sysconf & getrlimit to find page size
    cur_sched = {NULL, NULL, rr_admit, rr_remove, rr_next, rr_qlen};
    current_scheduler = &cur_sched;
    next_tid = 1;
    //transform calling thread into an LWP but don't allocate a new stack
    thread first_th = (thread)mmap(NULL, sizeof(struct threadinfo_st),
            PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
    if(first_th = MAP_FAILED){
        fprintf(stderr, "First thread memory allocation failed\n");
        
        
    }
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
