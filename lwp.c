//
// Created by Thomas on 1/22/25.
//
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include "include/*.h"


/*
Creates a new lightweight process which executes the given function
with the given argument.
lwp create() returns the (lightweight) thread id of the new thread
or NO THREAD if the thread cannot be created
*/
tid_t lwp_create(lwpfun function, void *argument){
    // create thread
    // add thread to scheduler

    // execve function(argument) for the thread
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
void lwp_set_scheduler(tid_t tid){

}

/*
Returns the pointer to the current scheduler.
*/
scheduler lwp_get_scheduler(void){
    
}
