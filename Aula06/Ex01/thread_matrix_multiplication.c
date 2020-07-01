#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

int sum_array(int *, int );
int **read_matrix(int , int );
void free_matrix(int **, int );
void print_matrix(int **, int , int );


struct sum_array_thread_params{
    int *array;
    int length;
    int *result;
};

//thread function
//recieves a ptr to the parameters
//returns null, since i couldnt get it to work properly
void *sum_array_thread(void *params){
    intptr_t sum = 0;
    int *array, length;
    struct sum_array_thread_params *p = params;
    //treats params as a pointer to the struct so it can get the fields correctly
//    array = ((struct sum_array_thread_params *) params)->array;
//    length = ((struct sum_array_thread_params *) params)->length;

    array = p->array;
    length = p->length;

    for(int i=0; i<length; i++){
        sum += array[i];
    }

    //returning ptr to local variable can break things!
    //return &sum;

    //this looks disgusting...
    //return ((struct sum_array_thread_params *) params)->return;

    //puts the partial sum where the params tells it to
//    * ((struct sum_array_thread_params *) params)->result = sum;
    *p->result = sum;
    return NULL;
}

int main(int argc, char *argv[]){
    int **m;
    int row=3, col=3;
    pthread_t *thread_id;

    m = read_matrix(row,col);
    //will create a thread per row
    thread_id = (pthread_t *) malloc(sizeof(pthread_t)*row);
    int *partial_sum = (int *) malloc(sizeof(int)*row);
    struct sum_array_thread_params params;
    for(int i=0; i < row; i++){
        //spawns a thread for every row of the matrix
        //the thread calculates the sum of all elements in that row

        params.array = m[i];
        params.length = col;
        params.result = &partial_sum[i];
/*
printf("params: %p, %d, %p\n", params.array, params.length, params.result);
*/
        pthread_create(&(thread_id[i]), NULL, &sum_array_thread, &params);
    }

    //waits for threads to finish, joining them
    //prints partial results, accumulates total results
    //prints final result
    int sum = 0;
    for(int i=0; i < row; i++){
        //had lots of problems getting the return to work :c
        pthread_join(thread_id[i], NULL);
        printf("Linha %d: %d\n", i+1, partial_sum[i]);
        sum += partial_sum[i];
    }
    printf("Total: %d\n", sum);


    free(thread_id);
    free(partial_sum);
    free_matrix(m, row);
    return 0;
}



// returns a ptr to a dynamically allocated row x colmatrix
int **read_matrix(int row, int col){
    int **m = (int **) malloc(sizeof(int*)*row);
    for( int i=0; i<row; i++){
        m[i] = (int *) malloc(sizeof(int)*col);
        for(int j=0; j<col; j++){
            scanf(" %d", &m[i][j]);
        }
    }
    return m;
}

// frees a row x any matrix
void free_matrix(int **m, int row){
    for( int i=0; i<row; i++){
        free(m[i]);
    }
    free(m);
    return;
}




// returns the sum of elements in an array with given length
int sum_array(int *array, int length){
    int sum = 0;
    for(int i=0;i<length;i++) sum += array[i];
    return sum;
}

// prints to stdio a row x col integer matrix
void print_matrix(int **m, int row, int col){

    for(int i=0; i<row; i ++){
        for(int j=0;j<col;j++){
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
    return;
}
