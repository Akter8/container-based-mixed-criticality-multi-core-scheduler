#include <stdio.h>

#include "utility.h"
#include "cpu.h"
#include "scheduler.h"

FILE* file_input[4];
FILE* file_output;

CPU cpu;

int main()
{
    // Inputs the required data.
    open_files();
    init_cpu();

    // Run the scheduler.
    init_scheduler();

    // Print the necessary output.

    // Prepare to finish the program.
    close_files();

    return 0;
}