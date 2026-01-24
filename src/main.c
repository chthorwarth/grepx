#define POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "files.h"
#include "grep_options.h"
#include "linux_return_codes.h"
#include "parse_args.h"
#include "validate_args.h"
#include "search.h"
#include "thread_handler.h"


int main(int argc, char *argv[]) {
    grep_options_t opts = {0};
    char *context_endptr = NULL;
    Queue *q = createQueue();

    parse(&argc, argv, &opts, context_endptr);
    if (validate(&opts, context_endptr) != 0) {
        freeQueue(q);
        grep_options_free(&opts);
        return EXIT_FAILURE;
    }

    // RECURSIVE
    if (opts.recursive) {
        exploreDirectories(opts.paths[0], q);
    }
    // MULTIPLE FILES
    for (int i = 0; i < opts.path_count; i++) {
        char *copy = malloc(strlen(opts.paths[i]) + 1);
        strcpy(copy, opts.paths[i]);

        enqueue(q,copy);
    }
    if (queueSize(q) >= 1) {
        int r = parallelize(q, &opts);
        freeQueue(q);
        grep_options_free(&opts);
        return r;
    }

    // STDIN
    if (opts.path_count == 0){
        freeQueue(q);
        int rtrn = searchStream(stdin, NULL, &opts);
        grep_options_free(&opts);
        return rtrn;
    }
    grep_options_free(&opts);
    freeQueue(q);
    return 0;
}
