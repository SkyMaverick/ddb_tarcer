#include <ddb_tracer.h>

#if defined(__linux__) || defined(__gnu_linux__)
    #include <linux/limits.h>
#endif

#ifndef NAME_MAX
    #define NAME_MAX 128
#endif

static volatile unsigned is_file_active = 0;
static volatile uintptr_t fd = 0;

#define FDFILE(X) (FILE*)(X)

static inline size_t
filename_process(char* buf, size_t sz) {
    char date[sz / 2];
    time_t now = time(NULL);

    return (strftime(date, sz / 2, "%d.%m.%Y-%X", localtime(&now)))
               ? snprintf(buf, sz, "deadbeef_spy-%s.log", date)
               : 0;
}

static void
to_file(const uint32_t type, const char* msg, const size_t sz_msg, void* ctx) {
    fprintf(FDFILE(fd), "%s", msg);
}

int
backend_file_load(void) {
    if (is_file_active)
        return 0;

    char file[NAME_MAX] = {0};
    if (filename_process(file, NAME_MAX)) {
        fd = (uintptr_t)fopen(file, "w");
        if (fd) {
            spy_dispatcher_register(to_file, NULL);

            is_file_active = 1;
            return 0;
        } else {
            push_message(SPY_TYPE_MESSAGE_INTERNAL, "%s: %s", "File not create", file);
        }
    }
    return -1;
}
void
backend_file_unload(void) {
    if (!is_file_active)
        return;
    if (fd)
        fclose(FDFILE(fd));

    spy_dispatcher_unregister(to_file, NULL);

    is_file_active = 0;
}
bool
backend_file_loaded(void) {
    return (is_file_active) ? true : false;
}

#undef FDFILE
