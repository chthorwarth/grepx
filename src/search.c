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
    int print_after_counter = 0;        // Keeps track of how many trailing context lines to print
    bool found_any = false;

    // asign memory for before context lines if -C is set
    char **before_context = NULL;
    if (opts->context > 0) {
        before_context = calloc(opts->context,sizeof(char *));      // calloc is used to initialize all pointers to NULL
    }
    int buffer_index = 0;

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


            if (!opts->count_only) {
                if (opts->recursive || opts->path_count > 1) {
                    printf("%s:", filename);                    //print filename prefix if recursive or multiple paths
                }

                if (before_context) {
                    for (int i = 0; i < opts->context; i++) {
                        int idx = (buffer_index + i) % opts->context;       // Index hops to 0, if the end of the buffer is reached
                        if (before_context[idx] != NULL) {
                            printf("%s", before_context[idx]);              // Print stored context lines
                            free(before_context[idx]);                      // Free memoryafter printing
                            before_context[idx] = NULL;                     // Free Pointer to prevent double free
                        }
                    }
                }

                if (opts->show_line_number) {
                    printf("%d:", line_nr);         // If -n is set, prefix the output with the current line number
                }
                printf("%s", line);
                }
                print_after_counter = opts->context;  
            } 
            // No match case
            else {  
                if (print_after_counter > 0 && !opts->count_only) {
                    printf("%s", line);
                    print_after_counter--;
                } 
                else if (opts->context > 0) {
                    // store line in circular buffer for potential future matches
                    if (before_context[buffer_index]) free(before_context[buffer_index]);   // free old line before overwriting
                    before_context[buffer_index] = strdup(line);                   // duplicate line into buffer
                    buffer_index = (buffer_index + 1) % opts->context;          // advance the ring index
                }
        }
        line_nr++;
    }

    if (before_context) {
        // Final cleanup of any remaining context buffer memory
        for (int i = 0; i < opts->context; i++) {
            if (before_context[i]) free(before_context[i]);
        }
        free(before_context);
    }
    
    fclose(file);
    return found_any ? EXIT_SUCCESS : EXIT_FAILURE; // Return 0 if found, 1 if not
}