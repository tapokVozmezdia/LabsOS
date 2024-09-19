#include "dirfn.h"

// does not skip a line (no \n)
void print_entry(const char * entry_name, bool long_form)
{

    if (!long_form)
    {
        printf("%s", entry_name);
        return;
    }
    
    struct stat file_stat;

    if (lstat(entry_name, &file_stat) == -1)
    {
        fprintf(stderr, "critical error in lstat\n");
        exit(EXIT_FAILURE);
    }

    char prmns[11];
    const char d_prmns[] = "rwxrwxrwx";
    prmns[0] = S_ISDIR(file_stat.st_mode) ? 'd' : '-';
    int i = 0;
    for (; i < 9; ++i)
        prmns[1 + i] = (file_stat.st_mode & (1 << (8-i))) ? d_prmns[i] : '-';   

    prmns[10] = '\0';    

    printf("%s %d %s", prmns, file_stat.st_nlink, entry_name);
}

void list_dir(const char * path, bool long_form, bool all)
{

    // for debug purposes 
    // printf("I WORK WHILE YOU SLEEP\n");

    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stderr, "failed to open directory\n");
        exit(EXIT_FAILURE);
    }
    struct dirent* d_info = readdir(dir);
    while(d_info != NULL)
    {
    
        //printf("%s\n", d_info->d_name);
        if (!all && d_info->d_name[0] == '.')
        {
            d_info = readdir(dir);
            continue;
        }
        print_entry(d_info->d_name, long_form);
        if (long_form)
            printf("\n");
        else
            printf(" ");
        d_info = readdir(dir);
    
    }
    if (!long_form)
        printf("\n");
    
    if (closedir(dir) == -1)
    {
        fprintf(stderr, "failed to close directory\n");
        exit(EXIT_FAILURE);
    }
}
