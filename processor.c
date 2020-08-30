#include "processor.h"

void init_processor(Processor *processor)
{
    init_container(0, &processor->container_A);
    init_container(1, &processor->container_B);
}