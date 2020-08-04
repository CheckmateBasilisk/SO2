#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAX 30
#define MAXHOSTLENGTH 100
#define MAXARGS 10

//fork, execvp, wait, sigaction, getcwd, getenv,
//gethostname, strlen, strtok, strncmp, chdir, fgets,
//strerror, exit

int spawn(char* program, char** arg_list){
	pid_t child_pid;	//cria um processo?
    int childReturn;
    int child_status = -1;

    //fork retorna o pid do filho
    //retorna 0 se o processo atual FOR o filho
	child_pid = fork();	//kagebushin do processo
	
    //eh o pai
	if(child_pid != 0){
        wait(&child_status);
        if(WIFEXITED(child_status)){
            childReturn = WEXITSTATUS(child_status);
        }

		return childReturn;
    }else{
		execvp(program, arg_list);	//executa programo no diretorio atual
		fprintf(stderr, "programa inexistente\n Error: No such file or directory.\n");
		exit(-1);
	}

    return 0;

}

void tokenizer(char *string, char **token){
		int cont = 0;
		char * tmp;
	
		
	    tmp = strtok(string," ");
		token[cont] = tmp;
		cont++;
		while(tmp != NULL){
		    tmp = strtok(NULL," ");
		    token[cont] = tmp;
		    cont++;
		}
		cont--;
		
}

int main()
{
	char comando[MAX];
	char *arg_l[MAXARGS];

	while(1){
	
		char hostname[MAXHOSTLENGTH];
		gethostname(hostname, MAXHOSTLENGTH);
		printf("%s@%s:%s> ",getenv ("USER"),hostname,getcwd(NULL,0));
		

		fgets(comando, MAX, stdin);
		
		comando[strlen(comando)-1] = '\0';
		
		if(strcmp(comando,"exit") == 0)
			return 0;
			
		if(strcmp(comando,"" ) == 0)
			continue;
		
		tokenizer(comando, arg_l);
		
		if(strcmp(arg_l[0],"cd") == 0){
			if(chdir(arg_l[1]) == -1){
				printf("Error: No such file or directory.\n");
			}	
			continue;
		}
		spawn(arg_l[0], arg_l);
	}
	return 0;
	
}
