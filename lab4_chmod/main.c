#include "chmfn.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "invalid number of args\n");
        exit(EXIT_FAILURE);
    }

    return customChmod(argv[1], argv[2]);
}