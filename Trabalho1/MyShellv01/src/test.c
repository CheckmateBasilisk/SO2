#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include<my_shell.h>
#include<utils.h>


// fork, execvp, wait, sigaction, getcwd, getenv, gethostname, strlen, strtok, strncmp, chdir, fgets, strerror, exit
int main(int argc, char *argv[]){

        char *line = readLine(stdin,'\n');
        printf("%s\n", line);

        char **words;
        char *rest;

        words = strToWordsWithFullstop(line, ' ', '|' , &rest);

        printf("[");
        for(int i=0; words[i]!=NULL; i++){
            printf(" \"%s\",", words[i]);
        }
        printf("NULL ]");
        rest = trim(rest,' ');
        printf("rest: \"%s\"\n", rest);

        for(int i=0; words[i] != NULL; i++){
            free(words[i]);
        }
        free(words);
        free(rest);
        free(line);


/*
        char *line = readLine(stdin, '\n');
        printf("%s\n", line);
        printf("%d whitespaces\n",countWhiteSpaces(line, '1'));
        free(line);
*/

/*
    char *line = readLine(stdin, '\n');
    char *trimmed;
    printf("\"%s\"\n",line);
    trimmed = trim(line, ' ');
    printf("\"%s\"\n",trimmed);

    free(line);
    free(trimmed);
*/


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
