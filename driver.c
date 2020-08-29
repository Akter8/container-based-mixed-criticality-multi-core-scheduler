#include <stdio.h>

#include "utility.h"
#include "job.h"

FILE* file_input[0];
Job* jobs[4];

int num_jobs[4];

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