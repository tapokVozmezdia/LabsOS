// this is 'main' file for the cat-command realisation

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <stdbool.h>

typedef struct
{
    bool n;
    bool b;
    bool E;    
} CatFlags;

void print_line_E(char * line)
{
    int i = 0;
    for (; i < strlen(line); ++i)
    {
        printf("%c", line[i] == '\n' ? '$' : line[i]);
        if (line[i] == '\n')
            printf("\n");
    }
}

void display_file(FILE * f, CatFlags * fl)
{
    char *line = NULL;
    int line_num = 1;
    size_t len = 0;

    while(getline(&line, &len, f) != -1)
    {

        bool empt = (line[0] == '\n');
        
        if ((fl->n && (!fl->b || !empt))
            || (fl->b && !empt))
        {
            printf("%6d\t", line_num);
            line_num += 1;
        }
        
        if (fl->E)
            print_line_E(line);
        else    
            printf("%s", line);

    }
}

int main(int argc, char * argv[])
{
    
    char c;
    
    CatFlags fl;
    fl.n = false;
    fl.b = false;
    fl.E = false;

    while ((c = getopt(argc, argv, "nbE")) != -1)
    {
        switch(c)
        {
            case 'n':
            {
                fl.n = true;
                break;
            }
            case 'b':
            {
                fl.b = true;
                fl.n = false;
                break;
            }
            case 'E':
            {
                fl.E = true;
                break;
            }
            default:
            {
                fprintf(stderr, "Unknown option.\n\t-n\n\t-b\n\t-E\n");
                exit(EXIT_FAILURE);
            }
        }
    }    

    int i;

    if (optind == argc)
        display_file(stdin, &fl);
    else 
        for (i = optind; i < argc; ++i)
        {
            FILE * f = fopen(argv[i], "r");
            if (f == NULL)  
            {
                fprintf(stderr, "file opening error\n");
                exit(EXIT_FAILURE);
            }
            display_file(f, &fl);
            fclose(f);
        }

    return 0;

}
