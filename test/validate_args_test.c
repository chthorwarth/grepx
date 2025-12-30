#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdbool.h>
#include "../src/grep_options.h"
#include "../src/validate_args.h"

bool basic_test(void)
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
    bool ok = (rc == expected);

    return ok;
}

// Plays out if for example: grepx -e
bool conflict_pattern_count_but_no_pattern_test(void)
{
    grep_options_t opts = {0};

    opts.path_count = 1;

    char *endptr = NULL;

    int rc = validate(&opts, endptr);
    int expected = -1;
    bool ok = (rc == expected);

    return ok;
}

// Plays out if for example: grepx -r
bool conflict_recursive_but_file_test(void)
{
    grep_options_t opts = {0};

    opts.recursive = true;

    char *endptr = NULL;

    int rc = validate(&opts, endptr);
    int expected = -1;
    bool ok = (rc == expected);

    return ok;
}

// Plays out if for example: grepx -C abc
bool param_not_a_number_test(void)
{
    grep_options_t opts = {0};

    opts.context = 2;

    char endptr = 'a';
    int rc = validate(&opts, &endptr);
    int expected = -1;
    bool ok = (rc == expected);

    return ok;
}

// Plays out if for example: grepx -C -1
bool negative_arg_test(void)
{
    grep_options_t opts = {0};

    add_string(&opts.patterns, &opts.pattern_count, "foo");
    add_string(&opts.paths, &opts.path_count, "file1.txt");

    opts.context = -1;
    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;
    bool ok = (rc == expected);

    return ok;
}

// Plays out if for example: grepx -q -c
bool conflict_args_q_and_c_together_test(void)
{
    grep_options_t opts = {0};

    opts.quiet=true;
    opts.count_only=true;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;
    bool ok = (rc == expected);

    return ok;
}

// Plays out if for example: grepx -c -n
bool conflict_args_c_and_n_together_test(void)
{
    grep_options_t opts = {0};

    opts.show_line_number=true;
    opts.count_only=true;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;
    bool ok = (rc == expected);

    return ok;
}

// Plays out if for example: grepx -q -l
bool conflict_args_q_and_l_together_test(void)
{
    grep_options_t opts = {0};

    opts.list_files=true;
    opts.quiet=true;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;
    bool ok = (rc == expected);

    return ok;
}

// Plays out if for example: grepx -q -n
bool conflict_args_q_and_n_together_test(void)
{
    grep_options_t opts = {0};

    opts.show_line_number=true;
    opts.quiet=true;

    char *endptr = NULL;
    int rc = validate(&opts, endptr);
    int expected = -1;
    bool ok = (rc == expected);

    return ok;
}


int main(void)
{
    int passed = 0;
    int total  = 8;

    if (basic_test()) passed++; else printf("basic_test() failed\n");
    if (param_not_a_number_test()) passed++; else printf("param_not_a_number_test() failed\n");
    if (negative_arg_test()) passed++; else printf("negative_arg_test() failed\n");
    if (conflict_args_q_and_c_together_test()) passed++; else printf("conflict_args_q_and_c_together_test() failed\n");
    if (conflict_args_c_and_n_together_test()) passed++; else printf("conflict_args_c_and_n_together_test() failed\n");
    if (conflict_args_q_and_l_together_test()) passed++; else printf("conflict_args_q_and_l_together_test() failed\n");
    if (conflict_args_q_and_n_together_test()) passed++; else printf("conflict_args_q_and_n_together_test() failed\n");
    if (conflict_pattern_count_but_no_pattern_test()) passed++; else printf("conflict_pattern_count_but_no_pattern_test() failed\n");
    //if (conflict_recursive_but_file_test()) passed++; else printf("conflict_recursive_but_file_test() failed\n");

    printf("\nSummary: %d / %d tests passed\n", passed, total);

    return passed == total ? 0 : 1;
}