#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include<my_shell.h>
#include<utils.h>


// fork, execvp, wait, sigaction, getcwd, getenv, gethostname, strlen, strtok, strncmp, chdir, fgets, strerror, exit

int main(int argc, char *argv[]){

    printf("returned %d\n",my_shell());
/*
    char *line = readLine(stdin, '\n');
    char **word = NULL;

    printf("line: %s\n", line);

    word = strToWords(line, ' ');

    int i;
    for(i = 0; word[i] != NULL; i++){
        printf("word: %s\n", word[i]);
    }
    printf("word: %s\n", word[i]);

    free(line);
    for(i = 0; word[i] != NULL; i++){
        free(word[i]);
    }

    free(word);
*/
    return 0;
}
