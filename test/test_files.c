//
// Created by luke on 21.12.25.
//

#include <stdio.h>
#include <stdlib.h>

#include "../src/files.h"
#include <string.h>

int testExploreDirectories()
{
    FILE *originalStdout = stdout;
    FILE *tmp = tmpfile();
    if (!tmp) return 0;

    stdout = tmp;

    int result = exploreDirectories(".");

    fflush(tmp);
    fseek(tmp, 0, SEEK_SET);

    char buffer[1024];
    int foundMain = 0;
    while (fgets(buffer, sizeof(buffer), tmp))
    {
        if (strstr(buffer, "main.c"))
            foundMain = 1;
    }

    fclose(tmp);
    stdout = originalStdout;

    return result == EXIT_SUCCESS && foundMain;
}

int main()
{
    if (testExploreDirectories())
        printf("TEST SUCCEEDED\n");
    else
        printf("TEST FAILED\n");
    return 0;
}
