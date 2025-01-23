//
// Created by Thomas on 1/22/25.
//
#include "lwp.h"
#include "RoundRobin.h"


struct scheduler rr_publish = {NULL, NULL, rr_admit, rr_remove, rr_next, rr_qlen};
scheduler RoundRobin = &rr_publish;










