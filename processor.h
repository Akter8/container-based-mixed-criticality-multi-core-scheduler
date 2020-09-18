#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "container.h"
#include "priority_queue.h"

typedef struct
{
    Container container_A;
    Container container_B;
    float dynamic_slack;
    PQueue ready_queue;
}
Processor;

void init_processor(Processor *);

#endif