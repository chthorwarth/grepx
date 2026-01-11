#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "test_header.h"
#include "../src/queue.h"

int exploreDirectories(const char *path, Queue *q);

static void createFile(const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) {
        perror(path);
        exit(1);
    }
    fprintf(f, "test\n");
    fclose(f);
}

static void createDir(const char *path)
{
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", path);
    int r = system(cmd);
    if (r != 0)
    {
        perror(path); exit(1);
    }
}

static int queueContains(Queue *q, const char *path)
{
    Element *e = q->head;
    while (e)
    {
        if (strcmp(e->path, path) == 0) return 1;
        e = e->next;
    }
    return 0;
}

int test_exploreDirectories()
{
    // temporally directory
    const char *root = "./test_root";
    createDir(root);

    createFile("./test_root/file1.txt");
    createFile("./test_root/file2.txt");
    createDir("./test_root/subdir");
    createFile("./test_root/subdir/file3.txt");

    Queue *q = createQueue();
    if (!q) return 1;

    int res = exploreDirectories(root, q);
    if (res != 0) return 1;

    int ok = 1;
    if (!queueContains(q, "./test_root/file1.txt")) ok = 0;
    if (!queueContains(q, "./test_root/file2.txt")) ok = 0;
    if (!queueContains(q, "./test_root/subdir/file3.txt")) ok = 0;

    if (!ok)
    {
        printf("Queue does not contain:\n");
        printQueue(q);
        freeQueue(q);
        return 1;
    }

    freeQueue(q);

    unlink("./test_root/file1.txt");
    unlink("./test_root/file2.txt");
    unlink("./test_root/subdir/file3.txt");
    system("rmdir ./test_root/subdir");
    system("rmdir ./test_root");

    return 0;
}

int main(void)
{
    printf("\n\n=== files.c Test ===\n\n");

    RUN_TEST(test_exploreDirectories);

    printf("\n=== Summary ===\n");
    printf("Total tests:   %d\n", total_tests);
    printf("Failed tests:  %d\n", failed_tests);
    printf("Passed tests:  %d\n", total_tests - failed_tests);
    return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
