#include <stdlib.h>
#include <stdio.h>

#include "job.h"

extern FILE* file_input[4];
extern Job* jobs[4];

extern int num_jobs[4];

void input_jobs()
{
    for (int cri = 0; cri < 4; cri++)
    {
        input_level_jobs(cri);
        sort_level_jobs(cri);

        printf("Level-%d Jobs\n", cri);
        print_level_jobs(cri);
        printf("\n\n");
    }
}

void input_level_jobs(int criticality)
{
    fscanf(file_input[criticality], "%d", &num_jobs[criticality]);

    jobs[criticality] = (Job *) malloc(sizeof(Job) * num_jobs[criticality]);

    for (int i = 0; i < num_jobs[criticality]; i++)
    {
        fscanf(file_input[criticality], "%f %f", &jobs[criticality][i].arrival_time, &jobs[criticality][i].absolute_deadline);
        for (int j = 0; j < 4 - criticality; j++)
        {
            fscanf(file_input[criticality], "%f", &jobs[criticality][i].wcet[j]);
        }

        jobs[criticality][i].criticality_level = criticality;
        jobs[criticality][i].time_executed = 0;
        jobs[criticality][i].admitted = false;
        jobs[criticality][i].alive = true;
    }
}


void print_level_jobs(int criticality)
{
    printf("Number of jobs: %d\n", num_jobs[criticality]);
    for (int i = 0; i < num_jobs[criticality]; i++)
    {
        printf("J%d arrival: %0.2f, A.e: %0.2f, B.e: %0.2f, C.e: %0.2f, D.e: %0.2f, d: %0.2f\n", i+1, jobs[criticality][i].arrival_time, jobs[criticality][i].wcet[3], jobs[criticality][i].wcet[2], jobs[criticality][i].wcet[1], jobs[criticality][i].wcet[0], jobs[criticality][i].absolute_deadline);
    }
}


int sort_jobs_comparator(const void *a, const void *b)
{
    Job job_a, job_b;
    job_a = *((Job *) a);
    job_b = *((Job *) b);

    return job_a.absolute_deadline - job_b.absolute_deadline;
}


void sort_level_jobs(int criticality)
{
    qsort((Job *) jobs[criticality], num_jobs[criticality], sizeof(jobs[criticality][0]), sort_jobs_comparator);
}