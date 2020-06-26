#include<stdlib.h> // included by default
#include<stdio.h>

int main(int argc, char *argv[]){

    int return_value = system("ls");
    printf("ls returned : %d\n", return_value);

    return 0;
}
