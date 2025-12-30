//
// Created by Christian Thorwarth on 24.12.25.
//


#include <stdio.h>
#include "grep_options.h"

int validate(grep_options_t *opts, char *context_endptr) {
    /* Validate -C argument */
    if (context_endptr != NULL && *context_endptr != '\0') {
        fprintf(stderr, "Invalid value for -C (context): not a number\n");
        return -1;
    }

    if (opts->context < 0) {
        fprintf(stderr, "Context (-C) must not be negative\n");
        return -1;
    }

    /* Conflicting options */
    if (opts->quiet && opts->count_only) {
        fprintf(stderr, "Options -q and -c cannot be used together\n");
        return -1;
    }

    if (opts->quiet && opts->list_files) {
        fprintf(stderr, "Options -q and -l cannot be used together\n");
        return -1;
    }

    if (opts->quiet && opts->show_line_number) {
        fprintf(stderr, "Options -q and -n cannot be used together\n");
        return -1;
    }

    if (opts->count_only && opts->show_line_number) {
        fprintf(stderr, "Options -c and -n cannot be used together\n");
        return -1;
    }

    /* Pattern validation */
    if (opts->pattern_count == 0 && opts->pattern_filename == NULL) {
        fprintf(stderr, "No search pattern specified\n");
        return -1;
    }

    /* Path validation */
    if (opts->path_count == 0 && !opts->recursive) {
        /* grep defaults to stdin if no path is given,
           so this may be acceptable depending on spec */
        /* Uncomment if you want to enforce at least one path */
        /*
        fprintf(stderr, "No input files specified\n");
        return -1;
        */
    }
}
