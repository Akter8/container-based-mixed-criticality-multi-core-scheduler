#ifndef CPU_H
#define CPU_H

#include "processor.h"

typedef struct
{
    Processor *processors;
    Container container_C;
    Container container_D;
    PQueue global_ready_queue;
}
CPU;

void init_cpu();

#endif