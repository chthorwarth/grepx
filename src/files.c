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

int exploreDirectories(const char *path)
{
    DIR *directory = opendir(path);

    struct dirent *dirEntry;

    if (directory == NULL)
    {
        return EXIT_FAILURE;
    }
    while ((dirEntry = readdir(directory)) != NULL)
    {
        if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
        {
            printf("%s\n", dirEntry->d_name);
            exploreDirectories(dirEntry->d_name);
        }
    }

    closedir(directory);
    return EXIT_SUCCESS;
}