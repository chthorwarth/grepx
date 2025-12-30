#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "grep_options.h"

void parse(int *argc, char *argv[], grep_options_t *opts, char *context_endptr) {
    int opt;
    // getOpt() is a basic c function for parsing arguments
    // getOpt() returns -1 if no (further) argument exists
    // colon behind a character means: this option needs an argument e.g. "grepx -C 2"
    while ((opt = getopt(*argc, argv, "ivnclqrC:e:f:")) != -1) {
        switch (opt) {
            case 'i':
                opts->ignore_case = true;
                break;
            case 'v':
                opts->invert_match = true;
                break;
            case 'n':
                opts->show_line_number = true;
                break;
            case 'c':
                opts->count_only = true;
                break;
            case 'l':
                opts->list_files = true;
                break;
            case 'q':
                opts->quiet = true;
                break;
            case 'r':
                opts->recursive = true;
                break;
            case 'C':
                // optarg is the argument of the parameter
                // strtol converts *char to long
                // context_endptr points to last char that could not be parsed \0 if successful
                opts->context = (int)strtol(optarg, &context_endptr, 10);
                break;
            case 'e':
                add_string(&opts->patterns, &opts->pattern_count, optarg);
                break;
            case 'f':
                // remember pattern file. read later from it.
                opts->pattern_filename = optarg;
                break;
            default:
                // Noch keine Fehlerbehandlung
                break;
        }
    }

    // find implicit position arguments e.g. grepx "foo" file.txt
    // optind is a global index/counter used by getOpt() that points to the actual argument
    // argc is the number of arguments of argv
    if (opts->pattern_count == 0 && optind < *argc) {
        // First argument "foo" is the pattern
        add_string(&opts->patterns, &opts->pattern_count, *(argv+optind));
        optind++;
    }

    // All other arguments (> optind) are files/paths
    while (optind < *argc) {
        add_string(&opts->paths, &opts->path_count, *(argv+optind));
        optind++;
    }
    // reset because of problems when calling method multiple times
    optind = 1;
}