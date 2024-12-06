#include "chmfn.h"

int main(int argc, char * argv[]) 
{
    if (argc < 3) 
    {
        fprintf(stderr, "not anough args\n");
        return 1;
    }

    char * rights = argv[optind++];
    char * filepath = argv[optind];
    
    setNewRights(filepath, rights);

    return 0;
}