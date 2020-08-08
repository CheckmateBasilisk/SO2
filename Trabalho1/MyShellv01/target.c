#include<stdio.h>

int main(int argc, char *argv[]){
    printf("I'm the Target! These are my args:\n");
    for(int i=0; i<argc; i++){
        printf("%s\n",argv[i]);
    }

    return 0;
}
