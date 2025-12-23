#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "grep_options.h"

/* Hilfsfunktion: String zu dynamischem Array hinzufügen */
static void add_string(char ***array, int *count, const char *value) {
    *array = realloc(*array, sizeof(char *) * (*count + 1));
    (*array)[*count] = strdup(value);
    (*count)++;
}

int main(int argc, char *argv[]) {

    // Stdiin
    /*
    char buf[1024];

    while (fgets(buf, sizeof(buf), stdin)) {
        printf("%s", buf);
    }
    */
    // end Stdin

    grep_options_t opts = {0};

    int opt;
    while ((opt = getopt(argc, argv, "ivnclqrC:e:f:")) != -1) {
        switch (opt) {
            case 'i':
                opts.ignore_case = true;
                break;
            case 'v':
                opts.invert_match = true;
                break;
            case 'n':
                opts.show_line_number = true;
                break;
            case 'c':
                opts.count_only = true;
                break;
            case 'l':
                opts.list_files = true;
                break;
            case 'q':
                opts.quiet = true;
                break;
            case 'r':
                opts.recursive = true;
                break;
            case 'C':
                opts.context = atoi(optarg);
                break;
            case 'e':
                add_string(&opts.patterns, &opts.pattern_count, optarg);
                break;
            case 'f':
                // Pattern-Datei merken (Inhalt später einlesen)
                add_string(&opts.patterns, &opts.pattern_count, optarg);
                break;
            default:
                // Noch keine Fehlerbehandlung
                break;
        }
    }

    /* Positionsargumente auswerten */
    if (opts.pattern_count == 0 && optind < argc) {
        // Erstes freies Argument ist das Pattern
        add_string(&opts.patterns, &opts.pattern_count, argv[optind]);
        optind++;
    }

    // Restliche Argumente sind Dateien / Pfade
    while (optind < argc) {
        add_string(&opts.paths, &opts.path_count, argv[optind]);
        optind++;
    }

    /* Debug-Ausgabe (nur zum Testen) */
    printf("OPTIONS:\n");
    printf("  ignore_case: %d\n", opts.ignore_case);
    printf("  invert_match: %d\n", opts.invert_match);
    printf("  show_line_number: %d\n", opts.show_line_number);
    printf("  count_only: %d\n", opts.count_only);
    printf("  list_files: %d\n", opts.list_files);
    printf("  quiet: %d\n", opts.quiet);
    printf("  recursive: %d\n", opts.recursive);
    printf("  context: %d\n", opts.context);

    printf("\nPATTERNS:\n");
    for (int i = 0; i < opts.pattern_count; i++) {
        printf("  %s\n", opts.patterns[i]);
    }

    printf("\nPATHS:\n");
    for (int i = 0; i < opts.path_count; i++) {
        printf("  %s\n", opts.paths[i]);
    }

    // 2. Durchsuche bla.txt nach "Hallo"

    printf("Hello World\n");
    return 0;
}