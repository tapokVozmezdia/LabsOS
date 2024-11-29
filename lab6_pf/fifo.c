#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <time.h>

int main()
{

    pid_t pid;
    time_t parent_time;
    char message[256];

    if (mkfifo("lab6_fifo", 0666) == -1)
    {
        perror("critical error in fifo");
        exit(1);
    }

    pid = fork();

    if (pid < 0)
    {
        perror("critical error in fork");
        exit(1);
    }
    else if (pid > 0)
    {

        parent_time = time(NULL);
        printf("\nparent pid=%d; time: %s", getpid(), ctime(&parent_time));
        snprintf(message, sizeof(message), "Parent pid=%d; time: %s", getpid(), ctime(&parent_time));

        sleep(5);

        int fd = open("lab6_fifo", O_WRONLY);
        if (fd == -1)
        {
            perror("failed to open fifo for writing");
            exit(1);
        }

        write(fd, message, strlen(message) + 1);

        close(fd);
        unlink("lab6_fifo");
    }
    else
    {
        int fd = open("lab6_fifo", O_RDONLY);
        if (fd == -1)
        {
            perror("failed to open fifo for reading");
            exit(1);
        }

        read(fd, message, sizeof(message));

        time_t child_time = time(NULL);
        printf("child pid=%d; time: %s", getpid(), ctime(&child_time));
        printf("recieved message: %s\n", message);

        close(fd);
    }

    return 0;

}