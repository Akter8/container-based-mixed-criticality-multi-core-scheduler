#include <stdio.h>

#include "processor.h"

extern FILE* file_output;

/*
 * Initialises each processor and its corresponding local contaienrs.
 */
void init_processor(Processor *processor)
{
    fprintf(file_output, "Level-A tasks.\n");
    init_container(0, &processor->container_A);

    fprintf(file_output, "Level-B tasks.\n");
    init_container(1, &processor->container_B);
    
    processor->dynamic_slack = 0;
}