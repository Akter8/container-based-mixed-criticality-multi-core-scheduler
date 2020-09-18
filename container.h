#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdbool.h>

#include "job.h"

typedef struct
{
    // No need for holding the container level, index in array will do that.
    int curr_crit_level;
    int num_jobs;
    Job *jobs;
    int curr_job_index;
    bool jobs_left;
}
Container;


// void init_containers();
void init_container(int, Container *);

#endif