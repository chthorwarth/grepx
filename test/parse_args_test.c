#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../src/parse_args.h"
#include "../src/grep_options.h"

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