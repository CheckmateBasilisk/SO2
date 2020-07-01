#include<stdio.h>
#include<pthread.h>


// puts sum of elements in an array with given length in result
void sum_array(int *array, int length, int *result){
    int sum = 0;
    for(int i=0;i<length;i++) sum += array[i];
    *result = sum
    return;
}

struct thread_sum_array_params{
    int *array;
    int length;
    int *ret_value;//return value
};

#define THREAD_COUNT 2

//runs in a separate thread
//races are the dev's problems :D
// returns the return of sum_array.
// puts in thread_id the thread_id of the spawned thread.
int thread_sum_array(int *array, int length, pthread_t *thread_id){
    int sum = 0;
    struct thread_sum_array_params{
        int *array;
        int length;
        int ret_value;//return value
    } params;

    params.array = array;
    params.length = length;
    params.ret_value = 0;

    //spawns the thread
    pthread_create(thread_id ,NULL, &sum_array, NULL);

    sum = params.ret_value;
    return sum;
}

int main(){
    int array1[10] = {1,2,3,4,5,6,7,8,9,10};//should add up to 55
    int array2[10] = {1,2,3,4,5,6,7,8,9,10};
    int length = 10;

    int sum1;
    int sum2;

    pthread_t thread_id1;
    pthread_t thread_id2;

    sum1 = thread_sum_array(array1, 10, &thread_id1, &sum1);



    printf("sum: %d\n", sum1+sum2);

}
