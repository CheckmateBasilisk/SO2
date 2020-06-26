#include<stdio.h>
#include<unistd.h>
//#include<sys/types.h>

int main(int argc, char *argv[]){

    pid_t pid;
    
    printf("current pid: %d\n",(int) getpid());
    printf("parent pid: %d\n",(int) getppid());

    return 0;
}
