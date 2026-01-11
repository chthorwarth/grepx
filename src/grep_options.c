#include "grep_options.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Helper function: Add string to dynamic array
void add_string(char ***array, int *count, const char *value) {
    *array = realloc(*array, sizeof(char *) * (*count + 1));
    char *dst = malloc(strlen(value) + 1);
    if (dst == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(2);
    }
    strcpy(dst, value);
    *(*array+*count) = dst;
    (*count)++;
}

void grep_options_print(grep_options_t *opts) {
    /* Debug-Ausgabe (nur zum Testen) */
    printf("OPTIONS:\n");
    printf("  ignore_case: %d\n", opts->ignore_case);
    printf("  invert_match: %d\n", opts->invert_match);
    printf("  show_line_number: %d\n", opts->show_line_number);
    printf("  count_only: %d\n", opts->count_only);
    printf("  list_files: %d\n", opts->list_files);
    printf("  quiet: %d\n", opts->quiet);
    printf("  recursive: %d\n", opts->recursive);
    printf("  context: %d\n", opts->context);

    printf("\nPATTERNS:\n");
    for (int i = 0; i < opts->pattern_count; i++) {
        printf("  %s\n", opts->patterns[i]);
    }

    printf("\nPATHS:\n");
    for (int i = 0; i < opts->path_count; i++) {
        printf("  %s\n", opts->paths[i]);
    }
}


bool options_equal(const grep_options_t *a, const grep_options_t *b)
{
    if (a->ignore_case != b->ignore_case) return false;
    if (a->invert_match != b->invert_match) return false;
    if (a->show_line_number != b->show_line_number) return false;
    if (a->count_only != b->count_only) return false;
    if (a->list_files != b->list_files) return false;
    if (a->quiet != b->quiet) return false;
    if (a->recursive != b->recursive) return false;
    if (a->context != b->context) return false;

    if (a->pattern_count != b->pattern_count) return false;
    for (int i = 0; i < a->pattern_count; i++)
        if (strcmp(a->patterns[i], b->patterns[i]) != 0)
            return false;

    if (a->path_count != b->path_count) return false;
    for (int i = 0; i < a->path_count; i++)
        if (strcmp(a->paths[i], b->paths[i]) != 0)
            return false;

    if ((a->pattern_filename == NULL) != (b->pattern_filename == NULL)) return false;
    if (a->pattern_filename && strcmp(a->pattern_filename, b->pattern_filename) != 0)
        return false;
    return true;
}

void grep_options_free(grep_options_t *opts)
{
    if (!opts) return;

    // Free patterns
    if (opts->patterns) {
        for (int i = 0; i < opts->pattern_count; i++) {
            free(opts->patterns[i]);
        }
        free(opts->patterns);
    }

    // Free pattern filename
    if (opts->pattern_filename) {
        free(opts->pattern_filename);
    }

    // Free paths
    if (opts->paths) {
        for (int i = 0; i < opts->path_count; i++) {
            free(opts->paths[i]);
        }
        free(opts->paths);
    }
}