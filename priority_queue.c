#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "priority_queue.h"

extern FILE* file_output;


int parent(int i) { return (i-1)/2; } 


// to get index of left child of node at index i.
int left(int i) { return (2*i + 1); }


// to get index of right child of node at index i.
int right(int i) { return (2*i + 2); }


void init_queue(PQueue *q)
{
    q->empty = true;
    q->jobs = NULL;
    q->num_elements = 0;
}


// Inserts a new key 'k' 
void insert_job(PQueue *q, Job job) 
{  
    // First insert the new job at the end.
    if (q->empty)
    {
        q->empty = false;
    }
    q->num_elements++;
    q->jobs = (Job *) realloc(q->jobs, sizeof(Job) * q->num_elements);
    // heap_size++;
    int i = q->num_elements - 1; 
    q->jobs[i] = job;

    // Fix the min heap property if it is violated.
    while (i != 0 && compare_jobs(q->jobs[parent(i)], q->jobs[i]) == -1) 
    { 
       swap(&q->jobs[i], &q->jobs[parent(i)]);
       i = parent(i);
    } 
}


// Method to peek min element from min heap.
void peek_min(PQueue *q, Job* min_job)
{
    min_job = &q->jobs[0];
}

  
// Method to remove minimum element (or root) from min heap.
Job extract_min(PQueue *q) 
{
    if (q->num_elements == 1)
    {
        q->num_elements--;
        q->empty = true;
        Job root = q->jobs[0];
        q->jobs = NULL;

        return root;
    }

    if (q->jobs == NULL)
    {
        fprintf(file_output, "Inside extract_min in priority_queue.c Error: Queue is empty.\n");
        fflush(file_output);
    }
  
    // Store the minimum value, and remove it from heap.
    Job root = q->jobs[0];

    q->jobs[0] = q->jobs[q->num_elements-1]; 
    q->num_elements--;
    q->jobs = (Job *) realloc(q->jobs, sizeof(Job) * q->num_elements);

    min_heapify(q, 0);

    return root;
} 


// A recursive method to heapify a subtree with the root at given index.
// This method assumes that the subtrees are already heapified.
void min_heapify(PQueue *q, int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    
    if (l < q->num_elements && compare_jobs(q->jobs[l], q->jobs[i]) == -1)
        smallest = l;
    if (r < q->num_elements && compare_jobs(q->jobs[r], q->jobs[smallest]) == -1)
        smallest = r;
    
    if (smallest != i)
    {
        swap(&q->jobs[i], &q->jobs[smallest]);
        min_heapify(q, smallest);
    }
}


// A utility function to swap two jobs.
void swap(Job *x, Job *y) 
{ 
    Job temp = *x; 
    *x = *y; 
    *y = temp; 
}


// Compares two jobs and returns a corresponding int value.
// Returns 1 when a is higher criticaliy (if same criticality, then higher priority) than b.
// Else returns -1.
int compare_jobs(Job a, Job b)
{
    // First compared based on criticality level.
    if (a.actual_criticality_level != b.actual_criticality_level)
    {
        return (a.actual_criticality_level < b.actual_criticality_level) ? 1 : -1;
    }
    else if (a.absolute_deadline != b.absolute_deadline)// If criticality is the same, then based on deadline.
    {
        return (a.absolute_deadline < b.absolute_deadline) ? 1 : -1;
    }
    else // This is required as when we are removing just the first job from the ready queue and putting it back, we might not get the same job back on top if there exists another job that has the same criticality level and same absolute deadline.
    {
        // This part of the code will only be reached once the aet of the job has been calculated (so there are no errors due to aet not being calculated and this code being executed).
        return (a.aet < b.aet) ? 1 : -1;
    }
}