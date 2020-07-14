#include<stdio.h>
#include<malloc.h>
#include<pthread.h>

//linked list for the job queue
struct job{
    struct job *next;
    //content for the job to be executed
}

struct job *job_queue;

extern void process_job(struct job*);

pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

// executes the jobs until the queue is empty
void *thread_function(void *arg){
    while(1){
        struct job *current_job;

        //locks mutex
        pthread_mutex_lock(&job_queue_mutex);
        if(job_queue == NULL){
            break;
        } else {
            next_job = job_queue;
        }
    }

    return NULL;
}

// this code causes a race condition
// between grabbing the current_job and taking it out of the queue another thread might run, ruining the whole thing

// the solution is using a MUTEX = Mutual Exclusion Lock
// lock mutex -> execute code that should be atomic -> unlock mutex
