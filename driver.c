#include <stdio.h>

#include "utility.h"
#include "job.h"

FILE* file_input_level_A;
Job* jobs_A;

int num_jobs_A;
int num_jobs_B;
int num_jobs_C;
int num_jobs_D;

int main()
{
    // Take input.
    open_files();
    input_jobs();

    // Run the scheduler.

    // Print the necessary output.

    close_files();

    return 0;
}