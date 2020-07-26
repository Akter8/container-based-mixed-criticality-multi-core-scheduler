typedef struct
{
    // Timing characteristics of the job.
    long arrival_time;
    float wcet; // Worst-case execution time.
    float aet; // Actual execution time.
    float time_executed; // Total amount of time the job has executed for so far.
    float time_left; // Time left in overall execution. Will be 0 when job has finished executing.
    long absolute_deadline;
    float finish_time;

    // To find whether a job has completed or has already been accepted.
    bool alive;
    bool admitted;
}
Job;
