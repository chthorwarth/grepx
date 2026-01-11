#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../src/parse_args.h"

#include <stdlib.h>

#include "../src/grep_options.h"
#include "header_test.h"

int test_basic(void)
{
    char *argv[] = {
        "grepx",
        "-i",
        "-C", "3",
        "-e", "foo",
        "file1.txt",
        "file2.txt"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

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

    return options_equal(&actual, &expected) ? 0 : 1;
}

int test_implicit(void)
{
    char *argv[] = {
        "grepx",
        "pattern",
        "file1.txt"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    grep_options_t actual = {0};

    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    grep_options_t expected = {0};

    add_string(&expected.patterns, &expected.pattern_count, "pattern");
    add_string(&expected.paths, &expected.path_count, "file1.txt");

    return options_equal(&actual, &expected) ? 0 : 1;
}

int test_only_pattern(void)
{
    char *argv[] = {
        "grepx",
        "pattern",
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    grep_options_t actual = {0};

    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    grep_options_t expected = {0};

    add_string(&expected.patterns, &expected.pattern_count, "pattern");

    return options_equal(&actual, &expected) ? 0 : 1;
}

int test_no_args(void)
{
    char *argv[] = { NULL };
    int argc = sizeof(argv) / sizeof(argv[0]);

    grep_options_t actual = {0};

    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    grep_options_t expected = {0};

    return options_equal(&actual, &expected) ? 0 : 1;
}

int test_all_flags(void)
{
    char *argv[] = {
        "grepx",
        "-ivnclqr",
        "-C", "4",
        "-e", "foo",
        "-e", "bar",
        "f1", "f2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    grep_options_t actual = {0};
    char *endptr = NULL;
    parse(&argc, argv, &actual, endptr);

    grep_options_t expected = {0};
    expected.ignore_case      = true;
    expected.invert_match     = true;
    expected.show_line_number = true;
    expected.count_only       = true;
    expected.list_files       = true;
    expected.quiet            = true;
    expected.recursive        = true;
    expected.context          = 4;

    add_string(&expected.patterns, &expected.pattern_count, "foo");
    add_string(&expected.patterns, &expected.pattern_count, "bar");

    add_string(&expected.paths, &expected.path_count, "f1");
    add_string(&expected.paths, &expected.path_count, "f2");

    return options_equal(&actual, &expected) ? 0 : 1;
}

int main(void)
{
    printf("\n\n=== parse_args.c Test ===\n\n");

    RUN_TEST(test_basic);
    RUN_TEST(test_all_flags);
    RUN_TEST(test_no_args);
    RUN_TEST(test_only_pattern);
    RUN_TEST(test_implicit);

    printSummary();

    return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
