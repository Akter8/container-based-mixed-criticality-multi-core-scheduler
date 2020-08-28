#include <stdlib.h>
#include <stdio.h>

#include "job.h"

extern FILE* file_input_level_A;
extern Job* jobs_A;

extern int num_jobs_A;
extern int num_jobs_B;
extern int num_jobs_C;
extern int num_jobs_D;

void input_jobs()
{
    input_jobs_A();

    print_jobs(jobs_A, num_jobs_A);
}

void input_jobs_A()
{
    fscanf(file_input_level_A, "%d", &num_jobs_A);

    jobs_A = (Job *) malloc(sizeof(Job) * num_jobs_A);

    for (int i = 0; i < num_jobs_A; i++)
    {
        fscanf(file_input_level_A, "%f", &jobs_A[i].arrival_time);
        for (int j = 0; j < 4; j++)
        {
            fscanf(file_input_level_A, "%f", &jobs_A[i].wcet[j]);
        }

        jobs_A[i].criticality_level = 0;
    }
}

void print_jobs(Job* jobs, int num_jobs)
{
    printf("Number of jobs: %d\n", num_jobs);
    for (int i = 0; i < num_jobs; i++)
    {
        printf("J%d arrival: %0.2f, A.e: %0.2f, B.e: %0.2f, C.e: %0.2f, D.e: %0.2f\n", i+1, jobs[i].arrival_time, jobs[i].wcet[3], jobs[i].wcet[2], jobs[i].wcet[1], jobs[i].wcet[0]);
    }
}