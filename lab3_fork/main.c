#include "sgfn.h"

int main()
{
    pid_t pid;

    if (atexit(on_exit_handler) != 0)
    {
        fprintf(stderr, "atexit error");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        fprintf(stderr, "SIGINT handler error\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_sigaction = sigterm_handler;
    sa.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        fprintf(stderr, "SIGTERM handler error");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    int sleep_time = 5;

    if (pid < 0)
    {
        fprintf(stderr, "fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("CHILD,\tPID: %d\n\tSleeping for %d seconds\n", getpid(), sleep_time);
        sleep(sleep_time);
        printf("\tEnd of sleep.\n");
    }
    else
    {
        int status;
        printf("PARENT,\tPID: %d\n\tChild pid=%d\n", getpid(), pid);
        pid_t terminated_pid = wait(&status);
        printf("Wait finished.\n");
        printf("Child process pid=%d exited with status %d\n", terminated_pid, status);
    }

    return 0;
}