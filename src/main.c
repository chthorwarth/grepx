#define POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "files.h"
#include "grep_options.h"
#include "parse_args.h"
#include "validate_args.h"
#include "search.h"
#include "thread_handler.h"


int main(int argc, char *argv[]) {
    grep_options_t opts = {0};
    char *context_endptr = NULL;
    Queue *q = createQueue();

    parse(&argc, argv, &opts, context_endptr);
    validate(&opts, context_endptr);

    // RECURSIVE
    if (opts.recursive) {
        exploreDirectories(opts.paths[0], q);
    }
    // MULTIPLE FILES
    for (int i = 0; i < opts.path_count; i++) {
        enqueue(q,opts.paths[i]);
    }
    if (queueSize(q) >= 1) {
        return parallelize(q, &opts);
    }

    // STDIN
    if (opts.path_count == 0)
        return searchStream(stdin, NULL, &opts);
    freeQueue(q);
    return 0;
}
