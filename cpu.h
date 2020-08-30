#include "processor.h"

typedef struct
{
    Processor *processors;
    Container container_C;
    Container container_D;
}
CPU;

void init_cpu();