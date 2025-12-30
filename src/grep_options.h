#ifndef GREP_OPTIONS_H
#define GREP_OPTIONS_H

#include <stdbool.h>

typedef struct {
    bool ignore_case;      // -i
    bool invert_match;     // -v
    bool show_line_number; // -n
    bool count_only;       // -c
    bool list_files;       // -l
    bool quiet;            // -q
    bool recursive;        // -r

    int context;           // -C <n>

    // Pattern handling
    char **patterns;       // -e -f (later) oder Positions-PATTERN
    int pattern_count;

    char *pattern_filename; // -f

    // Files / paths
    char **paths;          // file1 file2 dir1 ...
    int path_count;
} grep_options_t;

// TODO
void grep_options_init(grep_options_t *opts);
// TODO
void grep_options_free(grep_options_t *opts);
void add_string(char ***array, int *count, const char *value);
void grep_options_print(grep_options_t *opts);

#endif