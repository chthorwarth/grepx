#include <stdio.h>
#include <stdlib.h>

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


Queue *createQueue(void)
{
	Queue *q = malloc(sizeof(Queue));
	q->head = NULL;
	q->tail = NULL;
	return q;
}

void freeQueue(Queue *q)
{
	Element *current = q->head;
	while (current)
	{
		Element *next = current->next;
		free(current);
		current = next;
	}
	q->head = q->tail = NULL;
	free(q);
}

void enqueue(Queue *q, FILE *file)
{
	Element *newElem = malloc(sizeof(Element));
	newElem->file = file;
	newElem->next = NULL;

	if (q->tail)
	{
		q->tail->next = newElem;
		q->tail = newElem;
	}
	else
	{
		q->head = q->tail = newElem;
	}
}

FILE *dequeue(Queue *q)
{
	if (!q->head)
		return NULL;

	Element *tmp = q->head;
	FILE *file = tmp->file;
	q->head = q->head->next;
	if (!q->head)
		q->tail = NULL;
	free(tmp);
	return file;
}

void printQueue(const Queue *q)
{
	Element *current = q->head;
	while (current) {
		printf("%p\n", (void*)current->file);
		current = current->next;
	}
}

int queueSize(const Queue *q)
{
	int size = 0;
	Element *current = q->head;
	while (current)
	{
		size++;
		current = current->next;
	}
	return size;
}

int isQueueEmpty(const Queue *q)
{
	return q->head == NULL;
}
