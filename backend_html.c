#include "backend_html.h"

#if defined(__linux__) || defined(__gnu_linux__)
    #include <linux/limits.h>
#endif

#ifndef NAME_MAX
    #define NAME_MAX 128
#endif
#define FDFILE(X) (FILE *)(X)

static backend_file_t bh_state = {0};

static inline void
text_to_html (const uint32_t type, const char *msg, const size_t sz_msg, void *ctx) {

}

static void
to_html(const uint32_t type, const char *msg, const size_t sz_msg, void *ctx) {
    fprintf(FDFILE(bh_state.fd), "%s", msg);
}

int
backend_html_load(void) {
    if (bh_state.is_active)
        return 0;

    char file[NAME_MAX] = {0};
    if (filename_process(file, NAME_MAX, "html")) {
        bh_state.fd = (uintptr_t)fopen(file, "w");
        if (bh_state.fd) {
            spy_dispatcher_register(to_html, NULL);
            
            fprintf(FDFILE(bh_state.fd), "%s", BACKEND_HTML_HEADER);
            bh_state.is_active = 1;
            return 0;
        } else {
            push_message(SPY_TYPE_MESSAGE_INTERNAL, "%s: %s", "File not create", file);
        }
    }
    return -1;
}
void
backend_html_unload(void) {
    if (!bh_state.is_active)
        return;
    if (bh_state.fd) {
        fprintf(FDFILE(bh_state.fd), "%s", BACKEND_HTML_FOOTER);
        fclose(FDFILE(bh_state.fd));
    }

    spy_dispatcher_unregister(to_file, NULL);

    bh_state.is_active = 0;
}
bool
backend_html_loaded(void) {
    return (bh_state.is_active) ? true : false;
}

#undef FDFILE
