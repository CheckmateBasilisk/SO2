#include<stdio.h>
#include<unistd.h>

//this is the target for the exec.c

int main(int argc, char *argv[]){

    printf("THIS IS THE TARGET PID: %d\n", getpid());

    return 0;
}
