#include<stdio.h>


//linked list for the job queue
struct job{
    struct job *next;
    //content for the job to be executed
}

struct job *job_queue;

extern void process_job(struct job*);

// executes the jobs until the queue is empty
void *thread_function(void *arg){
    while (job_queue != NULL){
        //gets next job
        struct job* current_job = job_queue;
        //removes job from queue
        job_queue = job_queue->next;
        //does the thing
        process_job(current_job);
        //clears stuff
        free(current_job);
    }
    return NULL;
}

// this code causes a race condition
// between grabbing the current_job and taking it out of the queue another thread might run, ruining the whole thing

// the solution is using a MUTEX = Mutual Exclusion Lock
// lock mutex -> execute code that should be atomic -> unlock mutex
