#ifndef QUEUE_H
#define QUEUE_H

typedef struct element
{
    char *path;
    struct element *next;
} Element;

typedef struct queue
{
    Element *head;
    Element *tail;
} Queue;

Queue *createQueue(void);

int isQueueEmpty(const Queue *q);

void freeQueue(Queue *q);

void enqueue(Queue *q, char *path);

char *dequeue(Queue *q);

void printQueue(const Queue *q);

int queueSize(const Queue *q);



#endif