#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#include "priority_queue.h"
#include "scheduler.h"
#include "cpu.h"
#include "configuration.h"

extern CPU cpu;
extern FILE* file_output;

int curr_criticality;
float curr_time;
float next_decision_point_time;

/*
 * Utility functions extended to float values.
 */
float min(float a, float b) { return a < b ? a : b; }
float max(float a, float b) { return a > b ? a : b; }


/*
 * Initialises the variables required for scheduling and starts the scheduler.
 */
void init_scheduler()
{
    // Setting the seed before random numbers are generated.
    srand(time(NULL));

    curr_criticality = 3;
    curr_time = 0;

    // Initialise the ready queues.
    init_queue(&cpu.global_ready_queue);
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        init_queue(&cpu.processors[i].ready_queue);
    }

    fprintf(file_output, "\n------------------------------------------------------------------------\n");
    fprintf(file_output, "Scheduler is starting...\n");
    fprintf(file_output, "Starting the system with criticality level-3.\n");
    fflush(file_output);

    // Run the scheduler.
    scheduler();

    fprintf(file_output, "\n\n------------------------------------------------------------------------\n");
    fprintf(file_output, "All jobs have been completed. Exiting scheduler.\n\n");
}


/*
 * Prints the ready queues in each processor and also the global processor.
 */
void print_ready_queues()
{
    fprintf(file_output, "\nPrinting the ready queues.\n");
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        fprintf(file_output, "Processor-%d\n", i);
        fprintf(file_output, "Num_jobs=%d & empty=%d\n", cpu.processors[i].ready_queue.num_elements, cpu.processors[i].ready_queue.empty);
        for (int j = 0; j < cpu.processors[i].ready_queue.num_elements; j++)
        {
            Job job = cpu.processors[i].ready_queue.jobs[j];
            fprintf(file_output, "J%d, aet=%0.2f, time_left=%0.2f\n", job.job_num, job.aet, job.time_left);
        }
    }
    fprintf(file_output, "Global Queue.\n");
    for (int i = 0; i < cpu.global_ready_queue.num_elements; i++)
    {
        Job job = cpu.global_ready_queue.jobs[i];
        fprintf(file_output, "J%d, aet=%0.2f, time_left=%0.2f\n", job.job_num, job.aet, job.time_left);
    }
    fprintf(file_output, "Done printing the jobs.\n\n");
}


/*
 * Returns a number between (min_percent * WCET) and (1 * WCET) as AET.
 */
float get_actual_runtime(float wcet)
{
    float aet = rand() % (100 - MIN_PERCENT_EXECUTION);
    aet = (aet + MIN_PERCENT_EXECUTION) / 100;
    aet = aet * wcet;

    return aet;
}


/*
 * Returns whether a particular job will miss its deadline or not.
 */
int get_deadline_miss()
{
    float chance = rand() % 100;
    if (chance / 100 < CHANCE_MISS_DEADLINE)
        return 1;
    return 0;
}


/*
 * Returns whether all the ready queues of the CPU are empty or not.
 */
bool check_ready_queues_empty()
{
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        if (!cpu.processors[i].ready_queue.empty)
        {
            return false;
        }
    }

    return cpu.global_ready_queue.empty;
}

/*
 * Remove all the lower criticality tasks from the ready queue.
 */
void reflect_increasing_criticality_in_ready_queues()
{
    // Reflecting only in Level-A and Level-B jobs as the rest are in another queue, and we need not use that queue all together if criticality level is high.
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        PQueue temp;
        init_queue(&temp);

        // Filter the jobs meeting the criticality requirements.
        while (cpu.processors[i].ready_queue.num_elements > 0)
        {
            Job job = extract_min(&cpu.processors[i].ready_queue);
            if (job.actual_criticality_level <= curr_criticality) // If the job meets all the requirements, then add it to the temp queue.
            {
                // Update the wcet and the aet.
                float wcet = job.wcet[3 - curr_criticality];
                float aet = get_actual_runtime(wcet);
                if (aet - job.time_left > 0)
                {
                    job.aet = aet;
                    job.time_left = aet - job.time_executed;
                }

                insert_job(&temp, job);
            }
            else
            {
                fprintf(file_output, "Job J%d on Processor-%d has been dropped due to criticality change.\n", job.job_num, i);
            }
            
        }

        // Add the filtered jobs back to the processor queue.
        while (temp.num_elements > 0)
        {
            Job job = extract_min(&temp);
            insert_job(&cpu.processors[i].ready_queue, job);
        }
    }
}


/*
 * Helper function that finds the criticality level of a task and then the aet of the job and finally adds it to the run queue.
 */
void add_job_to_ready_queue_helper(PQueue *q, Job *job, int processor)
{
    // No need to add the job when the curr criticality level of the system is greater than the max criticality level.
    if (job->actual_criticality_level > curr_criticality)
        return;

    // Find the criticality level it will run in.
    int job_criticality_level = min(curr_criticality, job->curr_criticality_level);

    while (job_criticality_level != job->actual_criticality_level)
    {
        if (get_deadline_miss() == 1)
        {
            job_criticality_level--;
        }
        else break;
    }

    // Need to change the criticality level of the system if a job is going to miss its deadline.
    if (job_criticality_level < curr_criticality)
    {
        if (processor != -1)
            fprintf(file_output, "Job J%d on Processor-%d has increased the criticality level from %d to %d.\n", job->job_num, processor, curr_criticality, job_criticality_level);
        else
            fprintf(file_output, "Job J%d of Level-C or D has increased the criticality level from %d to %d.\n", job->job_num, curr_criticality, job_criticality_level);

        fflush(file_output);
        

        curr_criticality = job_criticality_level;
        reflect_increasing_criticality_in_ready_queues();
    }

    job->curr_criticality_level = job_criticality_level;
    int index = 3 - job_criticality_level; // Indices of wcet are stored in increasing criticality levels (D to A).
    float wcet = job->wcet[index];

    // Find the aet using the wcet for that criticality level.
    float aet = get_actual_runtime(wcet);
    job->aet = aet;
    job->time_left = aet;

    if (processor != -1)
    {
        fprintf(file_output, "Processor-%d, Job-%d, criticality-%d, wcet=%0.2f, aet=%0.2f\n", processor, job->job_num, job_criticality_level, wcet, aet);
    }
    else
    {
        fprintf(file_output, "Global Job-%d criticality-%d, wcet=%0.2f, aet=%0.2f\n", job->job_num, job_criticality_level, wcet, aet);
    }
    
    // Add to the ready queue.
    insert_job(q, *job);
}


/*
 * Adds job to the corresponding ready queue in the corresponding processor.
 */
void add_jobs_to_ready_queue()
{
    // For jobs of criticality A and B (which have a processor level queue).
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        while (cpu.processors[i].container_A.jobs_left && curr_time >= cpu.processors[i].container_A.jobs[cpu.processors[i].container_A.curr_job_index].arrival_time)
        {
            add_job_to_ready_queue_helper(&cpu.processors[i].ready_queue, &cpu.processors[i].container_A.jobs[cpu.processors[i].container_A.curr_job_index], i);
            
            cpu.processors[i].container_A.curr_job_index++;

            if (cpu.processors[i].container_A.curr_job_index >= cpu.processors[i].container_A.num_jobs)
            {
                cpu.processors[i].container_A.jobs_left = false;
                fprintf(file_output, "Processor-%d Criticality-A is over.\n", i);
            }
        }

        while (cpu.processors[i].container_B.jobs_left && curr_time >= cpu.processors[i].container_B.jobs[cpu.processors[i].container_B.curr_job_index].arrival_time)
        {
            add_job_to_ready_queue_helper(&cpu.processors[i].ready_queue, &cpu.processors[i].container_B.jobs[cpu.processors[i].container_B.curr_job_index], i);
            
            cpu.processors[i].container_B.curr_job_index++;

            if (cpu.processors[i].container_B.curr_job_index >= cpu.processors[i].container_B.num_jobs)
            {
                cpu.processors[i].container_B.jobs_left = false;
                fprintf(file_output, "Processor-%d Criticality-B is over.\n", i);
            }
        }
    }

    // For global ready queus of level C and D.
    while (cpu.container_C.jobs_left && curr_time >= cpu.container_C.jobs[cpu.container_C.curr_job_index].arrival_time)
    {
        add_job_to_ready_queue_helper(&cpu.global_ready_queue, &cpu.container_C.jobs[cpu.container_C.curr_job_index], -1);
        cpu.container_C.curr_job_index++;

        if (cpu.container_C.curr_job_index >= cpu.container_C.num_jobs)
            cpu.container_C.jobs_left = false;
    }

    while (cpu.container_D.jobs_left && curr_time >= cpu.container_D.jobs[cpu.container_D.curr_job_index].arrival_time)
    {
        add_job_to_ready_queue_helper(&cpu.global_ready_queue, &cpu.container_D.jobs[cpu.container_D.curr_job_index], -1);
        cpu.container_D.curr_job_index++;

        if (cpu.container_D.curr_job_index >= cpu.container_C.num_jobs)
            cpu.container_D.jobs_left = false;
    }
}


/*
 * Finds and returns the time at which the next earliest job arrives.
 */
float get_earliest_arrival_time()
{
    float min_earliest_arrival_time = FLT_MAX;
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        if (cpu.processors[i].container_A.jobs_left)
            min_earliest_arrival_time = min(min_earliest_arrival_time, cpu.processors[i].container_A.jobs[cpu.processors[i].container_A.curr_job_index].arrival_time);
        if (cpu.processors[i].container_B.jobs_left)
            min_earliest_arrival_time = min(min_earliest_arrival_time, cpu.processors[i].container_B.jobs[cpu.processors[i].container_B.curr_job_index].arrival_time);
    }
    if (cpu.container_C.jobs_left)
        min_earliest_arrival_time = min(min_earliest_arrival_time, cpu.container_C.jobs[cpu.container_C.curr_job_index].arrival_time);
    if (cpu.container_D.jobs_left)
        min_earliest_arrival_time = min(min_earliest_arrival_time, cpu.container_D.jobs[cpu.container_D.curr_job_index].arrival_time);

    return min_earliest_arrival_time;
}


/*
 * If all the jobs of that criticality are done, we can reduce the criticality of the system.
 */
void check_and_lower_criticality()
{
    if (curr_criticality == 3)
        return; // As the system is in the lower possible criticality.
    
    float max_criticality = 3;
    // Finding the max criticality of the jobs in each processor.
    // Using the first/top job in every ready queue is enough as the ready queue is sorted descending criticality level. 
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        if (cpu.processors[i].ready_queue.num_elements > 0)
        {
            Job top_job = extract_min(&cpu.processors[i].ready_queue);
            max_criticality = max(max_criticality, top_job.curr_criticality_level);
            insert_job(&cpu.processors[i].ready_queue, top_job);
        }
    }

    if (cpu.global_ready_queue.num_elements > 0)
    {
        Job top_job = extract_min(&cpu.global_ready_queue);
        max_criticality = max(max_criticality, top_job.curr_criticality_level);
        insert_job(&cpu.global_ready_queue, top_job);
    }

    if (max_criticality > curr_criticality)
    {
        fprintf(file_output, "Reducing the system's criticality level from %d to %0.0f at t=%f\n", curr_criticality, max_criticality, curr_time);
        curr_criticality = max_criticality;
    }
}


/*
 * Finds the next decision point based on Anderson's model and returns it.
 */
float get_next_decision_point()
{
    // The next decision point can either be a time when a job finishes, or a new job arrives, or both.
    // So we can find the earliest time a new job arrives as well as the earliest time a job finishes and return its min value.

    // Earliest time a new job arrives.
    float earliest_time_new_job_arrives = get_earliest_arrival_time();

    // Earliest time a job finishes.
    float earliest_time_job_finishes = FLT_MAX;
    bool all_processors_queues_empty = true;

    // fprintf(file_output, "Inside get_next_deadline.\n");
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        // fprintf(file_output, "%d is empty=%d and num_jobs=%d\n", i, cpu.processors[i].ready_queue.empty, cpu.processors[i].ready_queue.num_elements);
        fflush(file_output);
        if (cpu.processors[i].ready_queue.num_elements > 0)
        {
            all_processors_queues_empty = false;

            // First job of every processor local queue is enough as it is sorted based on criticality.
            Job top_job = extract_min(&cpu.processors[i].ready_queue);
            insert_job(&cpu.processors[i].ready_queue, top_job);
            earliest_time_job_finishes = min(earliest_time_job_finishes, top_job.time_left + curr_time);
            // fprintf(file_output, "Processor-%d will have Job-%d from %0.2f to %0.2f\n", i, top_job.job_num, curr_time, curr_time+top_job.time_left);
        }
    }

    if (all_processors_queues_empty) // Only if all processor local queues are empty will we be taking a job from the global queue.
    {
        if (!cpu.global_ready_queue.empty)
        {
            Job top_job = extract_min(&cpu.global_ready_queue);
            insert_job(&cpu.global_ready_queue, top_job);
            earliest_time_job_finishes = min(earliest_time_job_finishes, top_job.time_left + curr_time);
        }
    }

    return min(earliest_time_new_job_arrives, earliest_time_job_finishes);
}


/*
 * Runs a job in each processor till the next decision point.
 */
void run_jobs()
{
    Job running_jobs[NUM_PROCESSORS]; // Since we will be iterating over all the processors one at a time, we should not add the job back to the queues as soon as the job for that processor is done (since they should technically run in parallel).
    bool processor_local_job[NUM_PROCESSORS]; // To find which queue the job came from (processor local or global).

    // Get the next decision point.
    float next_decision_point = get_next_decision_point();

    fprintf(file_output, "At curr_time=%0.2f, starting to run job till next_decision_point=%0.2f.\n", curr_time, next_decision_point);

    // Running the job for each processor, one at a time.
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        if (cpu.processors[i].ready_queue.num_elements > 0) // If there are level-A or level-B jobs (local to processor)
        {
            Job job = extract_min(&cpu.processors[i].ready_queue);   

            running_jobs[i] = job;
            processor_local_job[i] = true;
        }
        else if (cpu.global_ready_queue.num_elements > 0) // If there are any level-C or level-D jobs.
        {
            Job job = extract_min(&cpu.global_ready_queue);

            running_jobs[i] = job;
            processor_local_job[i] = false;
        }
        else // If there are no jobs in the queues.
        {
            running_jobs[i].aet = -1;
        }
    }

    // Adding the job back into the ready queue after running the processors.
    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        Job job = running_jobs[i];

        if (job.aet == -1) // If the processor was idle.
        {
            fprintf(file_output, "Processor-%d was idle from %0.2f to %0.2f.\n", i, curr_time, next_decision_point);
        }
        else
        {
            fprintf(file_output, "Job J%d of criticality-%d executed on Processor-%d from %0.2f to %0.2f.\n", job.job_num, job.actual_criticality_level, i, curr_time, next_decision_point);

            job.time_left = job.time_left - (next_decision_point - curr_time);
            job.time_executed += (next_decision_point - curr_time);

            if (job.time_left <= ZERO_AET_TOLERANCE) // If the job in that processor has completed.
            {
                job.finish_time = next_decision_point; // Curr time is not updated to next_decision_point yet.
                job.alive = false;

                fprintf(file_output, "Job J%d of criticality-%d has FINISHED running on Processor-%d.\n", job.job_num, job.actual_criticality_level, i);
            }
            else // We can add the job back to the corresponding ready queue if it is still alive.
            {
                if (processor_local_job[i]) // Processor local.
                {
                    insert_job(&cpu.processors[i].ready_queue, job);
                }
                else // Global ready queue.
                {
                    insert_job(&cpu.global_ready_queue, job);
                }
            }
        }
    }

    // If all the tasks of that criticality level are done for now, then we can reduce the criticality of the system.
    check_and_lower_criticality();

    // Update the curr_time after executing jobs.
    curr_time = next_decision_point;
}


/*
 * The actual scheduler that uses the various helper functions to simulate the working of the scheduler.
 */
void scheduler()
{
    while (1) // To run the scheduler continuously.
    {
        // Find if any job is currently present.
        bool queues_empty = check_ready_queues_empty();

        // Idle if no jobs are present.
        if (queues_empty)
        {
            fprintf(file_output, "\nAll the ready queues are empty.\n");
            fflush(file_output);
            float min_earliest_arrival_time = get_earliest_arrival_time();

            // If all the jobs are completed, the control can break out of the infinite loop.
            if (min_earliest_arrival_time == FLT_MAX)
            {
                break;
            }

            // Stay idle till next job arrives and move time till it arrives.
            fprintf(file_output, "\nProcessor IDLE from %0.2f to %0.2f.\n", curr_time, min_earliest_arrival_time);

            curr_time = min_earliest_arrival_time;
            fprintf(file_output, "curr_time is now %0.2f\n", curr_time);
        }

        // Add all the jobs that enter the system now.
        add_jobs_to_ready_queue();

        // Prints the ready queues for each processor and the global ready queue.
        print_ready_queues();

        // Run one job in each processor till the next decision point.
        run_jobs();
    }
}
