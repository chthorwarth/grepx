#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../src/queue.h"
#include "../src/thread_handler.h"
#include "../src/grep_options.h"

#define RUN_TEST(test_fn)                    \
    do {                                     \
        total_tests++;                       \
        if (test_fn() != 0) {                \
            failed_tests++;                  \
            printf("[FAIL] %s\n", #test_fn); \
        } else {                             \
            printf("[ OK ] %s\n", #test_fn); \
        }                                    \
    } while (0)

static int total_tests = 0;
static int failed_tests = 0;

static pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
static int call_count = 0;

int searchInFile(const char *filename, grep_options_t *opts)
{
    (void)filename;
    (void)opts;

    pthread_mutex_lock(&count_mutex);
    call_count++;
    pthread_mutex_unlock(&count_mutex);

    return 0;
}

int get_call_count(void)
{
    return call_count;
}

void reset_call_count(void)
{
    call_count = 0;
}


int test_parallelize_empty_queue(void)
{
    Queue *q = createQueue();
    grep_options_t opts = {0};

    reset_call_count();
    parallelize(q, &opts);

    if (get_call_count() != 0)
        return 1;

    freeQueue(q);
    return 0;
}


int test_parallelize_single_element(void)
{
    Queue *q = createQueue();
    grep_options_t opts = {0};

    char *p = malloc(strlen("file1")+1);
    strcpy(p, "file1");
    enqueue(q, p);

    reset_call_count();
    parallelize(q, &opts);

    if (get_call_count() != 1)
        return 1;

    if (!isQueueEmpty(q))
        return 1;

    freeQueue(q);
    return 0;
}


int test_parallelize_multiple_elements(void)
{
    Queue *q = createQueue();
    grep_options_t opts = {0};

    for (int i = 0; i < 10; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "file%d", i);
        char *p = malloc(strlen(buf)+1);
        strcpy(p, buf);
        enqueue(q, p);
    }

    reset_call_count();
    parallelize(q, &opts);

    if (get_call_count() != 10)
        return 1;

    if (!isQueueEmpty(q))
        return 1;

    freeQueue(q);
    return 0;
}


int test_parallelize_queue_fully_consumed(void)
{
    Queue *q = createQueue();
    grep_options_t opts = {0};

    for (int i = 0; i < 5; i++) {
        char *p = malloc(8);
        strcpy(p, "dummy");
        enqueue(q, p);
    }

    parallelize(q, &opts);

    if (q->head != NULL)
        return 1;

    if (q->tail != NULL)
        return 1;

    freeQueue(q);
    return 0;
}


int main(void)
{
    printf("=== Thread Handler Tests ===\n\n");

    RUN_TEST(test_parallelize_empty_queue);
    RUN_TEST(test_parallelize_single_element);
    RUN_TEST(test_parallelize_multiple_elements);
    RUN_TEST(test_parallelize_queue_fully_consumed);

    printf("\n=== Summary ===\n");
    printf("Total tests:   %d\n", total_tests);
    printf("Failed tests:  %d\n", failed_tests);
    printf("Passed tests:  %d\n", total_tests - failed_tests);

    return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
