#define _GNU_SOURCE     // for strcasestr
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grep_options.h"
#include "search.h"

static bool line_matches(const char *line, const char *pattern, bool ignore_case) {
    if (ignore_case)
        return strcasestr(line, pattern) != NULL;
    else
        return strstr(line, pattern) != NULL;
}

int searchStream(FILE *stream, const char *filename, grep_options_t *opts) {
    char line[1024];
    int line_nr = 1;
    int match_count = 0;
    bool found_any = false;

    while (fgets(line, sizeof(line), stream)) {
        bool match = false;

        for (int i = 0; i < opts->pattern_count; i++) {
            if (line_matches(line, opts->patterns[i], opts->ignore_case)) {
                match = true;
                break;
            }
        }

        if (opts->invert_match)
            match = !match;

        if (match) {
            found_any = true;
            match_count++;
        }

        if (match && !opts->quiet && !opts->list_files && !opts->count_only) {
            if (opts->show_line_number)
                printf("%d:", line_nr);
            printf("%s", line); // \n not needed because line has already own \n
        }

        if (opts->quiet) {
            return EXIT_SUCCESS; // In quiet mode: exit immediately without printing anything
        }

        if (opts->list_files) {
            printf("%s\n", filename); //print filename and stop searching this file immediately for -l option
            return EXIT_SUCCESS;
        }
        line_nr++;
    }

    if (opts->count_only) {
        printf("%d\n", match_count); // print the total match_count if -c is set
    }

    return found_any ? EXIT_SUCCESS : EXIT_FAILURE; // Return 0 (Success) if found, 1 (Failure) if not
}

int searchInFile(const char *filename, grep_options_t *opts)
{
    FILE *file = fopen(filename, "r");

    if (!file) {
        if (!opts->quiet)
            perror(filename);
        return EXIT_FAILURE;
    }

    int rc = searchStream(file, filename, opts);

    fclose(file);
    return rc;
}
