// Since we can't have anything 'unnecessary' in lab dirs
// this is going to be a replacement for a README file

// This is the first lab for Operating Systems subject in uni.

#include "dirfn.h"

int main(int argc, char* argv[])
{
    
    int index;
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

    if (optind >= argc)
        list_dir(".", l, a);
    else
        for (index = optind; index < argc; ++index)
        {
            // TODO
        }   

    return 0;
}
