// Created by Christian Thorwarth on 21.12.25.
// -> Basic grep search functions for one file
//


// boolean searchWord(String word, String content)
// int searchWord(String word, String content)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int searchWord()
{
    FILE *file;
    file = fopen("/Users/christianthorwarth/git/grepx/src/main.c", "r");
    char pattern[] = "Users";
    char input[255];
    long counter = 0;
    long c;
    int line = 1;
    if (file == NULL)
    {
        return EXIT_FAILURE;
    }

    while (fgets(input, 255, file))
    {
        int i = 0;
        counter = 0;
        fputs(input, stdout);
        while (i <= strlen(input))
        {
            c = *(input+i);

            if (counter == strlen(pattern))
            {
                counter = 0;
                printf("Pattern found in line: %d\n",line);
            }
            if(c == pattern[counter])
            {
                counter++;
            }
            else
            {
                counter = 0;
            }
            i++;
        }

        line++;
    }

    fclose(file);
    return 0;
}
