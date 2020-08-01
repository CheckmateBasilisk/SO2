#include<stdio.h>
#include<malloc.h>
#include<pthread.h>
#include<semaphore.h>

//linked list for the job queue
struct job{
    struct job *next;
    //content for the job to be executed
}

struct job *job_queue;

// semaphore to deal with the job-getting operations
sem_t job_queue_sem;

// innitializes the job queue appropriatelly
void innitialize_job_queue(struct job *job_queue, sem_ t semaphore){
    *job_queue = NULL;
    sem_init(semaphore, 0, 0)
    //int sem_init(sem_t *sem, int pshared, unsigned int value);
    //pshared = 0 => semaphore is shared between threads of the same process
    //pshared != 0 => semaphore is shared between processess. It also should be located in shared memory! (shmget and stuff...)

    return;
}

extern void process_job(struct job*);

pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

// executes the jobs until the queue is empty
void *thread_function(void *arg){
    while(1){
        struct job *current_job;

        //locks mutex
        pthread_mutex_lock(&job_queue_mutex);
        if(job_queue == NULL){
            pthread_mutex_unlock(&job_queue_mutex);
            break;
        } else {
            //takes the first job out of the queue
            next_job = job_queue;
            job_queue = job_queue->next;
            //unlocks the mutex
            pthread_mutex_unlock(&job_queue_mutex);

            //processes the job
            process_job(current_job);
            free(current_job);
        }
    }

    return NULL;
}

// this code avoids the race condition
// grabbing the current_job and taking it out of the queue is an atomic operation that can't be interrupted by another thread;

// Locking mutexes is a dangerous operation. Locking an alredy locked mutex causes a Deadlock, preventing the thread to unlock itself, getting everything stuck in place
// to avoid locking locked mutexes we should use pthread_mutex_trylock! :D
// better yet, use Semaphores!
