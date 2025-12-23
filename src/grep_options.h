#ifndef GREP_OPTIONS_H
#define GREP_OPTIONS_H

#include <stdbool.h>

// Ich habe den Namen auf den C-Standard (klein mit _t) angepasst
typedef struct {
    bool ignore_case;
    bool invert_match;
    bool show_line_number;
    bool count_only;
    bool list_files;
    bool quiet;
    bool recursive;
    int context;
    char **patterns;
    int pattern_count;
    char **paths;
    int path_count;
} grep_options_t;

#endif