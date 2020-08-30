#include <stdlib.h>
#include <stdio.h>

#include "job.h"

extern FILE* file_input[4];
extern int num_jobs[4];


void create_level_jobs(int criticality, Job **jobs)
{    
    fscanf(file_input[criticality], "%d", &num_jobs[criticality]);
    *jobs = (Job *) malloc(sizeof(Job) * num_jobs[criticality]);
}


void input_level_jobs(int criticality, Job *jobs)
{
    for (int i = 0; i < num_jobs[criticality]; i++)
    {
        fscanf(file_input[criticality], "%f %f", &jobs[i].arrival_time, &jobs[i].absolute_deadline);
        for (int j = 0; j < 4 - criticality; j++)
        {
            fscanf(file_input[criticality], "%f", &jobs[i].wcet[j]);
        }

        jobs[i].criticality_level = criticality;
        jobs[i].time_executed = 0;
        jobs[i].admitted = false;
        jobs[i].alive = true;
    }
}


void print_level_jobs(int criticality, Job* jobs)
{
    printf("Number of jobs: %d\n", num_jobs[criticality]);
    for (int i = 0; i < num_jobs[criticality]; i++)
    {
        printf("J%d arrival: %0.2f, A.e: %0.2f, B.e: %0.2f, C.e: %0.2f, D.e: %0.2f, d: %0.2f\n", i+1, jobs[i].arrival_time, jobs[i].wcet[3], jobs[i].wcet[2], jobs[i].wcet[1], jobs[i].wcet[0], jobs[i].absolute_deadline);
    }
}


int sort_jobs_comparator(const void *a, const void *b)
{
    Job job_a, job_b;
    job_a = *((Job *) a);
    job_b = *((Job *) b);

    return job_a.absolute_deadline - job_b.absolute_deadline;
}


void sort_level_jobs(int criticality, Job* jobs)
{
    qsort((Job *) jobs, num_jobs[criticality], sizeof(jobs[0]), sort_jobs_comparator);
}