#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>

#include <unistd.h>
#include <sys/syscall.h>

#include <sys/types.h>

#define THREAD_NUM 10
#define SIZE 10

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

int cur_idx = 0;
int nums[SIZE];

int tempo = 400;

void sleep_ms(int ms)
{
    usleep(ms * 1000);
}

void* Read(void* arg) 
{
    (void)arg;
    char* result_str = (char*)calloc(1, 256);
    while (cur_idx < SIZE) 
    {
        sleep_ms(tempo); 
        pthread_rwlock_rdlock(&rwlock);
        sprintf(result_str, "reading -\ttid: %li\tdata: [ ", pthread_self()); 
        char* cur_state_str = (char*)calloc(1, 256);
        for (int i = 0; i < SIZE; i++) {
            sprintf (cur_state_str, "%d ", nums[i]);
            strcat(result_str, cur_state_str);
        }
        strcat(result_str, "]\n");
        printf("%s", result_str);
        free(cur_state_str);
        pthread_rwlock_unlock(&rwlock);
    } 
    free(result_str);
    pthread_exit(NULL);
}

void* Write(void* arg) {
    (void)arg;
    for (int i = 0; i < SIZE; i++) {
        pthread_rwlock_wrlock(&rwlock);
        sleep_ms(tempo); 
        printf("\nwriting\n\n");
        cur_idx++;
		nums[i] = i + 1;
		pthread_rwlock_unlock(&rwlock);
    }
    pthread_exit(NULL);
}

int main() {

    int check;
    pthread_t read_threads[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) 
    {
        check = pthread_create(&read_threads[i], NULL, Read, NULL);
        if (check != 0) 
        {
            fprintf(stderr, "pthread create error");
            return 1;
        }
    }

    pthread_t write_thread;
    check = pthread_create(&write_thread, NULL, Write, NULL);
    if (check != 0) 
    {
        fprintf(stderr, "pthread create error");
        return 1;
    }
    
	pthread_join(write_thread, NULL);
    for (int i = 0; i < THREAD_NUM; i++) 
    {
		void* status = NULL;
		check = pthread_join(read_threads[i], &status);
		if (check != 0) {
			fprintf(stderr, "pthread join error");
            return 1;
		}
	}
    return 0;
}