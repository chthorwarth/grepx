#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/search.h"
#include "../src/grep_options.h"

int testSearchInFile()
{
    const char *testFileName = "test_search_tmp.txt";
    FILE *f = fopen(testFileName, "w");
    if (!f) return 0;
    fprintf(f, "Line 1\nData Science Match\nLine 3\n");
    fclose(f);

    grep_options_t opts = {0};
    opts.pattern_count = 1;
    opts.patterns = malloc(sizeof(char *));
    opts.patterns[0] = strdup("Data Science");
    opts.path_count = 1;

    FILE *originalStdout = stdout;
    FILE *tmpOut = tmpfile();
    if (!tmpOut) {
        free(opts.patterns[0]);
        free(opts.patterns);
        return 0;
    }
    stdout = tmpOut;

    searchInFile(testFileName, &opts);

    fflush(tmpOut);
    fseek(tmpOut, 0, SEEK_SET);

    char buffer[1024];
    int foundMatch = 0;
    while (fgets(buffer, sizeof(buffer), tmpOut))
    {
        if (strstr(buffer, "Data Science Match"))
            foundMatch = 1;
    }

    fclose(tmpOut);
    stdout = originalStdout;
    remove(testFileName);
    free(opts.patterns[0]);
    free(opts.patterns);

    return foundMatch;
}

int main()
{
    if (testSearchInFile())
        printf("SEARCH TEST SUCCEEDED\n");
    else
        printf("SEARCH TEST FAILED\n");
    return 0;
}