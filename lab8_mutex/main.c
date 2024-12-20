#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>

#include <unistd.h>
#include <sys/syscall.h>

#include <sys/types.h>

#define THREAD_NUM 10
#define SIZE 10

pthread_mutex_t mut;
char data[SIZE];

int tempo = 100;

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
        printf("tid: %li\tdata: [", pthread_self()); 
        for (int i = 0; i < SIZE; i++) 
            printf ("%c", data[i]);
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
	    data[i] = (char)(i + 65);
	    pthread_mutex_unlock(&mut);
    }

    pthread_exit(NULL);
}

int main() 
{
    int check;
    pthread_mutex_init(&mut, NULL);
    pthread_t readThreads[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) 
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
    for (int i = 0; i < THREAD_NUM; i++) 
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