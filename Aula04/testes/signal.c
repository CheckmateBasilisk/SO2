#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

sig_atomic_t sigusr1_count = 0;

//will be called whenever an interruption with the signal occurs
//has a parameter because the struct sigaction demands it
void interruption(int signal_id){
    sigusr1_count+=1;
    return;
}

int main(int argc, char *argv[]){

    //setting up the "interruption infrastructure"
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &interruption;
    sigaction(SIGUSR1, &sa, NULL);


    printf("b4\n");
    //this can be interrupted many times
    for(int i =0 ; i <30; i++){
        sleep(30);
    }
    //this can only be interrupted once!
    //sleep(30);
    printf("after\n");

    printf("sigusr1: %d\n", sigusr1_count);
    
    return 0;
}
