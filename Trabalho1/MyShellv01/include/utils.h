#ifndef __UTILS_H__
#define __UTILS_H__

#include<stdio.h>

typedef enum{
    FALSE,
    TRUE
} BOOLEAN;

char *readLine(FILE *, int );
char *appendString(char *, char *);
char *copyString(char *);
char *trim(char *, int , char);
int countWhiteSpaces(char *, int );
char *tokenize(char *, int );
char **strToWords(char *, int );
char **strToWordsWithFullstop(char *, int , int , char**);


#endif
