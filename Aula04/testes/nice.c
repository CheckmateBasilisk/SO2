#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[]){

    //the nicer the process, the lower priority
    //low niceness doesn't care about other processes

    //nice -n niceValue(0-20) process/command
    //nice -n 20 ls

    //for running processes
    //renice -n niceValue(0-20) PID

    //this is just a looong process
    for(int i=0; i < 1000000000 ; i++){
        if( i%100000000 == 0 ){
            printf("\nstill running...");
        }
    }
    printf("====GETTING NICIER===");
    nice(20);
    for(int i=0; i < 1000000000 ; i++){
        if( i%100000000 == 0 ){
            printf("\nstill running...");
        }
    }

    return 0;
}
