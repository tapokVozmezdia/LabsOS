#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>

#include <unistd.h>
#include <sys/syscall.h>

#include <sys/types.h>

#define THREAD_NUM 10
#define SIZE 10

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int cur_idx = 0;
int nums[SIZE];

int tempo = 800;

void sleep_ms(int ms)
{
    usleep(ms * 1000);
}

void* readCycle(void* arg) 
{
    (void)arg;
    while (cur_idx < SIZE) 
    {
        pthread_mutex_lock(&mut);
        pthread_cond_wait(&cond, &mut);
        printf("reading -\t"); 
        printf("tid: %li\tdata: [ ", pthread_self()); 
        for (int i = 0; i < SIZE; i++) 
            printf ("%d ", nums[i]);
        printf("]\n");
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
        pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mut);
    }
    pthread_exit(NULL);
}

int main() {

    int check;
    pthread_mutex_init(&mut, NULL);
    pthread_t read_threads[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) 
    {
        check = pthread_create(&read_threads[i], NULL, readCycle, NULL);
        if (check != 0) 
        {
            fprintf(stderr, "pthread create error");
            return 1;
        }
    }

    pthread_t write_thread;
    check = pthread_create(&write_thread, NULL, writeCycle, NULL);
    if (check != 0) 
    {
        fprintf(stderr, "pthread create error");
        return 1;
    }
    for (int i = 0; i < THREAD_NUM; i++) 
    {
		void* status = NULL;
		check = pthread_join(read_threads[i], &status);
		if (check != 0) 
        {
			fprintf(stderr, "pthread join error");
            return 1;
		}
	}

	pthread_join(write_thread, NULL);
    pthread_mutex_destroy(&mut);
    return 0;
}