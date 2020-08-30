#include <stdio.h>
#include <stdlib.h>

#include "container.h"

extern FILE* file_input[4];


void init_container(int criticality, Container *container)
{
    fscanf(file_input[criticality], "%d", &container->num_jobs);
    
    create_level_jobs(container->num_jobs, &container->jobs);

    input_level_jobs(criticality, container->num_jobs, container->jobs);

    sort_level_jobs(container->num_jobs, container->jobs);

    print_level_jobs(criticality, container->num_jobs, container->jobs);
}
