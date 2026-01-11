#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../src/grep_options.h"
#include "../src/validate_args.h"
#include "header_test.h"

/* ---------- TESTS ---------- */

int basic_test(void)
{
    grep_options_t opts = {0};

    add_string(&opts.patterns, &opts.pattern_count, "foo");
    add_string(&opts.paths, &opts.path_count, "file1.txt");

    opts.context = 0;
    opts.ignore_case = false;
    opts.quiet = false;
    opts.count_only = false;
    opts.show_line_number = false;

    char *endptr = NULL;

    int rc = validate(&opts, endptr);
    int expected = 0;

    return (rc == expected) ? 0 : 1;
}

// Plays out if for example: grepx -e
int conflict_pattern_count_but_no_pattern_test(void)
{
    grep_options_t opts = {0};

    opts.path_count = 1;

    char *endptr = NULL;

    int rc = validate(&opts, endptr);
    int expected = -1;

    return (rc == expected) ? 0 : 1;
}

// Plays out if for example: grepx -r
int conflict_recursive_but_file_test(void)
{
    grep_options_t opts = {0};

    opts.recursive = true;

    char *endptr = NULL;

    int rc = validate(&opts, endptr);
    int expected = -1;

    return (rc == expected) ? 0 : 1;
}

// Plays out if for example: grepx -C abc
int param_not_a_number_test(void)
{
    grep_options_t opts = {0};

    opts.context = 2;

    char endptr = 'a';
    int rc = validate(&opts, &endptr);
    int expected = -1;

    return (rc == expected) ? 0 : 1;
}

// Plays out if for example: grepx -C -1
int negative_arg_test(void)
{
    grep_options_t opts = {0};

    add_string(&opts.patterns, &opts.pattern_count, "foo");
    add_string(&opts.paths, &opts.path_count, "file1.txt");

    opts.context = -1;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;

    return (rc == expected) ? 0 : 1;
}

// Plays out if for example: grepx -q -c
int conflict_args_q_and_c_together_test(void)
{
    grep_options_t opts = {0};

    opts.quiet = true;
    opts.count_only = true;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;

    return (rc == expected) ? 0 : 1;
}

// Plays out if for example: grepx -c -n
int conflict_args_c_and_n_together_test(void)
{
    grep_options_t opts = {0};

    opts.show_line_number = true;
    opts.count_only = true;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;

    return (rc == expected) ? 0 : 1;
}

// Plays out if for example: grepx -q -l
int conflict_args_q_and_l_together_test(void)
{
    grep_options_t opts = {0};

    opts.list_files = true;
    opts.quiet = true;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;

    return (rc == expected) ? 0 : 1;
}

// Plays out if for example: grepx -q -n
int conflict_args_q_and_n_together_test(void)
{
    grep_options_t opts = {0};

    opts.show_line_number = true;
    opts.quiet = true;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;

    return (rc == expected) ? 0 : 1;
}

/* ---------- MAIN ---------- */

int main(void)
{
    printf("\n\n=== validate_args.c Tests ===\n\n");

    RUN_TEST(basic_test);
    RUN_TEST(param_not_a_number_test);
    RUN_TEST(negative_arg_test);
    RUN_TEST(conflict_args_c_and_n_together_test);
    RUN_TEST(conflict_args_q_and_c_together_test);
    RUN_TEST(conflict_args_q_and_l_together_test);
    RUN_TEST(conflict_args_q_and_l_together_test);
    RUN_TEST(conflict_args_q_and_n_together_test);

    printSummary();

    return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
