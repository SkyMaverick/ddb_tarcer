#include <ddb_tracer.h>

typedef int (*cb_output_worker)(const uint32_t type, const char* msg, void* ctx);

typedef struct dispatch_item_s {
    cb_output_worker worker;
    void* ctx;
    struct dispatch_item_s* next;
} spy_dispatch_item_t;

static spy_dispatch_item_t* disp_list = NULL;
static uintptr_t disp_lock = 0;
static size_t wcount = 0;

static uintptr_t tid_dispatcher = 0;

void
dispatch_message(void* ctx) {
    spy_msg_t item = {0};

    for (;;) {
        while (spy_buffer_pop(&item) != -1) {
            // printf("%s", item.msg); /*DEBUG*/
            // TODO
            memset(&item, 0, sizeof(spy_msg_t));
        }
        if (is_terminated())
            break;
        spy_buffer_wait();
    }
}

int
spy_dispatcher_init(void) {
    disp_lock = deadbeef->mutex_create();
    if (!disp_lock)
        return -1;

    tid_dispatcher = deadbeef->thread_start(dispatch_message, NULL);
    return 0;
}

int
spy_dispatcher_register (cb_output_worker worker, void* ctx) {
}
void
spy_dispatcher_unregister (cb_output_worker worker) {
}

void
spy_dispatcher_release(void) {
    if (tid_dispatcher)
        deadbeef->thread_join(tid_dispatcher);
    if (disp_lock)
        deadbeef->mutex_free(disp_lock);
}
