#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>

#define NUM_OF_THREADS 10
#define SIZE 10

pthread_mutex_t mut;
int nums[SIZE];

int tempo = 200;

void sleep_ms(int ms)
{
    usleep(ms * 1000);
}

int cur_idx = 0;

void* readCycle(void* arg) 
{
    (void)arg;
    while (cur_idx < SIZE)
    {
        sleep_ms(tempo); 
        pthread_mutex_lock(&mut);
        printf("reading -\t"); 
        sleep_ms(tempo);
        printf("tid: %i\tdata: [", pthread_self()); 
        for (int i = 0; i < SIZE; i++) 
            printf ("%d", nums[i]);
        printf("]\n");
        sleep_ms(tempo);

        pthread_mutex_unlock(&mut);
    } 

    pthread_exit(NULL);
}

void* writeCycle(void* arg) 
{
    (void)arg;

    for (int i = 0; i < SIZE; i++) 
    {
        sleep_ms(tempo);
        pthread_mutex_lock(&mut);

        printf("\nwriting\n\n");
        cur_idx++;
	    nums[i] = i + 1;
	    pthread_mutex_unlock(&mut);
    }

    pthread_exit(NULL);
}

int main() 
{
    pthread_mutex_init(&mut, NULL);
    int check;
    pthread_t readThreads[NUM_OF_THREADS];
    for (int i = 0; i < NUM_OF_THREADS; i++) 
    {
        check = pthread_create(&readThreads[i], NULL, readCycle, NULL);
        if (check != 0) 
        {
            fprintf(stderr, "pthread create error");
            return 1;
        }
    }

    pthread_t writeThread;
    check = pthread_create(&writeThread, NULL, writeCycle, NULL);
    if (check != 0) 
    {
        fprintf(stderr, "pthread create error");
        return 1;
    }
    for (int i = 0; i < NUM_OF_THREADS; i++) 
    {
        void* status = NULL;

        check = pthread_join(readThreads[i], &status);
        if (check != 0) 
        {
            fprintf(stderr, "pthread join error");		
            return 1;
        }
    }

    pthread_join(writeThread, NULL);
    pthread_mutex_destroy(&mut);
    return 0;
}