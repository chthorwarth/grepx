//
// Created by luke on 06.01.26.
//

#ifndef GREPX_THREAD_HANDLER_H
#define GREPX_THREAD_HANDLER_H
#include "queue.h"
#include "grep_options.h"

int parallelize(Queue *q, grep_options_t *opts);

#endif //GREPX_THREAD_HANDLER_H