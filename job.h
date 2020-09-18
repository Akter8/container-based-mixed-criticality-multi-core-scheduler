#ifndef JOB_H
#define JOB_H

#include <stdbool.h>

typedef struct
{
    int actual_criticality_level;
    int curr_criticality_level;

    int job_num;

    // Timing characteristics of the job.
    float arrival_time;
    float wcet[4]; // Worst-case execution time for each criticality level. (0 being criticality_D and 3 being criticality_A)
    float aet; // Actual execution time.
    float time_executed; // Total amount of time the job has executed for so far.
    float time_left; // Time left in overall execution. Will be 0 when job has finished executing.
    float absolute_deadline;
    float finish_time;

    // To find whether a job has completed and/or has already been accepted.
    bool alive;
    bool admitted;
}
Job;

// Functions
void create_level_jobs(int, Job**);
void input_level_jobs(int, int, Job*);
void sort_level_jobs(int, Job*);
void print_level_jobs(int, int, Job*);

#endif