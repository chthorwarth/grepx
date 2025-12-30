#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "grep_options.h"
#include "parse_args.h"
#include "validate_args.h"


int main(int argc, char *argv[]) {
    grep_options_t opts = {0};
    char *context_endptr = NULL;

    parse(&argc, argv, &opts, context_endptr);

    validate(&opts, context_endptr);
}