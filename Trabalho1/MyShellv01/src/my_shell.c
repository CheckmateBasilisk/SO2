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


//desired interruptions
//TODO maybe these functions are too long for interruptions
//CTRL+c should be ignored
void CTRLC(int sinal){
    char user[MAX_USER_SIZE];
    char cwd[MAX_PATH_SIZE];
    char hostname[MAX_HOST_SIZE];

	fprintf(stderr,"\nCTRL+C pressed!\n");
    getlogin_r(user, MAX_USER_SIZE);
    getcwd(cwd, MAX_PATH_SIZE);
    gethostname(hostname, MAX_HOST_SIZE);
    print_shell(user, cwd, hostname);
    //spits out the print_shell
    fflush(stdout);

    return;
}
//TODO ctrlz is captured but SIGTSTP isnt
//CTRL+z should be ignored
void CTRLZ(int sinal){
    signal(SIGTSTP, CTRLZ);
    char user[MAX_USER_SIZE];
    char cwd[MAX_PATH_SIZE];
    char hostname[MAX_HOST_SIZE];


	fprintf(stderr,"\nCTRL+Z pressed!\n");
    getlogin_r(user, MAX_USER_SIZE);
    getcwd(cwd, MAX_PATH_SIZE);
    gethostname(hostname, MAX_HOST_SIZE);
    print_shell(user, cwd, hostname);
    //spits out the print_shell
    fflush(stdout);

    return;
}


//core loop
// TODO: reads an input cmd and forks itself to run that command
// TODO: runs programs
// will not strip additional empty spaces from cmd!
int my_shell(FILE *stream){
    //desired interruptions
    signal(SIGINT, CTRLC);		//impede CTRL+C de realizar a interrupcaoo do programa
    //TODO cant capture SIGTSTP properly. Recognizes the CTRL+z but it goes to the background anyways
	signal(SIGTSTP, CTRLZ);	//impede CTRL+Z de realizar a parada do programa

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
        cmd = readLine(stream, '\n');

        //if CTRL+d was pressed/met, cmd readLine will return NULL
        if(cmd == NULL){
            printf("\nCTRL+D pressed! Exiting...\n");
            return 0;
        //exiting
        } else if(strcmp(cmd, "exit") == 0){
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

    //if thte cwd starts with a substring equal to the home directory, ignore that substring and preceed it by "~"
    if(strncmp(cwd, getenv("HOME"), strlen(getenv("HOME"))) == 0){
        printf("[MySh] %s@%s:~%s $ ", user, hostname, &cwd[strlen(getenv("HOME"))] );
    } else {
        printf("[MySh] %s@%s:%s $ ", user, hostname, cwd );
    }
    return 0;
}

//will parse input into args
//will spawn child process to run input
//will take care of freeing the args but not input!
//TODO for now, requires a space between '|'
int run_cmd(char *input){
    if(input == NULL) return 0;

    //TODO make this prettier. Ad hoc implementations are ugly ç__ç
    char *cmd = trim(input, ' ', 'r');//rips all whitespaces at the beginning
    cmd = trim(cmd, ' ', 'l');//rips all whitespaces at the ending

    char *rest = NULL;
    char **args = strToWordsWithFullstop(cmd,' ', '|', &rest);
    int returnValue = 0;

    //TODO will break if path has ' ' like cd ./"foo bar" :c
    if(strcmp(args[0],"cd") == 0){
        if (args[1] == NULL || strcmp(args[1],"~") == 0){
            //cd to home!
            //TODO maybe there's a better way other than using getenv
            chdir(getenv("HOME"));
        } else if (chdir(args[1]) == -1){
            fprintf(stderr,"Error: no such file of directory.\n");
            returnValue = -1;//should use another return value?
        }
    //if found the fullstopDelimiter  rest != NULL
    } else if(rest != NULL){
        //need to set up the pipes from the first to the other calls to run_NextCmd

        //calls run_nextCmd on the right-side of the pipe
        //run_nextCmd will recursively take care of further pipes
        int fd[2];
        //fd[0] => readFD
        //fd[1] => writeFD
        pipe(fd);//creates a pipe
        //recursively calls run_cmd on the right-side of the pipe

        //spawns a new task with a different file descriptor for writing
        returnValue = spawn_pipe(args[0],args, fd[0], fd[1]);
        // TODO if return value indicates something went wrong... do something
        //printf("rest:%s\n", &rest[1]);
        close(fd[1]);//closes the writing file descriptor
        //recursively runs the rest of the command line with a different file descr for reading
        returnValue = run_nextCmd(&rest[1],fd[0]);//rest[0] = '|'
        close(fd[0]);//closes the reading file descriptor

    } else {
        returnValue = spawn(args[0],args);
    }

    //frees args
    for(int i=0;args[i] != NULL; i++){
        free(args[i]);
    }
    free(args);

    free(cmd);
    return returnValue;
}


//will parse input into args
//will spawn child process to run input
//will take care of freeing the args but not input!
//readFD is the file descriptor of the input
//TODO for now, requires a space between '|'
int run_nextCmd(char *input, int readFD){
    if(input == NULL) return 0;

    //TODO make this prettier. Ad hoc implementations are ugly ç__ç
    char *cmd = trim(input, '|', 'l');//rips all pipes at the beginning
    cmd = trim(cmd, ' ', 'l');//rips all whitespaces at the beginning

    char *rest = NULL;
    char **args = strToWordsWithFullstop(cmd,' ', '|', &rest);
    int returnValue = 0;

    //TODO will break if path has ' ' like cd ./"foo bar" :c
    if(strcmp(args[0],"cd") == 0){
        if (args[1] == NULL || strcmp(args[1],"~") == 0){
            //cd to home!
            //TODO maybe there's a better way other than using getenv
            chdir(getenv("HOME"));
        } else if (chdir(args[1]) == -1){
            fprintf(stderr,"Error: no such file of directory.\n");
            returnValue = -1;//should use another return value?
        }
    //if found the fullstopDelimiter  rest != NULL
    } else if(rest != NULL){
        int fd[2];
        //fd[0] => readFD
        //fd[1] => writeFD
        pipe(fd);//creates a pipe
        //recursively calls run_cmd on the right-side of the pipe

        //spawns a new task with a different file descriptor for writing
        //the read file descriptor is the one passed as parameter
        returnValue = spawn_pipe(args[0], args, readFD, fd[1]);
        printf("rest:%s\n", rest);
        close(fd[1]);//closes the writing file descriptor

        //recursively runs the rest of the command line with a different file descr for reading
        run_nextCmd(&rest[1],fd[0]);//rest[0] = '|'
        close(fd[0]);//closes the reading file descriptor
    } else {
        //readFD here is the one that came from outside (parameter)
        // the write fd == 0 makes it so that spawn_pipe spits out the results to stdout
        returnValue = spawn_pipe(args[0],args, readFD, 0);
    }

    //frees args
    for(int i=0;args[i] != NULL; i++){
        free(args[i]);
    }
    free(args);

    free(cmd);
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

//creates the 'clone' process that substitutes itself for the program
//won't free cmd or args!
//just like spawn() but expects a file descriptor for reading (readFD) and writing (readFD)
int spawn_pipe(char *cmd, char **args, int readFD, int writeFD){
    pid_t pid;//stores the PID of the current program
        //fork returns >0 if the current process is the parent
    int returnValue=0;

    //fork returns 0 if the current process is the child
    pid = fork();
    if(pid == 0) {
    //is the child
        //links the readFD with the stdin
        if(readFD != 0){
            dup2(readFD,STDIN_FILENO);
        }
        //links the readFD with the stdout
        if(writeFD != 0){
            dup2(writeFD, STDOUT_FILENO);
        }
        //terminates itself after execvp, no need to worry about exiting afterwords
        execvp(cmd, args);
        //unless it causes an error and doesnt morphs itself into the target process
        fprintf(stderr,"Error: no such file of directory..\n");

        exit(-1);//should use another return value?
        //if i use return here the 'clone' process wont die
    } else {
    //is the parent
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
