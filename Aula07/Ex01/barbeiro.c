#include<stdio.h>
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

int main( int argc, char *argv[] ){
    if(argc != NARGS){
        printf("wrong usage!\nexpected: %s nrBarbeiros nrCadeiras tCorte tChegada\n", argv[PROGNAME]);
        return 0;
    }

    int nrBarbeiros = atoi(argv[NRBARBEIROS]);
    int nrCadeiras = atoi(argv[NRCADEIRAS]);
    int tCorte = atoi(argv[TCORTE]);
    int tChegada = atoi(argv[TCHEGADA]);

    pthread_t *barbeiro = (pthread_t *) malloc(sizeof(pthread_t)*nrBarbeiros);

    for(int i=0; i<nrBarbeiros; i++){
        pthread_create(&barbeiro[i], NULL, &thread_function, NULL);
    }
    while(tChegada){
        printf("cliente chegou\n");
        sleep(1);
    }

    return 0;
}


void *thread_function(void *unused){

}
