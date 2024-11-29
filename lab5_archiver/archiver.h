#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>

#include <utime.h>
#include <pwd.h>

#include <stdbool.h>

#ifndef ARCHVR
#define ARCHVR

    typedef const char * string;

    typedef struct {
        char * name;
        mode_t mode;
        int uid;
        int gid;
        time_t m_time;
        unsigned long size;
        char * content;
    } File;

    typedef struct {
        bool foiled;
        char * name;
        unsigned long size;
        unsigned long capacity;
        File ** files;	
    } Archive;

    Archive * initArchive(string arch_name);

    File * initFile(string filename);
    File * readFile(char * block);

    void printArchiveStatus(Archive * arch);

    int addFileToArchive(Archive * arch, File * file);

    void inputFile(Archive * arch, string filename);
    void extractFile(Archive * arch, string filename);

    void freeFileMemory(Archive * arch, File * file);
    void freeArchiveMemory(Archive * arch);

    void saveArchive(Archive * arch);

#endif