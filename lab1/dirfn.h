#include <stdio.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <pwd.h>
#include <grp.h>

#include <time.h>

#include <memory.h>

#include <math.h>

#define COL_BG_GREEN "\x1b[42m"
#define COL_BLUE "\x1b[34m"
#define COL_GREEN "\x1b[32m"
#define COL_CYAN "\x1b[36m"
#define COL_RED "\x1b[31m"
#define COL_RESET "\x1b[0m"

#define BOLD_ON  "\e[1m"
#define BOLD_OFF   "\e[m"

typedef long int lint;

struct prm_lens
{
    int lks_len;
    int own_len;
    int grp_len;
    int s_len;
};

#ifndef DIR_FN
#define DIR_FN

void sort_entries(char ** entries, const int num_of_entries);

void print_entry(const char * path, const char * entry_name, bool long_form,
    struct prm_lens * lns);

void list_dir(const char * path, bool long_form, bool all);

#endif