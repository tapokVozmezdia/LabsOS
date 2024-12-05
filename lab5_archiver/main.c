#include "archiver.h"

int main(int argc, char * argv[])
{

	if (argc == 1)
	{
		printf("\n-h (--help)\n\tprint help for working with archiver\n");
		return 0;
	}

    if (argc == 2) 
	{
		if ((strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) 
		{
			printf("'archive_name' -i (--input) 'file_name'\n\tadd file to archive\n");
			printf("\n'archive_name' -e (--extract) 'file_name'\n\tdelete file from archive\n");
			printf("\n'archive_name' -s (--stat)\n\tprint out archive status\n");
			printf("\n-h (--help)\n\tprint help for working with archiver\n");
			return 0;
		}
		else 
		{
			fprintf(stderr, "invalid args\n");
			return -1;
		}
	}

    Archive * arch = NULL;

    if (argc == 3) 
	{
		if (strcmp(argv[2], "-s") == 0 || strcmp(argv[2], "--stat") == 0) 
		{
			arch = initArchive(argv[1]);
			printStatus(arch);
		}
		else 
		{
			fprintf(stderr, "invalid args\n");
		}
	}
    else if (argc == 4) 
	{
		if (strcmp(argv[2], "-i") == 0 || strcmp(argv[2], "--input") == 0) 
		{
			arch = initArchive(argv[1]);
            inputFile(arch, argv[3]);
		}
		else if(strcmp(argv[2], "-e") == 0 || strcmp(argv[2], "--extract") == 0) 
		{
			arch = initArchive(argv[1]);
			extractFile(arch, argv[3]);
		}
		else 
		{
			fprintf(stderr, "invalid args\n");
		}
	}
	else 
	{
		fprintf(stderr, "invalid number of args\n");
	}

	if (arch != NULL)
    	freeArchive(arch);

    return 0;

}