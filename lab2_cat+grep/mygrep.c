// this is the 'main' file for the grep-command realization

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <stdbool.h>

#include <string.h>
#include <regex.h>

#define COL_RED "\033[1;31m"
#define COL_RES "\033[0m"

void grep_print(const char * line, regex_t * reg) 
{
    const char * sl = line;
    regmatch_t match;
    bool found = false;

    while (regexec(reg, sl, 1, &match, 0) == 0) 
    {
	    found = true;
	    if (match.rm_eo - match.rm_so == 0)
		    break;
        fwrite(sl, 1, match.rm_so, stdout);
        fwrite(COL_RED, 1, strlen(COL_RED), stdout);
        fwrite(sl + match.rm_so, 1, match.rm_eo - match.rm_so, stdout);
        fwrite(COL_RES, 1, strlen(COL_RES), stdout);
        sl += match.rm_eo;
    }

    if (found)
	    fputs(sl, stdout);
}

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Not enough args. Use 'grep --help'\n");
        exit(EXIT_FAILURE);
    }

    regex_t reg;
    
    if (regcomp(&reg, argv[1], REG_EXTENDED) != 0)
        return 1;

    int i = 0;

    if (argc == 2) 
    {
        char * tmp = NULL;
        size_t len = 0;
        while (getline(&tmp, &len, stdin) != -1) 
            grep_print(tmp, &reg);
        
    }
    else 
       for (i = 2; i < argc; i++) 
        {
           FILE * f = fopen(argv[i], "r");
           if (f == NULL) {
               fprintf(stderr, "error in %s\n", argv[i]);
               continue;
           }
           char * tmp = NULL;
           size_t len = 0;
           while (getline(&tmp, &len, f) != -1) {
               grep_print(tmp, &reg);
           }
           fclose(f);
       }
    
    regfree(&reg);

    return 0;

}
