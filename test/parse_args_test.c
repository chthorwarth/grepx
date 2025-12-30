#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../src/parse_args.h"
#include "../src/grep_options.h"

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

bool test_basic(void)
{
    char *argv[] = {
        "grepx",
        "-i",
        "-C", "3",
        "-e", "foo",
        "file1.txt",
        "file2.txt"
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    grep_options_t actual = {0};

    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    /* Erwartete Werte */
    grep_options_t expected = {0};

    expected.ignore_case = true;
    expected.context = 3;


    add_string(&expected.patterns, &expected.pattern_count, "foo");

    add_string(&expected.paths, &expected.path_count, "file1.txt");
    add_string(&expected.paths, &expected.path_count, "file2.txt");

    //grep_options_print(&actual);
    //printf("\n");
    //grep_options_print(&expected);

    bool ok = options_equal(&actual, &expected);

    return ok;
}


bool test_implicit(void)
{
    char *argv[] = {
        "grepx",
        "pattern",
        "file1.txt"
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    grep_options_t actual = {0};

    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    grep_options_t expected = {0};

    add_string(&expected.patterns, &expected.pattern_count, "pattern");
    add_string(&expected.paths, &expected.path_count, "file1.txt");

    bool ok = options_equal(&actual, &expected);

    return ok;
}

bool test_only_pattern(void)
{
    char *argv[] = {
        "grepx",
        "pattern",
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    grep_options_t actual = {0};

    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    grep_options_t expected = {0};

    add_string(&expected.patterns, &expected.pattern_count, "pattern");

    grep_options_print(&actual);
    printf("\n");
    grep_options_print(&expected);

    bool ok = options_equal(&actual, &expected);

    return ok;
}


bool test_no_args(void)
{
    char *argv[] = {
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    grep_options_t actual = {0};

    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    grep_options_t expected = {0};

    bool ok = options_equal(&actual, &expected);

    return ok;
}


bool test_all_flags(void)
{
    char *argv[] = {
        "grepx",
        "-ivnclqr",
        "-C","4",
        "-e","foo",
        "-e","bar",
        "f1","f2"
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    grep_options_t actual = {0};
    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    grep_options_t expected = {0};
    expected.ignore_case = true;
    expected.invert_match = true;
    expected.show_line_number = true;
    expected.count_only = true;
    expected.list_files = true;
    expected.quiet = true;
    expected.recursive = true;
    expected.context = 4;

    add_string(&expected.patterns,&expected.pattern_count,"foo");
    add_string(&expected.patterns,&expected.pattern_count,"bar");

    add_string(&expected.paths,&expected.path_count,"f1");
    add_string(&expected.paths,&expected.path_count,"f2");

    return options_equal(&actual,&expected);
}

int main(void)
{
    int passed = 0;
    int total = 5;

    if (test_basic()) passed++; else printf("test_basic() failed\n");
    if (test_all_flags()) passed++; else printf("test_all_flags() failed\n");
    if (test_no_args()) passed++; else printf("test_no_args() failed\n");
    if (test_implicit()) passed++; else printf("test_implicit() failed\n");
    if (test_only_pattern()) passed++; else printf("test_only_pattern() failed\n");

    printf("\nSummary: %d / %d tests passed\n", passed, total);
    return passed == total ? 0 : 1;
}