#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>



#ifndef SGFN
#define SGFN

    void sigint_handler(int signum);

    void sigterm_handler(int signum, siginfo_t *info, void *context);

    void on_exit_handler();

#endif