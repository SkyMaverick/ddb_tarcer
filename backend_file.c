#include "ddb_tracer.h"

static backend_file_t bf_state = {0};

static void
to_file(const uint32_t type, const char *msg, const size_t sz_msg, void *ctx) {
    fprintf(FDFILE(bf_state.fd), "%s\n", msg);
}

int
backend_file_load(void) {
    if (bf_state.is_active)
        return 0;

    char file[NAME_MAX] = {0};
    if (filename_process(file, NAME_MAX, "log")) {
        bf_state.fd = (uintptr_t)fopen(file, "w");
        if (bf_state.fd) {
            spy_dispatcher_register(to_file, NULL);

            bf_state.is_active = 1;
            return 0;
        } else {
            push_message(SPY_TYPE_MESSAGE_INTERNAL, "%s: %s", "File not create", file);
        }
    }
    return -1;
}
void
backend_file_unload(void) {
    if (!bf_state.is_active)
        return;
    if (bf_state.fd)
        fclose(FDFILE(bf_state.fd));

    spy_dispatcher_unregister(to_file, NULL);

    bf_state.is_active = 0;
}
bool
backend_file_loaded(void) {
    return (bf_state.is_active) ? true : false;
}
