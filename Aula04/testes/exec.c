#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[]){

    pid_t parent;

    parent = fork();
    //fork returns != 0 for parent
    if(parent){
        printf("THIS IS EXEC. PID: %d\n", (int) getpid());
    //fork returns 0 for child
    }else if(!parent){
        //exec will substitute the current process
        //execvp will require a null-terminanted char-vector of the args (ls -l -a => execvp("ls", {"-l","-a",NULL }) )
        //calling something without args, requires an empty char-vector (ls = > execvp("ls",{}) )
        char *args[] = {NULL};
        execvp("./exec_target", args);
        printf("this won't run\n");
    }

    return 0;
}
