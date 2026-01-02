#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *createQueue(void)
{
	Queue *q = malloc(sizeof(Queue));
	if (!q) return NULL;
	q->head = NULL;
	q->tail = NULL;
	return q;
}

void freeQueue(Queue *q)
{
	if (!q) return;

	while (!isQueueEmpty(q))
	{
		char *path = dequeue(q);
		free(path);
	}

	free(q);
}

void enqueue(Queue *q, char *path)
{
	if (!q || !path) return;

	Element *newElem = malloc(sizeof(Element));
	if (!newElem) return;
	newElem->path = path;
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

char *dequeue(Queue *q)
{
	if (!q->head) return NULL;

	Element *tmp = q->head;
	char *path = tmp->path;
	q->head = q->head->next;
	if (!q->head)
		q->tail = NULL;
	free(tmp);
	return path;
}

void printQueue(const Queue *q)
{
	Element *current = q->head;
	while (current) {
		printf("%s\n", current->path);
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
