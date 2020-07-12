#include<stdio.h>
#include<sys/shm.h>
#include<sys/stat.h>

// shared memory
// a memory page shared between processes!

int main(){
    int segment_id;
    char *shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 0x6400;

    //allocate shared memory
    segment_id = shmget(IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    printf("segment_id:%d\n",segment_id);


    //attatch shared memory segment
    shared_memory = (char *) shmat(segment_id, NULL, 0);
    printf("shared memory attatched at adress: %p\",shared_memory);
    //getting size of segment
    shmctl(segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    printf("segment_size:%d\n",segment_size);
    //writing something in the shared memory segment
    sprintf(shared_memory, "Hello, World!");
    //detatch mem seg
    shmdt(shared_memory);

    //reattatch mem seg in a different adress
    shared_memory = (char *) shmat(segment_id, (void *) 0x500000, 0);
    printf("shared memory attatched at adress: %p\n",shared_memory);
    //print shared string
    printf("%s\n", shared_memory);
    //detatch mem seg
    shmdt(shared_memory);
    
    //free mem seg
    shmctl(segment_id, IPC_RMID, 0);


    return 0;
}
