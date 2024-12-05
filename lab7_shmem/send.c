#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>

#include <signal.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/ipc.h>

#include <sys/shm.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>

#define SHMEM_SIZE 64
#define SHMEM_FILE "sharedfile322"

char *addr = NULL;
int shmid = -1;

void handle_signal(int sig)
{
    if (addr != NULL)
    {
        if (shmdt(addr) < 0)
        {
            fprintf(stderr, "critical shmdt error\n");
            exit(1);
        }
    }

    if (shmid != -1)
    {
        if (shmctl(shmid, IPC_RMID, NULL) < 0)
        {
            fprintf(stderr, "critical shmctl error\n");
            exit(1);
        }
    }

    if (remove(SHMEM_FILE) == -1)
    {
        fprintf(stderr, "critical remove error\n");
        exit(1);
    }

    exit(0);
}

int main()
{
    int fd = open(SHMEM_FILE, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        fprintf(stderr, "shmem file opening error\n");
        return 1;
    }
    close(fd);

    key_t key = ftok(SHMEM_FILE, 1);

    if (key < 0)
    {
        fprintf(stderr, "ftok error\n");
        return -1;
    }

    shmid = shmget(key, SHMEM_SIZE, 0666 | IPC_CREAT);
    if (shmid < 0)
    {
        fprintf(stderr, "shmget error\n");
        return -1;
    }

    addr = shmat(shmid, NULL, 0);
    if (addr == (void *)-1)
    {
        fprintf(stderr, "shmat error\n");
        return -1;
    }

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    while (1)
    {
        sleep(1);

        char timeStr[SHMEM_SIZE] = {0};
        char result[2 * SHMEM_SIZE] = {0};

        time_t ts = time(NULL);
        struct tm current_time = *localtime(&ts);

        strftime(timeStr, sizeof(timeStr), "%H:%M:%S;", &current_time);
        snprintf(result, sizeof(result), "Process: %d; %s", getpid(), timeStr);

        strcpy(addr, result);
    }

    return 0;
}