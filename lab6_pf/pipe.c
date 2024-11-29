#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <time.h>

int main()
{

    int pipe_fd[2];
    pid_t pid;
    time_t parent_time;
    char message[256];

    if (pipe(pipe_fd) == -1)
    {
        perror("critical error in pipe");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("critical error in fork");
        exit(1);
    }

    if (pid > 0)
    {
        parent_time = time(NULL);
        printf("\nparent pid=%d; time: %s", getpid(), ctime(&parent_time));
        snprintf(message, sizeof(message), "parent pid=%d; time: %s", getpid(), ctime(&parent_time));

        close(pipe_fd[0]);

        sleep(5);
        write(pipe_fd[1], message, strlen(message) + 1);

        close(pipe_fd[1]);
    }
    else
    {

        close(pipe_fd[1]);

        read(pipe_fd[0], message, sizeof(message));
        
        close(pipe_fd[0]);

        time_t child_time = time(NULL);
        printf("child pid=%d; time: %s", getpid(), ctime(&child_time));
        printf("recieved message: %s\n", message);
    }

    return 0;

}