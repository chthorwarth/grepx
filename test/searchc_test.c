#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

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

void assert_output_all_equals(FILE *tmp, const char *expected) {
    char buf[1024] = {0};

    fflush(tmp);
    fseek(tmp, 0, SEEK_SET);

    fread(buf, 1, sizeof(buf) - 1, tmp);

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

void test_basic() {
    printf("TEST: basic...\n");

    write_temp_file("t1.txt",
                    "Hallo Welt\n"
                    "Hausbau ist toll\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out1.txt");

    grep_options_t opts = {0};
    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t1.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "t1.txt:Hausbau ist toll\n");

    cleanup_test(tmp, "t1.txt", "out1.txt");
    printf("PASS ✓\n");
}

void test_nomatch() {
    printf("TEST: no match...\n");

    write_temp_file("t2.txt",
                    "eins\n"
                    "zwei\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out2.txt");

    grep_options_t opts = {0};
    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t2.txt", &opts);
    assert(rc == EXIT_FAILURE);

    assert_output_empty(tmp);

    cleanup_test(tmp, "t2.txt", "out2.txt");
    printf("PASS ✓\n");
}

void test_ignore_case() {
    printf("TEST: ignore case...\n");

    write_temp_file("t3.txt", "hausbau\n");

    FILE *tmp = redirect_stdout_to_temp("out3.txt");

    grep_options_t opts = {0};
    opts.ignore_case = true;

    char *patterns[] = {"HAUS"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t3.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "t3.txt:hausbau\n");

    cleanup_test(tmp, "t3.txt", "out3.txt");
    printf("PASS ✓\n");
}

void test_invert_match() {
    printf("TEST: invert match...\n");

    write_temp_file("t4.txt",
                    "Haus\n"
                    "Auto\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out4.txt");

    grep_options_t opts = {0};
    opts.invert_match = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t4.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "t4.txt:Auto\n");

    cleanup_test(tmp, "t4.txt", "out4.txt");
    printf("PASS ✓\n");
}

void test_quiet_mode() {
    printf("TEST: quiet mode...\n");

    write_temp_file("t5.txt", "Haus\n");

    FILE *tmp = redirect_stdout_to_temp("out5.txt");

    grep_options_t opts = {0};
    opts.quiet = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t5.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_empty(tmp);

    cleanup_test(tmp, "t5.txt", "out5.txt");
    printf("PASS ✓\n");
}

void test_list_files() {
    printf("TEST: list files...\n");

    write_temp_file("t6.txt", "Haus\n"
        "Katze\n");

    FILE *tmp = redirect_stdout_to_temp("out6.txt");

    grep_options_t opts = {0};
    opts.list_files = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t6.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "t6.txt\n");

    cleanup_test(tmp, "t6.txt", "out6.txt");
    printf("PASS ✓\n");
}

void test_count_only() {
    printf("TEST: count only...\n");

    write_temp_file("t7.txt",
                    "Haus\n"
                    "Hausbau\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out7.txt");

    grep_options_t opts = {0};
    opts.count_only = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t7.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "t7.txt:2\n");

    cleanup_test(tmp, "t7.txt", "out7.txt");
    printf("PASS ✓\n");
}

void test_file_not_found() {
    printf("TEST: file not found...\n");

    FILE *tmp = redirect_stdout_to_temp("out8.txt");

    grep_options_t opts = {0};
    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("does_not_exist.txt", &opts);
    assert(rc == EXIT_FAILURE);

    assert_output_empty(tmp); // stdout MUST be empty

    cleanup_test(tmp, NULL, "out8.txt");

    printf("PASS ✓\n");
}

void test_show_line_number() {
    printf("TEST: show line number...\n");

    write_temp_file("t_ln.txt",
                    "Hallo Welt\n"
                    "Hausbau ist toll\n"
                    "Noch ein Haus\n"
                    "Tschuess\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out_ln.txt");

    grep_options_t opts = {0};
    opts.show_line_number = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_ln.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp,
    "t_ln.txt:2:Hausbau ist toll\n"
                         "t_ln.txt:3:Noch ein Haus\n");

    cleanup_test(tmp, "t_ln.txt", "out_ln.txt");
    printf("PASS ✓\n");
}

void test_multiple_patterns() {
    printf("TEST: multiple patterns...\n");

    write_temp_file("t_mp.txt",
                    "Hallo Welt\n"
                    "Hausbau ist toll\n"
                    "tschüss\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out_mp.txt");

    grep_options_t opts = {0};

    char *patterns[] = {"Auto", "Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 2;

    int rc = searchInFile("t_mp.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "t_mp.txt:Hausbau ist toll\n");

    cleanup_test(tmp, "t_mp.txt", "out_mp.txt");
    printf("PASS ✓\n");
}

void test_line_numbers_ignore_case() {
    printf("TEST: -n with -i...\n");

    write_temp_file("t_ln_i.txt",
        "Hallo\n"
        "hausbau\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out_ln_i.txt");

    grep_options_t opts = {0};
    opts.show_line_number = true;
    opts.ignore_case = true;

    char *patterns[] = {"HAUSBAU"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_ln_i.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "t_ln_i.txt:2:hausbau\n");

    cleanup_test(tmp, "t_ln_i.txt", "out_ln_i.txt");
    printf("PASS ✓\n");
}

void test_line_numbers_invert_match() {
    printf("TEST: -n with -v...\n");

    write_temp_file("t_ln_v.txt",
        "Haus\n"
        "Auto\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out_ln_v.txt");

    grep_options_t opts = {0};
    opts.show_line_number = true;
    opts.invert_match = true;

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_ln_v.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "t_ln_v.txt:2:Auto\n");

    cleanup_test(tmp, "t_ln_v.txt", "out_ln_v.txt");
    printf("PASS ✓\n");
}

void test_stdin_basic() {
    printf("TEST: stdin basic...\n");

    // Input-Datei erstellen
    write_temp_file("stdin_input.txt",
        "Hallo Welt\n"
        "Hausbau ist toll\n"
        "Tschuess\n"
    );

    // stdout umleiten
    FILE *tmp = redirect_stdout_to_temp("out_stdin.txt");

    // stdin simulieren
    FILE *input = fopen("stdin_input.txt", "r");
    assert(input);

    grep_options_t opts = {0};

    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchStream(input,NULL, &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp, "Hausbau ist toll\n");

    fclose(input);

    cleanup_test(tmp, "stdin_input.txt", "out_stdin.txt");
    printf("PASS ✓\n");
}

void test_empty_file() {
    printf("TEST: empty file...\n");

    write_temp_file("t_empty.txt", "");

    FILE *tmp = redirect_stdout_to_temp("out_empty.txt");

    grep_options_t opts = {0};
    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_empty.txt", &opts);

    assert(rc == EXIT_FAILURE);
    assert_output_empty(tmp);

    cleanup_test(tmp, "t_empty.txt", "out_empty.txt");
    printf("PASS ✓\n");
}

void test_long_line() {
    printf("TEST: long line...\n");

    char longline[1000];
    memset(longline, 'A', sizeof(longline));
    longline[999] = '\0';

    // baue Text mit Pattern rein
    char content[1200];
    snprintf(content, sizeof(content),
        "%sHausXYZ\n",
        longline
    );

    write_temp_file("t_long.txt", content);

    FILE *tmp = redirect_stdout_to_temp("out_long.txt");

    grep_options_t opts = {0};
    char *patterns[] = {"Haus"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_long.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    char expected[1300];
    snprintf(expected, sizeof(expected), "t_long.txt:%s", content);
    assert_output_equals(tmp, expected);

    cleanup_test(tmp, "t_long.txt", "out_long.txt");
    printf("PASS ✓\n");
}

void test_empty_pattern_matches_all() {
    printf("TEST: empty pattern matches all...\n");

    write_temp_file("t_empty_pat.txt",
        "eins\n"
        "zwei\n"
        "drei\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out_empty_pat.txt");

    grep_options_t opts = {0};

    char *patterns[] = {""};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_empty_pat.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp,
        "t_empty_pat.txt:eins\n"
        "t_empty_pat.txt:zwei\n"
        "t_empty_pat.txt:drei\n"
    );

    cleanup_test(tmp, "t_empty_pat.txt", "out_empty_pat.txt");
    printf("PASS ✓\n");
}

void test_regex_two_dots_match() {
    printf("TEST: regex purchase.. matches purchaseXX...\n");

    write_temp_file("t_regex1.txt",
        "purchaseAA\n"
        "purchase12\n"
        "purchase\n"
        "purchased\n"
        "purchaseABC\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out_regex1.txt");

    grep_options_t opts = {0};

    char *patterns[] = {"purchase..$"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_regex1.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp,
        "t_regex1.txt:purchaseAA\n"
        "t_regex1.txt:purchase12\n"
    );

    cleanup_test(tmp, "t_regex1.txt", "out_regex1.txt");
    printf("PASS ✓\n");
}

void test_regex_match_all() {
    printf("TEST: regex .* matches all...\n");

    write_temp_file("t_regex2.txt",
        "eins\n"
        "zwei\n"
        "drei\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out_regex2.txt");

    grep_options_t opts = {0};

    char *patterns[] = {".*"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_regex2.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp,
        "t_regex2.txt:eins\n"
        "t_regex2.txt:zwei\n"
        "t_regex2.txt:drei\n"
    );

    cleanup_test(tmp, "t_regex2.txt", "out_regex2.txt");
    printf("PASS ✓\n");
}

void test_regex_ignore_case() {
    printf("TEST: regex ignore-case works...\n");

    write_temp_file("t_regex3.txt",
        "Purchase\n"
        "PURCHASE\n"
        "purchase\n"
        "pUrChAsE\n"
        "nopurchase\n"
    );

    FILE *tmp = redirect_stdout_to_temp("out_regex3.txt");

    grep_options_t opts = {0};
    opts.ignore_case = true;

    char *patterns[] = {"^purchase$"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_regex3.txt", &opts);
    assert(rc == EXIT_SUCCESS);

    assert_output_equals(tmp,
        "t_regex3.txt:Purchase\n"
        "t_regex3.txt:PURCHASE\n"
        "t_regex3.txt:purchase\n"
        "t_regex3.txt:pUrChAsE\n"
    );

    cleanup_test(tmp, "t_regex3.txt", "out_regex3.txt");
    printf("PASS ✓\n");
}

void test_regex_no_match_returns_failure() {
    printf("TEST: regex no match returns failure...\n");

    write_temp_file("t_regex4.txt",
        "foo\n"
        "bar\n"
    );

    grep_options_t opts = {0};

    char *patterns[] = {"baz.*"};
    opts.patterns = patterns;
    opts.pattern_count = 1;

    int rc = searchInFile("t_regex4.txt", &opts);
    assert(rc == EXIT_FAILURE);

    remove("t_regex4.txt");

    printf("PASS ✓\n");
}


/* ---------- MAIN ---------- */

int main() {
    test_basic();
    test_nomatch();
    test_ignore_case();
    test_invert_match();
    test_quiet_mode();
    test_list_files();
    test_count_only();
    test_file_not_found();
    test_show_line_number();
    test_multiple_patterns();
    test_line_numbers_ignore_case();
    test_line_numbers_invert_match();
    test_stdin_basic();
    test_empty_file();
    test_long_line();
    test_empty_pattern_matches_all();
    test_regex_two_dots_match();
    test_regex_match_all();
    test_regex_ignore_case();
    test_regex_no_match_returns_failure();


    printf("\nALL TESTS PASSED ✓\n");
    return 0;
}
