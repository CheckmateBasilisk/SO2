#ifndef __MY_SHELL_H__
#define __MY_SHELL_H__

#include<stdio.h>

int print_shell(char *, char *, char *);
int my_shell(FILE *);
int spawn(char* , char** );
int spawn_pipe(char *, char **, int , int );
int run_nextCmd(char *, int );
int run_cmd(char *);

#endif
