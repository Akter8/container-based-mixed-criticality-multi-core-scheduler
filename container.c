#include <stdio.h>
#include <stdlib.h>

#include "container.h"

extern FILE* file_input[4];

/*
 * Initialises a container and inputs the jobs associated with that container.
 */
void init_container(int criticality, Container *container)
{
    fscanf(file_input[criticality], "%d", &container->num_jobs);
    
    create_level_jobs(container->num_jobs, &container->jobs);

    input_level_jobs(criticality, container->num_jobs, container->jobs);

    sort_level_jobs(container->num_jobs, container->jobs);

    container->curr_crit_level = 3;
    container->curr_job_index = 0;

    if (container->num_jobs > 0)
        container->jobs_left = true;
    else
        container->jobs_left = false;

    print_level_jobs(criticality, container->num_jobs, container->jobs);
}
