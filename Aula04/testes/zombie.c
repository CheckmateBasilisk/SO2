#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char *argv[]){

    pid_t child_pid;

    //forks into a child
    child_pid = fork();
    //current process is the child
    if(child_pid > 0 ){
        //child is too slow and will be left to become a zombie
        sleep(60);
    //current process is the parent
    } else {
        //parent runs away, leaving the child behind to become a zombie...
        exit(0);
    }

    return 0;
}
