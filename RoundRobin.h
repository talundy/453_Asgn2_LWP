//
// Created by Thomas on 1/22/25.
//

#ifndef ASGN2_ROUNDROBIN_H
#define ASGN2_ROUNDROBIN_H

#include "lwp.h"

// admit new thread to the pool
void rr_admit(thread new);
// remove a thread from the pool
void rr_remove(thread victim);
// return the next thread to run
thread rr_next(void);
// return the number of threads in the pool
int rr_qlen(void);


#endif //ASGN2_ROUNDROBIN_H
