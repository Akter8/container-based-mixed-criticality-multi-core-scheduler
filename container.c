#include <stdio.h>
#include <stdlib.h>

#include "container.h"

extern int num_jobs[4];
extern FILE* file_input[4];
extern Container containers[4];


void init_containers()
{
    for (int i = 0; i < 4; i++)
    {
        create_level_jobs(i, &containers[i].jobs);
        input_level_jobs(i, containers[i].jobs);
        sort_level_jobs(i, containers[i].jobs);

        containers[i].curr_crit_level = 4;

        print_level_jobs(i, containers[i].jobs);
        printf("\n");
    }

    containers[0].global = false;
    containers[1].global = false;
    containers[2].global = true;
    containers[3].global = true;
}
