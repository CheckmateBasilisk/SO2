
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char *argv[]){

    int lifespan = -1;
    pid_t child_pid;

    while(lifespan!=0){
        printf("child lifespan (0 to end parent): ");
        scanf(" %d", &lifespan);
        if(lifespan == 0) return 0; //end parent

        child_pid = fork();
        printf("creating child with lifespan %d s\n", lifespan);
        //if this process is a child
        if(child_pid == 0){
            sleep(lifespan);
            return 0;
        }
    }

    return 0;
}
