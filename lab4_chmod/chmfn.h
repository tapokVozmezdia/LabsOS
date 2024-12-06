#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

#ifndef CHMFN
#define CHMFN

    int rightsSettings(char* path, char* settings);

    int makeMode(char* rights);

    void setNewRights(char* path, char* rights);

#endif