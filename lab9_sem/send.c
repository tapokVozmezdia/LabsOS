#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/ipc.h>

#include <sys/shm.h>
#include <sys/sem.h>

#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <errno.h>

#include <fcntl.h>

#define SHMEM_SIZE 64
#define SHMEM_FILE "sharedfile322"

int sem_id = -1;
struct sembuf l = {0,-1,0};
struct sembuf o = {0,1,0};

char *addr = NULL;
int shmid = -1;

void handle_signal(int sig)
{
    if (addr != NULL){
        if (shmdt(addr) < 0){
            fprintf(stderr, "critical shmdt error\n");
            exit(1);
        }
    }

    if (shmid != -1){
        if (shmctl(shmid, IPC_RMID, NULL) < 0){
            fprintf(stderr, "critical shmctl error\n");
            exit(1);
        }
    }

    if (sem_id != -1) {
        if (semctl(sem_id, 0, IPC_RMID) != 0) {
            fprintf(stderr, "critical semctl error\n");
            exit(1);
        }
    }

    if (remove(SHMEM_FILE) == -1){
        fprintf(stderr, "critical remove error\n");
        exit(1);
    }

    exit(0);
}

int main(int argc, char **argv)
{
    (void)argc, (void)argv;

    int fd = open(SHMEM_FILE, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1){
        fprintf(stderr, "critical open error\n");
        return 1;
    }
    close(fd);

    key_t key = ftok(SHMEM_FILE, 1);
    if (key < 0){
        fprintf(stderr, "critical ftok error\n");
        return -1;
    }

    shmid = shmget(key, SHMEM_SIZE, 0666 | IPC_CREAT);
    if (shmid < 0){
        fprintf(stderr, "critical shmget error\n");
        return -1;
    }

    sem_id = semget(key, 1, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        fprintf(stderr, "semget\n");
        return 1;
    }

    semop(sem_id, &o, 1);

    addr = shmat(shmid, NULL, 0);
    if (addr == (void *)-1){
        fprintf(stderr, "critical shmat\n");
        return -1;
    }

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    while (1){
        char timeStr[SHMEM_SIZE] = {0};
        char result[2 * SHMEM_SIZE] = {0};
        time_t ts = time(NULL);
        struct tm curTime = *localtime(&ts);
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S;", &curTime);
        snprintf(result, sizeof(result), "pid=%d; %s", getpid(), timeStr);
        semop(sem_id, &l, 1);
        strcpy(addr, result);
        sleep(1);
        semop(sem_id, &o, 1);
    }

    return 0;
}