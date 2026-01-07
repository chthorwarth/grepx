#ifndef SEARCH_CORE_H
#define SEARCH_CORE_H

#include "grep_options.h"

int searchInFile(const char *filename, grep_options_t *opts);
int searchStream(FILE *stream, const char *filename, grep_options_t *opts);

#endif