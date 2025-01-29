//
// Created by Thomas on 1/22/25.
//
#include <sys/mman.h>       // mmap, munmap
#include <sys/resource.h>   // getrlimit
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>         // sysconf
#include "lwp.h"
#include "fp.h"
#include "schedulers.h"
#include "RoundRobin.h"

//TODO: make this shit fucking work

// struct
struct scheduler cur_sched;
// pointer to the current scheduler
scheduler current_scheduler;
// global pointer to the head of all threads (for finding zombies)
thread LWP_HEAD;
// global pointer to the current thread, for scheduling
thread HEAD;
tid_t next_tid = 1;
unsigned long* stack_base;
// init to 0 so we can check if it has been calculated
long page_size, stack_size = 0;



/*
Creates a new lightweight process which executes the given function
with the given argument.
lwp create() returns the (lightweight) thread id of the new thread
or NO THREAD if the thread cannot be created
*/
tid_t lwp_create(lwpfun function, void *argument){
    // calculate stack size if it hasn't been done yet
    calculate_stack_size();

    if(current_scheduler == NULL || current_scheduler->qlen() == 0){
        lwp_start();
    }
    // create thread
    // use mmap to make space for the thread's memory
    thread new = (thread)mmap(
            NULL, stack_size, PROT_READ|PROT_WRITE,
            MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
    if(new == MAP_FAILED){
        fprintf(stderr, "New thread memory allocation failed.\n");
    }
    // add to global list (not scheduler)
    add_thread_to_list(new);

    new->tid = next_tid;

    // set up the stack ptr
    new->stack = ((unsigned long*)new)+sizeof(context);
            // cast to integer for alignment
    new->stack = (unsigned long*)align(((uintptr_t)new->stack));
    //TODO: account for alignment
    new->stacksize = stack_size - sizeof(context);
    // save current registers into new->state
    swap_rfiles(&(new->state), NULL);
    new->state.fxsave = FPU_INIT;
    new->status = LWP_LIVE;
    //TODO: lib_one, lib_two, sched_one, sched_two, exited are configured
    // in RoundRobin.h
    // void* mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);

    // add thread to scheduler
    current_scheduler->admit(new);
    // function(argument) for the thread
    next_tid += 1;
    return new->tid;
}


/*
Starts the LWP system. Converts the calling thread into a LWP
and lwp yield()s to whichever thread the scheduler chooses.
*/
void lwp_start(void){
    struct scheduler cur_sched = {
            NULL, NULL, rr_admit, rr_remove, rr_next, rr_qlen
    };
    current_scheduler = &cur_sched;
    next_tid = 1;
    //transform calling thread into an LWP but don't allocate a new stack
    thread first_th = (thread)mmap(NULL, sizeof(struct threadinfo_st),
            PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
    if(first_th == MAP_FAILED){
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
    // Save the current LWP's context
    swap_rfiles(NULL, &(HEAD->state));
    // Pick the next one
    HEAD = current_scheduler->next();
    // Restore the next thread's context
    swap_rfiles(&(HEAD->state), NULL);
    // Return
}

/*
Terminates the current LWP and yields to whichever thread the
scheduler chooses. lwp_exit() does not return.
*/
void lwp_exit(int exitval){
    HEAD->status = exitval;
    lwp_yield();
}


/*
Waits for a thread to terminate, deallocates its resources, and re-
ports its termination status if status is non-NULL.
Returns the tid of the terminated thread or NO THREAD.
*/
tid_t lwp_wait(int *status){
    // how to determine if a thread has terminated?
    return NO_THREAD;
}


/*
Returns the tid of the calling LWP or NO THREAD if not called by a LWP.
*/
tid_t lwp_gettid(void){
    // convention is head is current lwp
    if(HEAD == NULL){
        return NO_THREAD;
    }
    return HEAD->tid;
}

/*
Returns the thread corresponding to the given thread ID, or NULL
if the ID is invalid
*/
thread tid2thread(tid_t tid){
    //TODO: use lib_one, lib_two for global thread list instead
    // in order to capture zombie threads
    //
    if(tid == NO_THREAD){
        return NULL;
    }
    // iterate through the list of threads
    thread current = HEAD;
    while(current != NULL){
        if(current->tid == tid){
            return current;
        }
        current = current->sched_one;
        // does not account for nonzero invalid thread
    }
    return NULL;
}

/*
Causes the LWP package to use the given scheduler to choose the
next process to run. Transfers all threads from the old scheduler
to the new one in next() order. If scheduler is NULL the library
should return to round-robin scheduling
*/
void lwp_set_scheduler(scheduler sched){
    if((void *)sched == NULL){
        // return to round-robin scheduling
        struct scheduler rr_publish = {
                NULL, NULL, rr_admit, rr_remove, rr_next, rr_qlen
        };
        current_scheduler = &rr_publish;
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

void calculate_stack_size(void){
    // if the stack size has already been calculated, return
    if (stack_size != 0){
        return;
    }
    // gets the page size
    if((page_size = sysconf(_SC_PAGE_SIZE)) == -1){
        fprintf(stderr, "sysconf failed.\n");
    }
    // get the stack resource limit, if it exists
    struct rlimit rlimit;
    int result = getrlimit(RLIMIT_STACK, &rlimit);
    if(result != 0){
        fprintf(stderr, "getrlimit failed.\n");
    }
    // check the *soft* limit
    if((rlimit.rlim_cur == RLIM_INFINITY)
    || (rlimit.rlim_cur == DOES_NOT_EXIST)){
        // if the soft limit is infinite or DNE, set the stack size to 8MB
        stack_size = EIGHT_MB;
    } else {
        // otherwise, use the soft limit
        stack_size = rlimit.rlim_cur;
    }
    // use int truncation to get a stack size
    // that is a multiple of the page size
    stack_size = (stack_size/page_size)*page_size;
}

void add_thread_to_list(thread new){
    // uses singly linked list. just thought it was easier.
    if(LWP_HEAD == NULL){
        LWP_HEAD = new;
        return;
    }else{
        thread current = LWP_HEAD;
        while(current->sched_one != NULL){
            current = current->sched_one;
        }
        current->sched_one = new;
    }
}

thread find_thread_by_tid(tid_t tid){
    // returns the thread with the given tid,
    // or NULL if the tid is invalid
    // see this is simple
    thread current = LWP_HEAD;
    while(current != NULL){
        if(current->tid == tid){
            return current;
        }
        current = current->sched_one;
    }
    return NULL;
}