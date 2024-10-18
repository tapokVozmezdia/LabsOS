#include "sgfn.h"

void sigint_handler(int signum)
{
    printf("SIGINT: (signal %d). \nExecuted in process pid=%d.\n", signum, getpid());
}

void sigterm_handler(int signum, siginfo_t *info, void *context)
{
    printf("Process pid=%d received SIGTERM (signal %d) from process pid=%d.\n", getpid(), signum, info->si_pid);
}

void on_exit_handler()
{
    printf("Process pid=%d is exiting. Custom exit handler executed.\n", getpid());
}