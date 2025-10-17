#ifndef TIMING_H
#define TIMING_H

#include "error_codes.h"

typedef struct {
    int key;
    char *value;
} KeyValuePair;

Err timing_tree();

#endif
