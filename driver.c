#include <stdio.h>

#include "utility.h"
#include "container.h"

FILE* file_input[0];
Container containers[4];

int num_jobs[4];

int main()
{
    // Take input.
    open_files();
    init_containers();

    // Run the scheduler.

    // Print the necessary output.

    close_files();

    return 0;
}