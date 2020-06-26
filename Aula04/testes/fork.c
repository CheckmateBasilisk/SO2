#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[]){
   
    pid_t pid;//stores the PID of the current program
        //fork returns >0 if the current process is the parent
        //fork returns 0 if the current process is the child 
    printf("starting process PID: %d\n",(int) getpid());
    pid = fork();
    //child will count even
    if(pid == 0){
        for(int i =0; i < 200; i+=2){
            printf("child: %d\n", i);
        }
    //parent will count odd 
    }else if(pid!=0){
        for(int i =1; i < 200; i+=2){
            printf("parent: %d\n", i);
        }
    }

    return 0;
}
