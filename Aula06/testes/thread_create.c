#include<stdio.h>
#include<pthread.h>

void *print_xs(void *unused){
    while(1){
        //fputc('x', stderr);
        printf("x");
    }
    return NULL;
}

int main(){
    //pthread_t is a type that stores thread ids
    pthread_t thread_id;

    //spawn a new thread, tell it :
    //  -where to put the id,
    //  - details on how to run
    //  - the function to run
    //  - and a ptr to their parameters
    pthread_create(&thread_id ,NULL, &print_xs, NULL);

    // threads are part of the same process and die with it
    while(1){
        printf("y");
    }

    return 0;
}
