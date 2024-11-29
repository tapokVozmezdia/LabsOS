#include "archiver.h"

const unsigned MAX_BLOCK_SIZE = 512;
const char sep = ';';

File * readFile(char * block) 
{

    File * file = (File *)calloc(1, sizeof(File)); 
 
    char * curFilePos = strchr(block, sep);
    int curSize = strlen(block) - strlen(curFilePos);

    file->name = (char *)calloc(1, curSize + 1);

	strncpy(file->name, block, curSize);
	sscanf(curFilePos + 1, "%u", &file->mode); 

	curFilePos = strchr(curFilePos + 1, sep);
	sscanf(curFilePos + 1, "%d", &file->uid);

	curFilePos = strchr(curFilePos + 1, sep);
	sscanf(curFilePos + 1, "%d", &file->gid);

	curFilePos = strchr(curFilePos + 1, sep);
	sscanf(curFilePos + 1, "%ld", &file->m_time);

	curFilePos = strchr(curFilePos + 1, sep);
	sscanf(curFilePos + 1, "%ld", &file->size);

	file->content = (char *)calloc(1, file->size + 1);

    return file;

}

int addFileToArchive(Archive * arch, File * file) 
{

    if (arch->files == NULL) 
    {
		arch->files = (File **)calloc(2, sizeof(File *));
		arch->capacity = 2;
	}

    for (int i = 0; i < arch->size; ++i)
    {
        // printf("%s\t%s\n", file->name, arch->files[i]->name);
        // printf("%d\t%d\n", strlen(file->name), strlen(arch->files[i]->name));

        if (strcmp(arch->files[i]->name, file->name) == 0)
        {
            printf("File with the name '%s' already exists in the archive, no changes done\n", file->name);
            return -1;
        }
    }

	if (arch->size + 1 >= arch->capacity) 
    {
		arch->capacity *= 2;
		arch->files = (File **)realloc(arch->files, arch->capacity * sizeof(File *));
	}

	arch->files[arch->size] = file;
    arch->size++;

    return 0;
}

Archive * initArchive(string arch_name) 
{

    Archive * arch = (Archive *)calloc(1, sizeof(Archive));
    arch->name=(char *)calloc(1, strlen(arch_name) + 1);

    strcpy(arch->name, arch_name);

    arch->size = 0;
    arch->capacity = 0;
    arch->files = NULL;

    int fd = open(arch->name, O_RDWR);

    arch->foiled = false;

    if (fd == -1) 
    {
        close(fd);
        printf("Archive does not exist - '%s'\n", arch_name);
        printf("Create new archive? (y/N): ");

        int c = getchar();

        // printf("input - %c\nint - %d\n", c, (int)(c));

        if (c == 'Y' || c == 'y')
        {
            int nfd = open(arch->name, O_RDWR|O_CREAT, 0666);
            close(nfd);
            printf("New archive was created - '%s'\n", arch_name);
        }
        else
        {
            arch->foiled = true;
        }

        return arch;
    }

    int archSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, 0);
    char * block = (char *)calloc(1, MAX_BLOCK_SIZE + 1);

    while(lseek(fd, 0, SEEK_CUR) != archSize) 
    {
        read(fd, block, MAX_BLOCK_SIZE);
        File* file = readFile(block);
        read(fd, file->content, file->size);
        addFileToArchive(arch, file);
    }

    free(block);
    close(fd);

    return arch;

}

void printArchiveStatus(Archive * arch) 
{
    if (arch->foiled)
        return;

    printf("Archive name: %s\nNumber of files: %ld\n--------------\n", arch->name, arch->size);
    for (int i = 0; i < arch->size; i++) 
    {
        File * file = arch->files[i];
        printf("Filename: %s\nmode: %u, uid: %d, gid: %d, size: %ld\n", file->name, file->mode, file->uid, file->gid, file->size);
        // printf("%s\n", file->content);
    }

}

File* initFile(string filename) 
{

    int fd = open(filename, O_RDONLY);

	if(fd == -1) 
    {
		fprintf(stderr, "file opening error\n");
		return NULL;
	}

    File * file = (File*)calloc(1, sizeof(File));

    struct stat st;
	int answ = stat(filename, &st);

	if(answ == -1) 
    {
		fprintf(stderr, "stat critical error\n");
		return NULL;
	}

    file->size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, 0);

    file->name = (char *)calloc(1, sizeof(filename) + 1);

    strcpy(file->name, filename);

    file->mode = st.st_mode;
	file->uid = st.st_uid;
	file->gid = st.st_gid;
	file->m_time = st.st_mtime;
    file->content = (char *)calloc(1, file->size + 1);

    read(fd, file->content, file->size);

    close(fd);

    return file;

}

void saveArchive(Archive * arch) 
{

    int fd = open(arch->name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) 
    {
        fprintf(stderr, "Error in saving archive\n");
    }

    for (int i = 0; i < arch->size; i++) 
    {
        char * fileBlock = (char *)calloc(1, MAX_BLOCK_SIZE + 1);
		File * file = arch->files[i];

		int infoLineSize = sprintf(fileBlock, "%s;%u;%d;%d;%ld;%ld", file->name, file->mode, file->uid, file->gid, file->m_time, file->size);	
		for (int i = infoLineSize; i < MAX_BLOCK_SIZE; i++) 
            fileBlock[i] = '?';

		write(fd, fileBlock, MAX_BLOCK_SIZE);
		write(fd, file->content, file->size);	

		free(fileBlock);

    }
    close(fd);

}

void inputFile(Archive * arch, string filename) 
{

    File * file = initFile(filename);

    if (file == NULL) 
        return;

    int res = addFileToArchive(arch, file);
    saveArchive(arch);
    if (res == 0)
        printf("file successfully added: '%s'\n", filename);
}

void extractFile(Archive * arch, string filename) 
{

    for (int i = 0; i < arch->size; i++) 
    {
        if (strcmp(arch->files[i]->name, filename) == 0) 
        {
            File* file = arch->files[i];
            int fd = creat(filename, file->mode);
            if (fd == -1) 
            {
                fprintf(stderr, "Error in creating extracted file %s\n", filename);
                close(fd);
                return;
            }

            fchmod(fd, file->mode);
            write(fd, file->content, file->size);
            close(fd);

            struct utimbuf new_time;
            new_time.modtime = file->m_time;
            utime(filename, &new_time);

            freeFileMemory(arch, arch->files[i]);
            
            if (i != arch->size - 1) 
            {
                arch->files[i] = arch->files[arch->size - 1];
            }

            arch->size--;
            saveArchive(arch);

            printf("file successfully extracted: '%s'\n", filename);
            return;
        }
    }
    printf("file extracting error: '%s' not found in archive\n", filename);

}

void freeFileMemory(Archive * arch, File * file) 
{

    if (file->name != NULL) 
        free(file->name);
	if (file->content != NULL) 
        free(file->content);

	free(file);

}

void freeArchiveMemory(Archive * arch) 
{

    for (int i = 0; i < arch->size; i++) 
    {
        if (arch->files[i] != NULL) 
            freeFileMemory(arch, arch->files[i]);
    }

    if (arch->files != NULL) 
        free(arch->files);
    if (arch->name != NULL) 
        free(arch->name);

    free(arch); 

}