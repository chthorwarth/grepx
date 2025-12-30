#ifndef PARSE_H
#define PARSE_H

#define _POSIX_C_SOURCE 200809L

#include "grep_options.h"

/**
 * Parses command-line arguments and fills the grep_options_t structure.
 *
 * This function processes both option flags (e.g. -i, -v, -n, -C)
 * and positional arguments. Patterns can be provided explicitly
 * using -e, implicitly as the first positional argument, or via
 * a pattern file using -f. All remaining positional arguments are
 * treated as file or path names.
 *
 * @param argc Number of command-line arguments from main()
 * @param argv Argument vector from main()
 * @param opts Pointer to an initialized grep_options_t structure
 * @param context_endptr End pointer used by strtol() when parsing
 *                       the -C (context) option for error checking
 */
void parse(int *argc, char *argv[], grep_options_t *opts, char *context_endptr);

#endif /* PARSE_H */