#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <unistd.h>

#include <string.h>
#include <ctype.h>

#include <fcntl.h>
#include <errno.h>

typedef const char * string;

#ifndef CHMFN
#define CHMFN

    int symMode(string mode_str, mode_t * f_mode);

    int customChmod(string mode_str, string file_name);

#endif