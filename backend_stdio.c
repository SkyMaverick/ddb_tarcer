#include <ddb_tracer.h>

static volatile unsigned is_stdio_active = 0;

static void
to_stdout(const uint32_t type, const char *msg, const size_t sz_msg, void *ctx) {
    fprintf(stdout, "%s\n", msg);
}

int
backend_stdio_load(void) {
    spy_dispatcher_register(to_stdout, NULL);
    is_stdio_active = 1;
    return 0;
}
void
backend_stdio_unload(void) {
    spy_dispatcher_unregister(to_stdout, NULL);
    is_stdio_active = 0;
}
bool
backend_stdio_loaded(void) {
    return (is_stdio_active) ? true : false;
}
