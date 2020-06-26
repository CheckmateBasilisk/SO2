#include<stdio.h>
#include<stdlib.h>


//returns a null-terminated, dynamically allocated string ( char *) read from string until delimiter is met
char *readLine(FILE *stream, int delimiter){
    char *line=NULL;
    int counter = 0;
    char c;

    do{
        c = fgetc(stream);
        line = (char *) realloc(line, (counter+1)*sizeof(char));
        line[counter] = c;
        counter++;
    }while(c!=delimiter);
    line[counter-1] = '\0';

    return line;
}
