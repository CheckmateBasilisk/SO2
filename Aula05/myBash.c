
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_CMD_SIZE 512
#define MAX_ARG_NUMBER 10

void printPromptString(); //cria e printa a Prompt String
char* parseCWD(); //se cwd for filho ou o próprio $HOME, coloca '~' no lugar do $HOME
char* getCommand(char* command,unsigned long size); //retorna stdin menos \n
char** split(char* str); //divide str em substrings separadas por ' '
void runCommand(char* command, char** argList); //roda o comando se for um built-in, senão chama spawn
int spawn(char* program, char** argList); //spawna um processo do programa de entrada

char* user;
char* name;
char* home;

int main(){
    user = getenv("USER");
    name = getenv("NAME");
    home = getenv("HOME");
    int childStatus;
    char* command = (char*)malloc(MAX_CMD_SIZE * sizeof(char));
    char** argList;

    while(1){
        printPromptString();
        command = getCommand(command,MAX_CMD_SIZE);
        argList = split(command);
        runCommand(argList[0],argList);
        wait(&childStatus);
    }

    return 0;
}

int spawn(char* program, char** argList){
    pid_t childPid;

    childPid = fork();
    if(childPid != 0)
        return childPid;
    else{
        execvp(program,argList);
        fprintf(stderr,"Error: no such file of directory.\n");
        abort();
    }
}

void printPromptString(){
    char* GREEN = "\033[0;32m";
    char* BLUE = "\033[0;34m";
    char* ENDCOLOR = "\033[0m";

    char* cwd = parseCWD();

    printf("%s%s@%s:%s%s%s$ ",GREEN,user,name,BLUE,cwd,ENDCOLOR);
}

char* parseCWD(){
    char* cwd = getcwd(NULL,0);
    char* parsedCwd = malloc(strlen(cwd));
    if(strncmp(cwd,home,strlen(home)) == 0){
        parsedCwd[0]='~';
        strcpy(parsedCwd+1,cwd+strlen(home));
        return parsedCwd;
    }
    else return cwd;
}

char* getCommand(char* command,unsigned long size){
    fgets(command,size,stdin);
    command[strlen(command)-1] = '\0';
    return command;
}

void runCommand(char* command, char** argList){
    if(*argList == NULL)
        return;

    if(strcmp(command,"cd") == 0){
        if (chdir(argList[1]) == -1)
            fprintf(stderr,"Error: no such file of directory.\n");
    } else if(strcmp(command,"exit") == 0)
        exit(0);
    else
        spawn(command,argList);
}

char** split(char* str){
    char** splices = malloc(MAX_ARG_NUMBER * sizeof(char*));
    char delim[2] = " ";

    char* token = strtok(str,delim);
    for(int i=0; token!=NULL && i<MAX_ARG_NUMBER;i++){
        splices[i] = token;
        token = strtok(NULL,delim);
    }

    return splices;
}
