# Container-based Mixed-Criticality Real-time Scheduling for Multi-core Systems

Simulates a 4-level container based MCS+RTS scheduler for a multi-core platform.

## Description of files

* configuration.h - Contains the configuration settings of the program.
* job.h - Contains the ADT of Job.
* job.c - Contains the function definitions of a job.
* container.h - Contains the ADT of a container.
* container.c - Contains the functions of a container.
* processor.h - Containers the ADT of the processor with two non-global containers.
* processor.c - Containers the functions of a processor.
* cpu.h - Contains the ADT of cpu with multiple processors and two global-containers.
* cpu.c - Contains the functions of the cpu.
* driver.c - Contains the driver code of the program.

## Description of program structure

* Every container has its own set of jobs that are encapsulated in the container ADT.
* Every processor has two container (level-A and level-B) which are local to the particular processor.
* The cpu has multiple processor (each containing its own level-A and level-B containers) and two global containers (level-C and level-D).

## Assumptions and Limitations