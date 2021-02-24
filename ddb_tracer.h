#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <deadbeef/deadbeef.h>

#define SPY_MESSAGE_SIZE 4096
#define SPY_MESSAGE_BUFSIZE 1000

enum {
    SPY_TYPE_MESSAGE_LOGGER,
    SPY_TYPE_MESSAGE_PUMP,
    SPY_TYPE_MESSAGE_INTERNAL,
    SPY_TYPE_MESSAGE_TERMINATED
};

typedef struct {
    uint8_t type;
    struct timespec time_stamp;

    char msg[SPY_MESSAGE_SIZE];
} spy_msg_t;

typedef struct {
    unsigned is_active;
    uintptr_t fd;
} backend_file_t;

#define free_and_null(X)                                                                           \
    do {                                                                                           \
        free(X);                                                                                   \
        X = NULL;                                                                                  \
    } while (0);
