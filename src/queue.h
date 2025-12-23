#ifndef FILE_QUEUE_H
#define FILE_QUEUE_H

#include <stdio.h>


typedef struct element
{
    FILE *file;
    struct element *next;
} Element;

typedef struct queue
{
    Element *head;
    Element *tail;
} Queue;

Queue *createQueue(void);

void freeQueue(Queue *q);

void enqueue(Queue *q, FILE *file);

FILE *dequeue(Queue *q);

void printQueue(const Queue *q);

int queueSize(const Queue *q);

int isQueueEmpty(const Queue *q);

#endif