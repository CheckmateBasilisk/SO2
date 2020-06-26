#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

int main(int argc, char *argv[]){

    pid_t pid;//stores the PID of the current program
        //fork returns >0 if the current process is the parent
        //fork returns 0 if the current process is the child
    pid = fork();
    printf("starting process PID: %d\n",(int) getpid());


    //child will count even
    if(pid == 0){
        for(int i =0; i < 20; i+=2){
            printf("child: %d\n", i);
        }
    //parent will count odd
    }else if(pid!=0){
        //will wait until the child returns (and prints all the even numbers in the process)
        int child_status = -1; // keeps the exit/return status of the child
        wait(&child_status);
        if(child_status != -1){//child has returned alredy. Doesn't work all the times
            if(WIFEXITED(child_status)){
                printf("child returned: %d\n", WEXITSTATUS(child_status));
            }else{
                printf("child was aborted");
            }
        }
        for(int i =1; i < 20; i+=2){
            printf("parent: %d\n", i);
        }
    }

    return 0;
}
