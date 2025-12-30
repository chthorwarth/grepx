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