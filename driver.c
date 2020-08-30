#include <stdio.h>

#include "utility.h"
#include "cpu.h"

FILE* file_input[0];
FILE* file_output;
CPU cpu;

int main()
{
    // Take input.
    open_files();
    init_cpu();

    // Run the scheduler.

    // Print the necessary output.

    close_files();

    return 0;
}