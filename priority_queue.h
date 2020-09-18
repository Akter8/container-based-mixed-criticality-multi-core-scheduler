#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "job.h"

typedef struct
{
    int num_elements;
    bool empty;
    Job *jobs;
}
PQueue;


void init_queue(PQueue *);
int parent(int);
int left(int);
int right(int);
void insert_job(PQueue *, Job);
void peek_min(PQueue *, Job*);
Job extract_min(PQueue *);
void min_heapify(PQueue *, int);
void swap(Job *, Job *);
int compare_jobs(Job, Job);


#endif