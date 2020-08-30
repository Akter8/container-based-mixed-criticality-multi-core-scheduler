#include <stdlib.h>
#include <stdio.h>

#include "job.h"

extern FILE* file_input[4];
extern FILE* file_output;


void create_level_jobs(int num, Job **jobs)
{    
    *jobs = (Job *) malloc(sizeof(Job) * num);
}


void input_level_jobs(int criticality, int num, Job *jobs)
{
    for (int i = 0; i < num; i++)
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


void print_level_jobs(int criticality, int num, Job* jobs)
{
    fprintf(file_output, "Number of jobs: %d\n", num);
    for (int i = 0; i < num; i++)
    {
        fprintf(file_output, "J%d arrival: %0.2f, Abs-d: %0.2f, A.e: %0.2f, B.e: %0.2f, C.e: %0.2f, D.e: %0.2f,\n", i+1, jobs[i].arrival_time, jobs[i].absolute_deadline, jobs[i].wcet[3], jobs[i].wcet[2], jobs[i].wcet[1], jobs[i].wcet[0]);
    }
    fprintf(file_output, "\n");
}


int sort_jobs_comparator(const void *a, const void *b)
{
    Job job_a, job_b;
    job_a = *((Job *) a);
    job_b = *((Job *) b);

    return job_a.absolute_deadline - job_b.absolute_deadline;
}


void sort_level_jobs(int num, Job* jobs)
{
    qsort((Job *) jobs, num, sizeof(jobs[0]), sort_jobs_comparator);
}