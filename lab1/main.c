// Since we can't have anything 'unnecessary' in lab dirs
// this is going to be a replacement for a README file

// This is the first lab for Operating Systems subject in uni.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/types.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    
    char c;
    bool l = false, a = false; 

    while ((c = getopt(argc, argv, "la")) != -1)
    {
        switch(c)
        {
            case 'l':
            {
                l = true;
                break;
            }
            case 'a':
            {
                a = true;
                break;
            }
            default:
            {
                fprintf(stderr, "Unknown option.\n\t-l : long format\n\t-a : show hidden\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    

    return 0;
}
