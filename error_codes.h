#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef enum {
    ERR_OK = 0,
    ERR_NO_NODE = 1,
    ERR_MEM = 2,
    ERR_EMPTY_TREE = 3,
    ERR_EOF = 4,
    ERR_PARSE = 5,
    ERR_FILE = 6,
    ERR_READ = 7,
    ERR_CREATE_NODE = 8,
    ERR_GENERATE_TREE = 9,
    ERR_INSERT = 10,
} Err;

#endif
