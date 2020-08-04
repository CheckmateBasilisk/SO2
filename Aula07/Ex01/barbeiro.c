#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<pthread.h>
#include<semaphore.h>

enum{
    PROGNAME,
    NRBARBEIROS,
    NRCADEIRAS,
    TCORTE,
    TCHEGADA,
    NARGS
};

typedef enum{
    FALSE,
    TRUE
} BOOLEAN;

// each job is a client with its service length
typedef struct JOB{
    int service_length;
    int service_id;
    struct JOB *next;
}JOB;

typedef struct JOB_LIST{
    JOB *head;
    int max_length;
    int length; //current list length
} JOB_LIST;

typedef struct BARBER_ARGS{
    int n;
    JOB_LIST *job_list;
}BARBER_ARGS;



JOB *create_job(int , int );
void free_job(JOB *);
JOB *pop_job(JOB_LIST *);
int append_job(JOB_LIST *, JOB *);
void *startBarber_thread(void *);
void startBarber(int n, JOB_LIST *);


//time is global
//length of time the sistem runs
#define TMAX 100
int t;

//mutex for posting jobs safely
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
//semaphore for posting and taking jobs
sem_t job_queue_length;

int main( int argc, char *argv[] ){
    if(argc != NARGS){
        printf("wrong usage!\nexpected: %s nrBarbers nrCadeiras tCorte tChegada\n", argv[PROGNAME]);
        return 0;
    }

    //innitializes a semaphore to control posting and taking jobs
    sem_init(&job_queue_length, 0,0);

    int nrBarbers = atoi(argv[NRBARBEIROS]);
    int nrCadeiras = atoi(argv[NRCADEIRAS]);
    int tCorte = atoi(argv[TCORTE]);
    int tChegada = atoi(argv[TCHEGADA]);

    JOB_LIST job_list;
    job_list.head = NULL;
    job_list.max_length = nrCadeiras;

    pthread_t *barber = (pthread_t *) malloc(sizeof(pthread_t)*nrBarbers);

    // starts barber shop
    // each thread is a barber
    for(int i=0; i<nrBarbers; i++){
        BARBER_ARGS barber_args;
        barber_args.n = i;
        barber_args.job_list = &job_list;

        pthread_create(&barber[i], NULL, &startBarber_thread, &barber_args);
    }

    // appends to job_list as clients arrive
    int client_counter = 0;
    for(t = 0; t< TMAX; t++){
        // at every tChegada a client arrives and tries to enter the queue
        if(t % tChegada == 0){
            client_counter += 1;
            printf("Cliente %d chegou.\n", client_counter);
            JOB *new = create_job(tCorte, client_counter);
//TODO lock thread
            //locking thread is necessary to guarantee complete posting of a new job b4 someone consumes it
            pthread_mutex_lock (&job_queue_mutex);
            if( append_job(&job_list, new) == -1){
                //appending failed
                printf("Cliente %d foi embora sem cortar o cabelo. Sala de espera cheia.\n", client_counter);
                free_job(new);
            } else {
                //there's a new client
                sem_post(&job_queue_length);
            }
//TODO unlock thread
            pthread_mutex_unlock(&job_queue_mutex);
            // after putting on the queue barbers will wake up by themselves and pick up the jobs
        }
    }

    //joins all threads
    for(int i=0; i<nrBarbers; i++){
        pthread_join(barber[i], NULL);
    }

    //kills the remaining jobs in job_list
    while(job_list.head != NULL){
        free_job( pop_job(&job_list) );
    }
    //kills the semaphore
    sem_destroy(&job_queue_length);


    //for this to work 100% i'd need to check how many jobs were added to the queue SINCE the last wait resolved, not how many are there at every instant
    // this would effectivelly sincronize the barbers and the clock
    // or i could use sleep, which is kinda meh...


    return 0;
}


JOB *create_job(int service_length, int service_id){
    JOB *job = (JOB *) malloc(sizeof(JOB));

    job->service_length = service_length;
    job->service_id = service_id;
    job->next = NULL;


    return job;
}

//pops head of job_list and returns pointer to it
//NULL if list is empty
JOB *pop_job(JOB_LIST *job_list){
    //list is empty
    if(job_list == NULL){
        return NULL;
    } else {
        JOB *tmp;
        tmp = job_list->head;
        job_list->head = job_list->head->next;
        job_list->length -= 1;
        return tmp;
    }
}

void free_job(JOB *job){
    free(job);
    return;
}

// returns -1 if list is full
int append_job(JOB_LIST *list, JOB *job){
    if(list->length >= list->max_length){
        return -1;//list is full
    }
    //find tail
    JOB *runner = list->head;
    //list is empty
    if(runner == NULL){
        list->head = job;
        list->length += 1;
    } else {
        //finds tail
        while(runner->next != NULL){
            runner = runner->next;
        }
        //appends job
        runner->next = job;
        list->length += 1;
    }
    return 0;
}


void *startBarber_thread(void *args){
    BARBER_ARGS *args1 = (BARBER_ARGS *) args;
    startBarber(args1->n, args1->job_list);
    return NULL;
}

// n-th barber
// each barber has acess to the job list
void startBarber(int n, JOB_LIST *job_list){
    BOOLEAN is_sleeping=TRUE;
    while(t<TMAX){
        //if there's no queue, sleep and wait
        if(job_list->head == NULL){
            printf("Barbeiro %d dormindo.\n", n);
            is_sleeping = TRUE;
        }

//locks thread
//nao pode usar mutex_lock aqui pq se um barbeiro travar a thread antes do "postador de tarefas" da ruim

        //will only decrement the length after it picks the job
        sem_wait (&job_queue_length);
        //locks the thread to safely pick the job
        pthread_mutex_lock (&job_queue_mutex);
        //gets new job from queue
        if(is_sleeping == TRUE){
            printf("Barbeiro %d acordou.\n", n);
            is_sleeping = FALSE;
        }
        JOB *current_job = pop_job( job_list );
        //unlocks thread as it picked the job safely
        pthread_mutex_unlock(&job_queue_mutex);
        printf("Barbeiro %d cortando o cabelo do cliente %d.\n", n, current_job->service_id);
        free_job(current_job);
        //idles for a length of time
        for( int i=0; i<current_job->service_length; i++);
    }
    return;
}
