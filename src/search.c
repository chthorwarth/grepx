#define _GNU_SOURCE     // for strcasestr
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grep_options.h"
#include "search.h"

int searchInFile(const char *filename, grep_options_t *opts) {
FILE *file = fopen(filename, "r");
    
    if (!file) {
        //Only print error if NOT in quiet mode
        if (!opts->quiet) {
            perror(filename);
        }
        return EXIT_FAILURE;
    }

    char line[1024];
    int line_nr = 1;
    int match_count = 0;
    bool found_any = false;

    while (fgets(line, sizeof(line), file)) {
        bool match = false;
        
        for (int i = 0; i < opts->pattern_count; i++) {
            if (opts->ignore_case) {
                if (strcasestr(line, opts->patterns[i]) != NULL) {          // Case-insensitive substring search(GNU extension)
                    match = true;
                    break;
                }
            } else {
                if (strstr(line, opts->patterns[i]) != NULL) {
                    match = true;
                    break;
                }
            }
        }

        if (match != opts->invert_match) {      // Output line if it matches the search criteria (considering -v for inverted results)
            found_any = true;
            
            if (opts->quiet) {
                fclose(file);                   // In quiet mode: exit immediately without printing anything
                return EXIT_SUCCESS;
            }

            if (opts->list_files) {
                printf("%s\n", filename);       //print filename and stop searching this file immediately for -l option
                fclose(file);
                return EXIT_SUCCESS;
            }

            match_count++;
            if (opts->count_only) {

                if (opts->show_line_number) {
                    printf("%d:", line_nr);         // If -n is set, prefix the output with the current line number
                }
                printf("%s", line);
                }
        }
        line_nr++;
    }

    if (opts->count_only) {
        printf("%d\n", match_count);        // print the total match_count if -c is set
    }

    fclose(file);
    return found_any ? EXIT_SUCCESS : EXIT_FAILURE; // Return 0 (Success) if found, 1 (Failure) if not
}