#include <stdlib.h>
#include <stdio.h>

#include "job.h"

extern FILE* file_input[4];
extern FILE* file_output;

/*
 * Allocates memory to store the jobs datastructure dynamically.
 */
void create_level_jobs(int num, Job **jobs)
{    
    *jobs = (Job *) malloc(sizeof(Job) * num);
}


/*
 * Inputs the tasks for a single level for a single processor.
 */
void input_level_jobs(int criticality, int num, Job *jobs)
{
    for (int i = 0; i < num; i++)
    {
        fscanf(file_input[criticality], "%f %f", &jobs[i].arrival_time, &jobs[i].absolute_deadline);
        for (int j = 0; j < 4 - criticality; j++)
        {
            fscanf(file_input[criticality], "%f", &jobs[i].wcet[j]);
        }

        jobs[i].actual_criticality_level = criticality;
        jobs[i].curr_criticality_level = 3;
        jobs[i].job_num = i;
        jobs[i].time_executed = 0;
        jobs[i].admitted = false;
        jobs[i].alive = true;
    }
}


/*
 * Prints the jobs and the various times associated with them for a single level of criticality.
 */
void print_level_jobs(int criticality, int num, Job* jobs)
{
    fprintf(file_output, "Number of jobs: %d\n", num);
    for (int i = 0; i < num; i++)
    {
        fprintf(file_output, "J%d arrival: %0.2f, Abs-d: %0.2f, A.e: %0.2f, B.e: %0.2f, C.e: %0.2f, D.e: %0.2f,\n", i+1, jobs[i].arrival_time, jobs[i].absolute_deadline, jobs[i].wcet[3], jobs[i].wcet[2], jobs[i].wcet[1], jobs[i].wcet[0]);
    }
    fprintf(file_output, "\n");
}

/*
 * Comparator used by qsort to sor the jobs. The comparision is only based on arrival time.
 */
int sort_jobs_comparator(const void *a, const void *b)
{
    Job job_a, job_b;
    job_a = *((Job *) a);
    job_b = *((Job *) b);

    return job_a.arrival_time - job_b.arrival_time;
}


/*
 * Sorts the jobs in a level based on the comparator.
 */
void sort_level_jobs(int num, Job* jobs)
{
    qsort((Job *) jobs, num, sizeof(jobs[0]), sort_jobs_comparator);
}