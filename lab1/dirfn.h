#include <stdio.h>
#include <stdbool.h>

#include <stdlib.h>

#include <unistd.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifndef DIR_FN
#define DIR_FN


void print_entry(const char * entry_name, bool long_form);

void list_dir(const char * path, bool long_form, bool all);


#endif
