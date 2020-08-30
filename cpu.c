#include <stdlib.h>
#include <stdio.h>

#include "cpu.h"
#include "configuration.h"

extern FILE* file_output;

extern CPU cpu;

void init_cpu()
{
    cpu.processors = (Processor *) malloc(sizeof(Processor) * NUM_PROCESSORS);

    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        fprintf(file_output, "Processor-%d\n", i+1);
        init_processor(&cpu.processors[i]);
        fprintf(file_output, "\n\n");
    }

    // Global containers.
    fprintf(file_output, "Printing global containers C and D\n\n");
    init_container(2, &cpu.container_C);
    init_container(3, &cpu.container_D);
}