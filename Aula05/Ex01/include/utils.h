#ifndef __UTILS_H__
#define __UTILS_H__

#include<stdio.h>

char *readLine(FILE *, int );
char *appendString(char *, char *);
char *tokenize(char *, int );
char **strToWords(char *, int );

#endif
