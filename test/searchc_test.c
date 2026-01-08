#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "test_header.h"
#include "../src/grep_options.h"
#include "../src/search.h"

static int saved_stdout = -1;

/* ---------- stdout helpers ---------- */

FILE *redirect_stdout_to_temp(const char *tmpname) {
    saved_stdout = dup(STDOUT_FILENO);

    FILE *tmp = fopen(tmpname, "w+");
    assert(tmp);

    fflush(stdout);
    dup2(fileno(tmp), STDOUT_FILENO);

    return tmp;
}

void restore_stdout(void) {
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
}

/* ---------- cleanup ---------- */

void cleanup_test(FILE *tmp, const char *file1, const char *file2) {
    fclose(tmp);
    restore_stdout();

    if (file1) remove(file1);
    if (file2) remove(file2);
}

/* ---------- output helpers ---------- */

char *read_all(FILE *tmp, char *buf, size_t size) {
    fflush(tmp);
    fseek(tmp, 0, SEEK_SET);

    size_t n = fread(buf, 1, size - 1, tmp);
    buf[n] = '\0';

    return (n > 0) ? buf : NULL;
}

void assert_output_empty(FILE *tmp) {
    char buf[512];
    char *ret = read_all(tmp, buf, sizeof(buf));
    assert(ret == NULL);
}

void assert_output_equals(FILE *tmp, const char *expected) {
    char buf[1024];
    char *ret = read_all(tmp, buf, sizeof(buf));
    assert(ret != NULL);
    assert(strcmp(buf, expected) == 0);
}

/* ---------- file helper ---------- */

char *write_temp_file(const char *name, const char *content) {
    FILE *f = fopen(name, "w");
    assert(f && "could not create temp test file");
    fputs(content, f);
    fclose(f);
    return (char *) name;
}

/* ---------- TESTS ---------- */

int test_basic(void) {
    write_temp_file("t1.txt",
        "Hallo Welt\n"
        "Hausbau ist toll\n");

    FILE *tmp = redirect_stdout_to_temp("out1.txt");

    grep_options_t opts = {0};
    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("t1.txt", &opts) != EXIT_SUCCESS) return 1;
    assert_output_equals(tmp, "Hausbau ist toll\n");

    cleanup_test(tmp, "t1.txt", "out1.txt");
    return 0;
}

int test_nomatch(void) {
    write_temp_file("t2.txt", "eins\nzwei\n");
    FILE *tmp = redirect_stdout_to_temp("out2.txt");

    grep_options_t opts = {0};
    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("t2.txt", &opts) != EXIT_FAILURE) return 1;
    assert_output_empty(tmp);

    cleanup_test(tmp, "t2.txt", "out2.txt");
    return 0;
}

int test_ignore_case(void) {
    write_temp_file("t3.txt", "hausbau\n");
    FILE *tmp = redirect_stdout_to_temp("out3.txt");

    grep_options_t opts = {0};
    opts.ignore_case = true;

    char *patterns[] = {"HAUS"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("t3.txt", &opts) != EXIT_SUCCESS) return 1;
    assert_output_equals(tmp, "hausbau\n");

    cleanup_test(tmp, "t3.txt", "out3.txt");
    return 0;
}

int test_invert_match(void) {
    write_temp_file("t4.txt", "Haus\nAuto\n");
    FILE *tmp = redirect_stdout_to_temp("out4.txt");

    grep_options_t opts = {0};
    opts.invert_match = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("t4.txt", &opts) != EXIT_SUCCESS) return 1;
    assert_output_equals(tmp, "Auto\n");

    cleanup_test(tmp, "t4.txt", "out4.txt");
    return 0;
}

int test_quiet_mode(void) {
    write_temp_file("t5.txt", "Haus\n");
    FILE *tmp = redirect_stdout_to_temp("out5.txt");

    grep_options_t opts = {0};
    opts.quiet = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("t5.txt", &opts) != EXIT_SUCCESS) return 1;
    assert_output_empty(tmp);

    cleanup_test(tmp, "t5.txt", "out5.txt");
    return 0;
}

int test_list_files(void) {
    write_temp_file("t6.txt", "Haus\n");
    FILE *tmp = redirect_stdout_to_temp("out6.txt");

    grep_options_t opts = {0};
    opts.list_files = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("t6.txt", &opts) != EXIT_SUCCESS) return 1;
    assert_output_equals(tmp, "t6.txt\n");

    cleanup_test(tmp, "t6.txt", "out6.txt");
    return 0;
}

int test_count_only(void) {
    write_temp_file("t7.txt", "Haus\nHausbau\n");
    FILE *tmp = redirect_stdout_to_temp("out7.txt");

    grep_options_t opts = {0};
    opts.count_only = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("t7.txt", &opts) != EXIT_SUCCESS) return 1;
    assert_output_equals(tmp, "2\n");

    cleanup_test(tmp, "t7.txt", "out7.txt");
    return 0;
}

int test_file_not_found(void) {
    FILE *tmp = redirect_stdout_to_temp("out8.txt");

    grep_options_t opts = {0};
    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("does_not_exist.txt", &opts) != EXIT_FAILURE) return 1;
    assert_output_empty(tmp);

    cleanup_test(tmp, NULL, "out8.txt");
    return 0;
}

int test_show_line_number(void) {
    write_temp_file("t_ln.txt",
        "Hallo Welt\n"
        "Hausbau ist toll\n"
        "Noch ein Haus\n"
        "Tschuess\n");

    FILE *tmp = redirect_stdout_to_temp("out_ln.txt");

    grep_options_t opts = {0};
    opts.show_line_number = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    if (searchInFile("t_ln.txt", &opts) != EXIT_SUCCESS) return 1;
    assert_output_equals(tmp,
        "2:Hausbau ist toll\n"
        "3:Noch ein Haus\n");

    cleanup_test(tmp, "t_ln.txt", "out_ln.txt");
    return 0;
}

/* ---------- MAIN ---------- */

int main(void)
{
    printf("\n\n=== search.c Test ===\n\n");

    RUN_TEST(test_basic);
    RUN_TEST(test_nomatch);
    RUN_TEST(test_ignore_case);
    RUN_TEST(test_invert_match);
    RUN_TEST(test_quiet_mode);
    RUN_TEST(test_list_files);
    RUN_TEST(test_count_only);
    RUN_TEST(test_file_not_found);
    RUN_TEST(test_show_line_number);

    printf("\n=== Summary ===\n");
    printf("Total tests:   %d\n", total_tests);
    printf("Failed tests:  %d\n", failed_tests);
    printf("Passed tests:  %d\n", total_tests - failed_tests);
    return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
