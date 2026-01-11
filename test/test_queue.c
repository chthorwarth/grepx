#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_header.h"
#include "../src/queue.h"


int test_createQueue()
{
    Queue *q = createQueue();
    if (!q)
        return 1;

    if (q->head != NULL || q->tail != NULL)
        return 1;

    freeQueue(q);
    return 0;
}

int test_isQueueEmpty_on_new_queue()
{
    Queue *q = createQueue();
    int result = isQueueEmpty(q);
    freeQueue(q);
    return result == 1 ? 0 : 1;
}

int test_dequeue_empty_queue()
{
    Queue *q = createQueue();
    char *p = dequeue(q);
    freeQueue(q);
    return p == NULL ? 0 : 1;
}

int test_enqueue_single_element()
{
    Queue *q = createQueue();

    if (!q)
        return 1;

    char *p = malloc(strlen("file1.txt") + 1);
    if (!p) return 1;
    strcpy(p, "file1.txt");

    enqueue(q, p);

    if (!q->head || !q->tail) return 1;
    if (q->head != q->tail) return 1;
    if (strcmp(q->head->path, "file1.txt") != 0) return 1;

    freeQueue(q);

    return 0;
}

int test_enqueue_dequeue_fifo(void)
{
    Queue *q = createQueue();

    char *a = malloc(strlen("a.txt")+1); strcpy(a, "a.txt"); enqueue(q, a);
    char *b = malloc(strlen("b.txt")+1); strcpy(b, "b.txt"); enqueue(q, b);
    char *c = malloc(strlen("c.txt")+1); strcpy(c, "c.txt"); enqueue(q, c);

    char *p;

    p = dequeue(q); if (!p || strcmp(p, "a.txt") != 0) return 1; free(p);
    p = dequeue(q); if (!p || strcmp(p, "b.txt") != 0) return 1; free(p);
    p = dequeue(q); if (!p || strcmp(p, "c.txt") != 0) return 1; free(p);

    if (dequeue(q) != NULL) return 1;

    freeQueue(q);
    return 0;
}

int test_queueSize()
{
    Queue *q = createQueue();

    if (queueSize(q) != 0) return 1;

    char *a = malloc(strlen("x") + 1);
    if (!a) return 1;
    strcpy(a, "x");

    char *b = malloc(strlen("y") + 1);
    if (!b) {
        free(a);
        return 1;
    }

    strcpy(b, "y");

    enqueue(q, a);
    enqueue(q, b);

    if (queueSize(q) != 2) return 1;

    char *p = dequeue(q);
    free(p);

    if (queueSize(q) != 1) return 1;

    freeQueue(q);
    return 0;
}

int test_tail_null_after_last_dequeue(void)
{
    Queue *q = createQueue();

    char *last = malloc(strlen("last") + 1);
    if (!last) return 1;
    strcpy(last, "last");

    enqueue(q, last);

    char *p = dequeue(q);
    free(p);

    if (q->head != NULL) return 1;
    if (q->tail != NULL) return 1;

    freeQueue(q);
    return 0;
}


int main(void)
{
    printf("\n\n=== queue.c Tests ===\n\n");

    RUN_TEST(test_createQueue);
    RUN_TEST(test_isQueueEmpty_on_new_queue);
    RUN_TEST(test_dequeue_empty_queue);
    RUN_TEST(test_enqueue_single_element);
    RUN_TEST(test_enqueue_dequeue_fifo);
    RUN_TEST(test_queueSize);
    RUN_TEST(test_tail_null_after_last_dequeue);

    printSummary();
    return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
