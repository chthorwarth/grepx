//
// Created by luke on 21.12.25.
//


#include <stdio.h>
#include <stdlib.h>
#include "../src/queue.h"

int main(void)
{
    Queue *q = createQueue();
    FILE *f1 = fopen("Makefile", "r");
    FILE *f2 = fopen("test/test_queue.c", "r");
    FILE *f3 = fopen("test/test.txt", "r");


    if (!f1) perror("Fehler beim Öffnen von Makefile");
    if (!f2) perror("Fehler beim Öffnen von test_queue.c");
    if (!f3) perror("Fehler beim Öffnen von test.txt");

    enqueue(q, f1);
    enqueue(q, f2);
    enqueue(q, f3);

    printf("Queue nach enqueue:\n");
    printQueue(q);

    FILE *file;
    while ((file = dequeue(q)) != NULL)
    {
        printf("Dequeued file pointer: %p\n", (void*)file);
        fclose(file);
    }

    freeQueue(q);

    return 0;
}
