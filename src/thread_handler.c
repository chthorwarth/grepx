//
// Created by luke on 06.01.26.
//
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "thread_handler.h"
#include "search.h"
#include "linux_return_codes.h"

typedef struct
{
    Queue *queue;
    grep_options_t *opts;
} thread_arg_t;

static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

static int calcReturnCode(int actualCode, int newCode) {
    if (actualCode == ERROR || newCode == ERROR)
        return ERROR;
    if (actualCode == MATCH || newCode == MATCH)
        return MATCH;
    return NO_MATCH;
}

static void *worker(void *arg)
{
    thread_arg_t *targ = arg;
    int thread_rc = NO_MATCH;

    while (1)
    {
        pthread_mutex_lock(&queue_mutex);
        char *path = dequeue(targ->queue);
        pthread_mutex_unlock(&queue_mutex);

        if (!path)
            break;


        int r = searchInFile(path, targ->opts);
        thread_rc = calcReturnCode(thread_rc, r);
        free(path);
    }
    free(targ);
    return (void*)(long)thread_rc;
}

/** After using this function parallelize() you have to free the queue manually with freeQueue() */

int parallelize(Queue *q, grep_options_t *opts)
{
    if (!q || isQueueEmpty(q))
        return ERROR;

    int thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (thread_count <= 0)
        thread_count = 4;

    pthread_t *threads = malloc(sizeof(pthread_t) * thread_count);

    for (int i = 0; i < thread_count; i++)
    {
        thread_arg_t *targ = malloc(sizeof(thread_arg_t));
        targ->queue = q;
        targ->opts = opts;

        if (pthread_create(&threads[i], NULL, worker, targ) != 0)
        {
            perror("pthread_create");
            free(targ);
            thread_count = i;
            break;
        }
    }

    int final_rc = NO_MATCH;

    for (int i = 0; i < thread_count; i++) {
        void *retval;
        pthread_join(threads[i], &retval);
        int rc = (int)(long)retval;
        final_rc = calcReturnCode(final_rc, rc);
    }

    free(threads);
    return final_rc;
}
