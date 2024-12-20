#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <errno.h>
#include <unistd.h>

#include <string.h>
#include <sys/sem.h>

#define SHMEM_SIZE 64
#define SHMEM_FILE "sharedfile322"

char * shared_memory = NULL;
struct sembuf l = {0,-1,0};
struct sembuf o = {0,1,0};

void handle_signal(int sig)
{
    if (shared_memory)
        if (shmdt(shared_memory) < 0)
            fprintf(stderr, "shmdt error\n");

    exit(0);
}

int main()
{
    key_t key = ftok(SHMEM_FILE, 1);
    if (key < 0){
        fprintf(stderr, "critical ftok error\n");
        return -1;
    }
    int shmid = shmget(key, SHMEM_SIZE, 0666);
    if (shmid < 0){
        fprintf(stderr, "critical shmget error\n");
        return -1;
    }
    int semid = semget(key, 1, 0666);
    if (semid == -1){
        fprintf(stderr, "critical segment error\n");
        return -1;
    }
    shared_memory = shmat(shmid, NULL, SHM_RDONLY);
    if (shared_memory == (void *)-1){
        fprintf(stderr, "critical shmat error\n");
        return -1;
    }
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    char local_copy[SHMEM_SIZE] = {0};
    while (1){
        semop(semid, &l, 1);
        strcpy(local_copy, shared_memory);
        semop(semid, &o, 1);

        time_t cur = time(NULL);
        struct tm * current_time = localtime(&cur);

        char time_buffer[32];
        strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", current_time);

        printf("time=%s\npid=%d\nshared hemory: %s\n", time_buffer, getpid(), local_copy);
    }

    return 0;
}