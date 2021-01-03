#include <ddb_tracer.h>

typedef void (*cb_output_worker)(const uint32_t type, const char* msg, const size_t sz_msg,
                                 void* ctx);

typedef struct dispatch_item_s {
    cb_output_worker callback;
    void* ctx;
    struct dispatch_item_s* next;
} spy_dispatch_item_t;

static spy_dispatch_item_t* disp_list = NULL;
static uintptr_t disp_lock = 0;
static size_t wcount = 0;

static uintptr_t tid_dispatcher = 0;

static inline const char*
message_type_str(const unsigned type) {
    switch (type) {
    case SPY_TYPE_MESSAGE_LOGGER:
        return "LOG";
    case SPY_TYPE_MESSAGE_PUMP:
        return "MSG";
    case SPY_TYPE_MESSAGE_INTERNAL:
        return "INT";
    }
    return "ERR";
}

static inline size_t
message_process(spy_msg_t* M, char* buf, size_t size) {
    char strtv[20] = {0};

    int ret = strftime(strtv, 20, "%X", localtime(&(M->time_stamp.tv_sec)));
    if (ret > 0) {
        return snprintf(buf, size, "[%s (%s.%09ld)] %s", message_type_str(M->type), strtv,
                        M->time_stamp.tv_nsec, M->msg);
    } else {
        return snprintf(buf, size, "[%s (%s)] %s", message_type_str(M->type), "nd", M->msg);
    }
}

static void
dispatch_message(void* ctx) {
    char buffer[SPY_MESSAGE_SIZE];

    spy_msg_t msg = {0};

    for (;;) {
        while (spy_buffer_pop(&msg) != -1) {
            if (msg.type == SPY_TYPE_MESSAGE_TERMINATED)
                return;
            size_t sz = message_process(&msg, buffer, SPY_MESSAGE_SIZE * sizeof(char));
            if (sz > 0) {
                deadbeef->mutex_lock(disp_lock);

                for (spy_dispatch_item_t* item = disp_list; item; item = item->next)
                    item->callback(msg.type, buffer, sz, item->ctx);

                deadbeef->mutex_unlock(disp_lock);
            }
            memset(&msg, 0, sizeof(spy_msg_t));
        }
        if (is_terminated())
            break;
        spy_buffer_wait();
    }
}

int
spy_dispatcher_init(void) {
    if (tid_dispatcher == 0) {
        disp_lock = deadbeef->mutex_create();
        if (!disp_lock)
            return -1;

        tid_dispatcher = deadbeef->thread_start(dispatch_message, NULL);
    }
    return 0;
}

int
spy_dispatcher_register(cb_output_worker worker, void* ctx) {
    deadbeef->mutex_lock(disp_lock);

    for (spy_dispatch_item_t* X = disp_list; X; X = X->next)
        if ((X->callback == worker) && (X->ctx == ctx)) {
            deadbeef->mutex_unlock(disp_lock);
            return -1;
        }

    spy_dispatch_item_t* W = calloc(1, sizeof(spy_dispatch_item_t));
    if (W == NULL) {
        deadbeef->mutex_unlock(disp_lock);
        return -1;
    }

    W->callback = worker;
    W->ctx = ctx;
    W->next = disp_list;
    disp_list = W;

    deadbeef->mutex_unlock(disp_lock);
    return 0;
}
void
spy_dispatcher_unregister(cb_output_worker worker, void* ctx) {
    deadbeef->mutex_lock(disp_lock);

    spy_dispatch_item_t* prev = NULL;
    for (spy_dispatch_item_t* X = disp_list; X; X = X->next) {
        if ((X->callback == worker) && (X->ctx == ctx)) {
            if (prev) {
                prev->next = X->next;
            } else {
                disp_list = X->next;
            }
            free_and_null(X);
            break;
        }
        prev = X;
    }
    deadbeef->mutex_unlock(disp_lock);
}

void
spy_dispatcher_release(void) {
    if (tid_dispatcher) {
        deadbeef->thread_join(tid_dispatcher);
        if (disp_lock)
            deadbeef->mutex_free(disp_lock);
        tid_dispatcher = 0;
    }
}
