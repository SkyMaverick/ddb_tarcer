#include <ddb_tracer.h>

typedef struct {
    uint32_t head;
    uint32_t tail;
    uint32_t count;

    uintptr_t lock;
    uintptr_t cond;

    spy_msg_t ring[SPY_MESSAGE_BUFSIZE];
} spy_buffer_t;

static spy_buffer_t* buffer;

int
spy_buffer_init(void) {
    uintptr_t L, C;
    if (buffer == NULL) {
        if ((L = deadbeef->mutex_create()) == 0)
            return 1;
        if ((C = deadbeef->cond_create()) == 0) {
            deadbeef->mutex_free(L);
            return 1;
        }

        if ((buffer = calloc(1, sizeof(spy_buffer_t))) != NULL) {
            buffer->lock = L;
            buffer->cond = C;
        } else {
            deadbeef->mutex_free(L);
            deadbeef->cond_free(C);
        }
    }
    return 0;
}

void
spy_buffer_push(const uint8_t type, const char* message) {
    if (buffer) {
        deadbeef->mutex_lock(buffer->lock);
        if (buffer->count < SPY_MESSAGE_BUFSIZE) {

            // get message time stamp
            clock_gettime(CLOCK_REALTIME, &(buffer->ring[buffer->tail].time_stamp));
            // copy text message
            snprintf(buffer->ring[buffer->tail].msg, SPY_MESSAGE_SIZE, "%s", message);
            buffer->ring[buffer->tail].type = type; // set message type

            buffer->count++;
            buffer->tail++;

            if (buffer->tail == SPY_MESSAGE_BUFSIZE)
                buffer->tail = 0;
            deadbeef->cond_signal(buffer->cond);
            fprintf(stdout, "%s", message);
        }
        deadbeef->mutex_unlock(buffer->lock);
    }
}

void
spy_buffer_pop(spy_msg_t* message) {
    if (buffer && message) {
        deadbeef->mutex_lock(buffer->lock);
        if (buffer->count) {

            memcpy(message, &(buffer->ring[buffer->head]), sizeof(spy_msg_t));

            buffer->count--;
            buffer->head++;

            if (buffer->head == SPY_MESSAGE_BUFSIZE)
                buffer->head = 0;
        }
        deadbeef->mutex_unlock(buffer->lock);
    }
}

bool
spy_buffer_isempty(void) {
    return (buffer && (buffer->count > 0)) ? true : false;
}

void
spy_buffer_clear(void) {
    if (buffer) {
        buffer->head = buffer->tail = buffer->count = 0;
        memset(buffer->ring, 0, sizeof(char) * SPY_MESSAGE_SIZE);
    }
}

void
spy_buffer_wait(void) {
    if (buffer) {
        deadbeef->mutex_lock(buffer->lock);
        deadbeef->cond_wait(buffer->cond, buffer->lock);
        deadbeef->mutex_unlock(buffer->lock);
    }
}

void
spy_buffer_release(void) {
    if (buffer) {
        if (buffer->lock)
            deadbeef->mutex_free(buffer->lock);
        if (buffer->cond)
            deadbeef->cond_free(buffer->cond);
        free(buffer);
    }
}
