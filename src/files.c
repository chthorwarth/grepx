// Created by Christian Thorwarth on 21.12.25.
// -> Functions to find files in folders

// List<Filenames> searchForFilesWithEnding(String ending);
// List<Filenames> searchForFiles();
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

// files.c
#include "files.h"

void traverse(char *path)
{
    DIR *directory;
    directory = opendir(path);

    struct dirent *direntry;

    if (directory == NULL)
    {
        return;
    }
    printf("%s", "Hello Word2\n");
    while ((direntry = readdir(directory)) != NULL)
    {
        if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0)
        {
            printf("%s\n",(*direntry).d_name);
            traverse(direntry->d_name);
        }
    }


    closedir(directory);
}

char *getContent(char *path) {
    return "asdfölasödjlf\nHello World\n";
}