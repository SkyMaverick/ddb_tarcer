#include "ddb_tracer.h"
#include "backend_html.h"

static backend_file_t bh_state = {0};

static void
to_html(const uint32_t type, const char *msg, const size_t sz_msg, void *ctx) {
    switch (type) {
    case SPY_TYPE_MESSAGE_LOGGER:
        fprintf(FDFILE(bh_state.fd), "<p class=\"%s\"> %s </p>", "spy_logger", msg);
        break;
    case SPY_TYPE_MESSAGE_PUMP:
        fprintf(FDFILE(bh_state.fd), "<p class=\"%s\"> %s </p>", "spy_message", msg);
        break;
    case SPY_TYPE_MESSAGE_INTERNAL:
        fprintf(FDFILE(bh_state.fd), "<p class=\"%s\"> %s </p>", "spy_intrnl", msg);
        break;
    case SPY_TYPE_MESSAGE_TERMINATED:
        break;
    }
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
