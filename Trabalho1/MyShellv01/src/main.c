#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

#include<my_shell.h>

/*
    TRABALHO 1 - MY SHELL - GRUPO:

GABRIEL PARANATINGA DORNELAS
LEONARDO JARDIM PINHATI
LUCAS BAGATINI DO NASCIMENTO

*/



/*
o que não funciona

- pipe requer espaço antes do | , mas depois não precisa
    ls | cowsay  (OK)
    ls |cowsay  (OK)
    ls| cowsay  (NOPE)


- ctrl+z é capturado, mas não impede SIGTSTP de colocar o programa em plano de fundo

*/


int main(int argc, char *argv[]){
    return my_shell(stdin);
}
