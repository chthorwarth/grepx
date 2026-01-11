#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

/* INCLUDE THE PROVIDED HEADER */
#include "test_header.h"

/* --- HELPER FUNCTIONS --- */

// Writes content to a specific path
void write_file(const char *path, const char *content) {
    FILE *f = fopen(path, "w");
    if (f) {
        fputs(content, f);
        fclose(f);
    }
}

// Executes ./bin/grepx with arguments and captures stdout
// Returns the exit code of the application
int run_app(const char *args, char *output, size_t out_size) {
    char command[4096];
    // Construct command: ./bin/grepx [args]
    snprintf(command, sizeof(command), "./bin/grepx %s", args);

    FILE *fp = popen(command, "r");
    if (!fp) return -1;

    memset(output, 0, out_size);
    size_t n = fread(output, 1, out_size - 1, fp);
    output[n] = '\0';

    int status = pclose(fp);
    return WEXITSTATUS(status);
}

/* --- TESTS (Return 0 on success, 1 on failure) --- */

int test_basic() {
    write_file("t_basic.txt", "Apple\nBanana\ncherry\n");
    char out[1024];

    int rc = run_app("\"Banana\" ./t_basic.txt", out, sizeof(out));

    remove("t_basic.txt");

    if (rc != 0) return 1;
    if (strstr(out, "Banana") == NULL) return 1;
    if (strstr(out, "Apple") != NULL) return 1;
    
    return 0; // Success
}

int test_invert() {
    write_file("t_inv.txt", "Apple\nBanana\n");
    char out[1024];

    // Search everything EXCEPT "Apple" -> Expect "Banana"
    run_app("-v \"Apple\" ./t_inv.txt", out, sizeof(out));

    remove("t_inv.txt");

    if (strstr(out, "Banana") == NULL) return 1;
    if (strstr(out, "Apple") != NULL) return 1;
    
    return 0;
}

int test_ignore_case() {
    write_file("t_case.txt", "HELLO WORLD\n");
    char out[1024];

    // Search "hello" in "HELLO WORLD"
    run_app("-i \"hello\" ./t_case.txt", out, sizeof(out));

    remove("t_case.txt");

    if (strstr(out, "HELLO WORLD") == NULL) return 1;
    
    return 0;
}

int test_line_number() {
    write_file("t_num.txt", "LineOne\nLineTwo\nTarget\n");
    char out[1024];

    // Expecting line number 3
    run_app("-n \"Target\" ./t_num.txt", out, sizeof(out));

    remove("t_num.txt");

    // Output format is usually "filename:3:Target" or just "3:Target"
    if (strstr(out, "3:") == NULL) return 1;
    if (strstr(out, "Target") == NULL) return 1;
    
    return 0;
}

int test_count() {
    write_file("t_count.txt", "Match\nNo\nMatch\n");
    char out[1024];

    // Should return "2"
    run_app("-c \"Match\" ./t_count.txt", out, sizeof(out));

    remove("t_count.txt");

    if (strstr(out, "2") == NULL) return 1;
    // Should not print the lines
    if (strstr(out, "Match") == NULL) return 0; // Wait, logic check: string contains "Match" in command, ensure output check
    // Actually output should NOT contain the text lines, just the number.
    // However, if the output buffer catches the number "2", that's good. 
    // Checking strict correctness:
    if (strlen(out) > 5) return 1; // Assuming result is just "2\n" + filename potentially
    
    return 0;
}

int test_list_files() {
    write_file("t_list.txt", "SecretData\n");
    char out[1024];

    // Should print filename only
    run_app("-l \"SecretData\" ./t_list.txt", out, sizeof(out));

    remove("t_list.txt");

    if (strstr(out, "t_list.txt") == NULL) return 1;
    if (strstr(out, "SecretData") != NULL) return 1; // Content not printed
    
    return 0;
}

int test_quiet() {
    write_file("t_quiet.txt", "FoundMe\n");
    char out[1024];

    // Should print nothing, but return 0 (Success)
    int rc = run_app("-q \"FoundMe\" ./t_quiet.txt", out, sizeof(out));

    remove("t_quiet.txt");

    if (rc != 0) return 1;
    if (strlen(out) != 0) return 1;
    
    return 0;
}

int test_quiet_nomatch() {
    write_file("t_quiet.txt", "FoundMe\n");
    char out[1024];

    // Should print nothing, but return 0 (Success)
    int rc = run_app("-q \"House\" ./t_quiet.txt", out, sizeof(out));

    remove("t_quiet.txt");

    if (rc != 1) return 1;
    if (strlen(out) != 0) return 1;
    
    return 0;
}

int test_threading_multiple_files() {
    write_file("t_thread1.txt", "CommonStr\n");
    write_file("t_thread2.txt", "CommonStr\n");
    char out[2048];

    // Pass multiple files -> Triggers parallelize()
    run_app("\"CommonStr\" ./t_thread1.txt ./t_thread2.txt", out, sizeof(out));

    remove("t_thread1.txt");
    remove("t_thread2.txt");

    if (strstr(out, "t_thread1.txt") == NULL) return 1;
    if (strstr(out, "t_thread2.txt") == NULL) return 1;
    
    return 0;
}

int test_recursive() {
    // Setup directory structure
    system("mkdir -p t_rec/sub");
    write_file("t_rec/sub/hidden.txt", "DeepItem\n");
    char out[2048];

    // Search in folder "t_rec"
    run_app("-r \"DeepItem\" ./t_rec", out, sizeof(out));

    // Cleanup dir immediately (or later)
    system("rm -rf t_rec");

    if (strstr(out, "t_rec/sub/hidden.txt") == NULL) return 1;
    if (strstr(out, "DeepItem") == NULL) return 1;

    return 0;
}

int test_stdin() {
    write_file("t_input.txt", "FromPipe\n");
    char out[1024];

    // Use < to redirect file into stdin
    run_app("\"FromPipe\" < ./t_input.txt", out, sizeof(out));

    remove("t_input.txt");

    if (strstr(out, "FromPipe") == NULL) return 1;
    
    return 0;
}

int test_no_match() {
    write_file("t_none.txt", "Empty\n");
    char out[1024];

    int rc = run_app("\"Ghost\" ./t_none.txt", out, sizeof(out));

    remove("t_none.txt");

    if (rc != 1) return 1; // Expecting exit code 1
    
    return 0;
}

int test_file_not_found() {
    char out[1024];

    // Try to search in a file that definitely does not exist
    int rc = run_app("\"Pattern\" ./does_not_exist.txt", out, sizeof(out));

    // If rc is 0, the program claimed success -> FAIL
    if (rc == 0) return 1;
    
    return 0;
}

int test_conflicting_options() {
    char out[1024];
    write_file("t_conflict.txt", "Some content\n");

    // We pass both -c (Count) and -n (Line Numbers).
    int rc = run_app("-c -n \"Some\" ./t_conflict.txt", out, sizeof(out));

    remove("t_conflict.txt");

    // If rc is 0, the program ignored the validation error -> FAIL
    if (rc == 0) return 1;
    
    return 0;
}

int test_invalid_option() {
    char out[1024];
    write_file("t_inv_opt.txt", "Content");

    int rc = run_app("-x \"Search\" ./t_inv_opt.txt", out, sizeof(out));
    
    remove("t_inv_opt.txt");

    if (rc == 0) return 1; // Should fail
    return 0;
}


/* --- MAIN ENTRY POINT --- */

int main() {
    // Check if binary exists
    if (access("./bin/grepx", X_OK) != 0) {
        fprintf(stderr, "Error: ./bin/grepx not found. Run 'make' first.\n");
        return 1;
    }

    printf("\n\n=== integration_tests.c Test ===\n\n");
    
    RUN_TEST(test_basic);
    RUN_TEST(test_invert);
    RUN_TEST(test_ignore_case);
    RUN_TEST(test_line_number);
    RUN_TEST(test_count);
    RUN_TEST(test_list_files);
    RUN_TEST(test_quiet);
    RUN_TEST(test_quiet_nomatch);
    RUN_TEST(test_threading_multiple_files);
    RUN_TEST(test_recursive);
    RUN_TEST(test_stdin);
    RUN_TEST(test_no_match);
    RUN_TEST(test_file_not_found);
    RUN_TEST(test_conflicting_options);
    RUN_TEST(test_invalid_option);

    printf("\n=== Summary ===\n");
    printf("Total tests:   %d\n", total_tests);
    printf("Failed tests:  %d\n", failed_tests);
    printf("Passed tests:  %d\n", total_tests - failed_tests);

    // Return status based on failure count
    return (failed_tests > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}