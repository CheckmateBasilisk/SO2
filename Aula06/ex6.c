#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>

void* matrixSum (void *arg)
{
	int i;
	intptr_t *arr = (intptr_t*)arg;
	intptr_t sum = 0;
	
	for(i=0;i<5;i++)
		sum += arr[i];
		
	return(void*) sum;
}


int main(){
	srand(time(NULL));
	
	pthread_t threads[4];
	
	int matrix[4][5];
	int i, j;
	intptr_t line[5];
	intptr_t res;
	
	for(i=0;i<4;i++){
		for(j=0;j<5;j++)
			matrix[i][j] = rand() % 100;
	}
	
	for(i=0;i<4;i++){
		for(j=0;j<5;j++)
			printf("%d ",matrix[i][j]);
		
		printf("\n");
	}
	
	for(i=0;i<4;i++){
		for(j=0;j<5;j++)
			line[j] = matrix[i][j];
			
		pthread_create(&(threads[i]), NULL, &matrixSum, line);
		pthread_join(threads[i],(void*)&res);
		printf("O valor da soma da linha %d e %"PRIdPTR".\n", i+1, res);
	}
		
	return 0;
}
