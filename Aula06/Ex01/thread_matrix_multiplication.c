#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int sum_array(int *, int );
int **read_matrix(int , int );
void free_matrix(int **, int );
void print_matrix(int **, int , int );


int main(int argc, char *argv[]){
    int **m;
    int row=3, col=3;
    m = read_matrix(row,col);

    print_matrix(m, row, col);

    free_matrix(m, row);
    return 0;
}

// returns the sum of elements in an array with given length
int sum_array(int *array, int length){
    int sum = 0;
    for(int i=0;i<length;i++) sum += array[i];
    return sum;
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
