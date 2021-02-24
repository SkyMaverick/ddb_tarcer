#include "ddb_tracer.h"

static void
spy_log_callback(struct DB_plugin_s *plugin, uint32_t layers, const char *text, void *ctx) {
    push_message(SPY_TYPE_MESSAGE_LOGGER, "[%s] %s", (plugin) ? plugin->name : "<unknown>", text);
}
