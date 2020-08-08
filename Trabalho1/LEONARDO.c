#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

	//volatile sig_atomic_t sinal = 0;

void CTRLC(int sinal){
	printf("CTRL+C foi pressionado! Ignorando...\n");
}

void CTRLZ(int sinal){
	printf("CTRL+Z foi pressionado! Ignorando...\n");
}

char** separar(char* string, int tamanho){		//divide a string com espa�os
	int i=0;
	char** partes = malloc(tamanho*sizeof(char*));
	char* simbolo = strtok(string," ");
	while(i<tamanho && simbolo!=NULL){
		partes[i] = simbolo;
		simbolo = strtok(NULL," ");
		i++;
	}
	//partes[i-1] = NULL;
	return partes;

}

int nascimento(char* comando, char** lista){		//cria��o do processo
	pid_t pidFilho;
	pidFilho=fork();
	if(pidFilho!=0){
		return pidFilho;
	}
	else{
		execvp(comando, lista);
		fprintf(stderr,"Erro: arquivo ou diret�rio n�o encontrado.\n");
		abort();
	}
}



int main(){
	signal(SIGINT, CTRLC);		//impede CTRL+C de realizar a interrupcaoo do programa
	signal(SIGTSTP, CTRLZ);	//impede CTRL+Z de realizar a parada do programa
	//signal(SIGINT, encerrar);	//aciona quando CTRL+D for apertado para encerrar o processo
	char* pwd;
	char* tempPwd = malloc(strlen(pwd));
	int filho;
	int tamanho_max = 512;
	int executando = 1;
	char* comando = (char*)malloc(tamanho_max*sizeof(char));
	char** lista = malloc(10 * sizeof(char));


	while(executando){

		pwd = getcwd(NULL,0);
		if(strncmp(pwd, getenv("HOME"), strlen(getenv("HOME"))) == 0){		//strlen(getenv("HOME")
			tempPwd[0] = '~';		//coloca til no primeiro espa�o da string temporaria
			strcpy(tempPwd + 1, pwd + strlen(getenv("HOME")));	// copia para tempPwd+1 (a partir do segundo endere�o) a string pwd depois de pular home/user
			strcpy(pwd, tempPwd);
			printf("[MySh] %s@%s: %s> ", getenv("USER"), getenv("NAME"), pwd); //print do endere�o
		} else {
			printf("[MySh] %s@%s:%s> ", getenv("USER"), getenv("NAME"), pwd); //print do endere�o
		}


		fgets(comando, tamanho_max, stdin);	//leitura da string
		if(feof(stdin)){		//checa se o input � EOF, ou seja, checa se CTRL+D foi pressionado
			printf("CTRL+D foi pressionado! Encerrando...\n");
			exit(0);
		}
		comando[strlen(comando)-1] = '\0';	//coloca \0 no ultimo endere�o da string
		lista = separar(comando, strlen(comando));	//realiza separa��o para leitura da string
		if (strcmp(comando,"exit") == 0){	//checa se exit foi digitado
			exit(0);
		}else if(strcmp(comando,"") == 0){
			continue;
		}else if (strcmp(comando,"cd") == 0){	//checa se o comando usado foi cd
			if(strcmp(lista[1],"~") == 0 || lista[1] == NULL){		//SEGUNDA CONDI��O N�O FUNCIONA, D� SEGFAULT
				chdir(getenv("HOME"));
			}
			else {
				if (chdir(lista[1]) == -1){
					perror("Error");
				}

			}
		}else{
			nascimento(comando, lista);	//chama fun��o para criar novo processo
			wait(&filho);
		}


	}
	return 0;
}
