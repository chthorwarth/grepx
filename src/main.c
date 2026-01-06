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


int main(int argc, char *argv[])
{
    grep_options_t opts = {0};
    char *context_endptr = NULL;

    parse(&argc, argv, &opts, context_endptr);

    validate(&opts, context_endptr);

    if (opts.recursive)
    {
        Queue *q = createQueue();
        exploreDirectories(opts.paths[0], q);
        if (queueSize(q) > 1)
        {
            parallelize(q, &opts);
        }
        freeQueue(q);
        return 0;
    }

    for (int i = 0; i < opts.path_count; i++) {
        searchInFile(opts.paths[i], &opts);
    }

    return 0;
}