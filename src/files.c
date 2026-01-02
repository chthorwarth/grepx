// Created by Christian Thorwarth on 21.12.25.
// -> Functions to find files in folders

// List<Filenames> searchForFilesWithEnding(String ending);
// List<Filenames> searchForFiles();
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

#include "files.h"

int exploreDirectories(const char *path, Queue *q)
{
    DIR *directory = opendir(path);

    if (directory == NULL)
    {
        //perror("opendir");
        return EXIT_FAILURE;
    }

    struct dirent *dirEntry;

    while ((dirEntry = readdir(directory)) != NULL)
    {
        if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0)
            continue;

        size_t len = strlen(path) + 1 + strlen(dirEntry->d_name) + 1;
        char *fullPath = malloc(len);
        if (!fullPath)
        {
            closedir(directory);
            return EXIT_FAILURE;
        }
        snprintf(fullPath, len, "%s/%s", path, dirEntry->d_name);

        enqueue(q, fullPath);
        exploreDirectories(fullPath, q);
    }

    closedir(directory);
    return EXIT_SUCCESS;
}