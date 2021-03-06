#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#include<utils.h>
#include<my_shell.h>

#define MAX_USER_SIZE 50
#define MAX_PATH_SIZE 1024
#define MAX_HOST_SIZE 1024


//core loop
// TODO: reads an input cmd and forks itself to run that command
// TODO: runs programs
// will not strip additional empty spaces from cmd!
int my_shell(){
    char user[MAX_USER_SIZE];
    char cwd[MAX_PATH_SIZE];
    char hostname[MAX_HOST_SIZE];

    char *cmd = NULL;

    //core loop
    do{
        //printing shell line
        getlogin_r(user, MAX_USER_SIZE);
        getcwd(cwd, MAX_PATH_SIZE);
        gethostname(hostname, MAX_HOST_SIZE);
        print_shell(user, cwd, hostname);
        //getting the command from stdin
        cmd = readLine(stdin, '\n');

        //exiting
        if(strcmp(cmd, "exit") == 0){
            free(cmd);
            return 0;
        } else if(strcmp(cmd,"")==0){
            //do nothing on empty commands
            free(cmd);
            continue;
        } else {
            run_cmd(cmd);
        }
        free(cmd);
    //basically forever...
    }while(cmd != NULL);

    return 0;
}

//prints the shell "header"
//"[MySh] user@cwd:host $"
int print_shell(char *user, char *cwd, char *hostname){
    printf("[MySh] %s@%s:%s$ ", user, cwd, hostname);
    return 0;
}

//will parse cmd into args
//will spawn child process to run cmd
//will take care of freeing the args but not cmd!
int run_cmd(char *cmd){
    char **args = strToWords(cmd,' ');
    int returnValue = 0;
    //TODO will break if path has ' ' like cd ./"foo bar" :c
    if(strcmp(args[0],"cd") == 0){
        if (chdir(args[1]) == -1){
            fprintf(stderr,"Error: no such file of directory.\n");
            returnValue = -1;//should use another return value?
        }
    } else {
        returnValue = spawn(args[0],args);
    }

    //frees args
    for(int i=0;args[i] != NULL; i++){
        free(args[i]);
    }
    free(args);

    return returnValue;
}


//creates the 'clone' process that substitutes itself for the program
//won't free cmd or args!
int spawn(char* cmd, char** args){
    pid_t pid;//stores the PID of the current program
        //fork returns >0 if the current process is the parent
    int returnValue=0;

    //fork returns 0 if the current process is the child
    pid = fork();
    //is the child
    if(pid == 0) {
        //terminates itself after execvp, no need to worry about exiting afterwords
        execvp(cmd, args);
        //unless it causes an error and doesnt morphs itself into the target process
        fprintf(stderr,"Error: no such file of directory..\n");

        exit(-1);//should use another return value?
        //if i use return here the 'clone' process wont die

    } else {
        int child_status = -1;
        //will wait until the child returns
        // keeps the exit/return status of the child in child_status
        //locks current process until child updates their info and ends
        wait(&child_status);
        //retrieves child info and return value
        if(WIFEXITED(child_status)){
            returnValue = WEXITSTATUS(child_status);
        }else{
            printf("process was aborted!\n");
            returnValue = -1;//best return value?
        }
    }

    return returnValue;
}


int run_cmdDEPRECATED_AND_BROKEN(char *cmd){
    int returnValue = 0;
    pid_t pid;//stores the PID of the current program
        //fork returns >0 if the current process is the parent

    //parsing command to get args
    //args[0] is the program name
    char **args = strToWords(cmd, ' ');

printf("cmd: %s\n", cmd);
int count;
for(count = 0; args[count] != NULL; count++){
    printf("arg[%d]:%s\n",count, args[count]);
}
printf("arg[%d]:%s\n",count, args[count]);

    //should not fork to execute a cd, since it would spawn another shell
    if (strcmp(args[0], "cd")==0) {
        //will break if path has ' ' like cd ./"foo bar" :c
        if (chdir(args[1]) == -1){
            fprintf(stderr,"Error:no such file of directory.\n");
            returnValue = -1;//best return value?
        }
    } else {
        //fork returns 0 if the current process is the child
        pid = fork();
        //is the child
        if(pid == 0){
            // TODO also, cd breaks stuff. Each cd is spawning a new shell ???
            //args has the format {"foo","bar",NULL}, seems to happen only when cd first

            execvp(cmd, args);
            //terminates itself after execvp, no need to worry about exiting afterwords
        //is the parent
        }else if(pid != 0){
            int child_status = -1;
            //will wait until the child returns
            // keeps the exit/return status of the child in child_status
            //locks current process until child updates their info and ends
            wait(&child_status);
            //retrieves child info and return value
            if(WIFEXITED(child_status)){
                returnValue = WEXITSTATUS(child_status);
            }else{
                printf("process was aborted!\n");
                returnValue = -1;//best return value?
            }
        //if for some reason, fork was impossible
        }else{
            fprintf(stderr, "fork error!\n");
            returnValue = -1;//best return value?
        }
    }

    //free parsed command
    //only the parent will run this no risk of double free
    int i;
    for(i = 0; args[i] != NULL; i++){
        free(args[i]);
    }
    free(args);

    return returnValue;
}
