#include <deadbeef/deadbeef.h>
#include "ddb_tracer.h"

DB_functions_t* deadbeef = NULL;
static DB_misc_t plugin;

static volatile unsigned flag_terminated = 0;
static volatile unsigned flag_loaded = 0;

bool
is_terminated(void) {
    return (flag_terminated) ? true : false;
}

bool
is_loaded(void) {
    return (flag_loaded) ? true : false;
}

DB_plugin_t*
plugin_instance(void) {
    return (DB_plugin_t*)(&plugin);
}

#include "buffer.c"
#include "dispatcher.c"

static inline void
push_message_direct(uint8_t type, const char* msg) {
    spy_buffer_push(type, msg);
}
static inline size_t
push_message(uint8_t type, const char* fmt, ...) {
    char msg[SPY_MESSAGE_SIZE];
    int ret = 0;

    va_list args;

    va_start(args, fmt);
    if ((ret = vsnprintf(msg, SPY_MESSAGE_SIZE, fmt, args)) > 0)
        push_message_direct(type, msg);
    va_end(args);

    return (ret < 0) ? 0 : ret;
}

#include "ddb_tracer_logs.c"
#include "ddb_tracer_msgs.c"

#include "backend_stdio.c"
#include "backend_file.c"

#define BACKEND_LOAD(X)                                                                            \
    if (deadbeef->conf_get_int("ddbspy.backend_" #X, 0))                                           \
    backend_##X##_load()
#define BACKEND_CHANGE(X)                                                                          \
    deadbeef->conf_get_int("ddbspy.backend_" #X, 0) ? backend_##X##_load() : backend_##X##_unload()
#define BACKEND_UNLOAD(X) backend_##X##_unload()

static int
spy_system_load(void) {
    if ((!(is_loaded())) && (deadbeef->conf_get_int("ddbspy.spy_enable", 0))) {
        if (spy_buffer_init() != 0)
            return -1;
        if (spy_dispatcher_init() != 0) {
            spy_buffer_release();
            return -1;
        }

        deadbeef->log_viewer_register(spy_log_callback, NULL);

        BACKEND_LOAD(stdio);
        BACKEND_LOAD(file);

        flag_loaded = 1;
    }
    return 0;
}

static int
spy_system_unload(void) {
    if (is_loaded()) {
        BACKEND_UNLOAD(file);
        BACKEND_UNLOAD(stdio);

        deadbeef->log_viewer_unregister(spy_log_callback, NULL);
        push_message_direct(SPY_TYPE_MESSAGE_TERMINATED, NULL);

        spy_dispatcher_release();
        spy_buffer_release();

        flag_loaded = 0;
    }
    return 0;
}

/*============= DEADBEEF API =======================*/

static int
spy_start(void) {
    return spy_system_load();
}

static int
spy_stop(void) {
    return spy_system_unload();
}

static int
spy_messages(uint32_t id, uintptr_t ctx, uint32_t p1, uint32_t p2) {

    switch (id) {
    case DB_EV_CONFIGCHANGED: {
        deadbeef->conf_get_int("ddbspy.spy_enable", 0) ? spy_system_load() : spy_system_unload();
        if (flag_loaded) {
            BACKEND_CHANGE(stdio);
            BACKEND_CHANGE(file);
        }
        break;
    }
    case DB_EV_TERMINATE:
        flag_terminated = 1;
        break;
    }

    spy_process(id, ctx, p1, p2);
    return 0;
}

static const char settings_dlg[] =
    "property \"Enable tracing\" checkbox ddbspy.spy_enable 0;\n"
    "property \"Enable stdio backend\" checkbox ddbspy.backend_stdio 1;\n"
    "property \"Enable file backend\" checkbox ddbspy.backend_file 0;\n"
    "property \"Show message extensions\" checkbox ddbspy.msg_extension 1;\n";

static DB_misc_t plugin = {
    .plugin.type = DB_PLUGIN_MISC,
    .plugin.flags = DDB_PLUGIN_FLAG_LOGGING,
    .plugin.api_vmajor = 1,
    .plugin.api_vminor = 11,
    .plugin.version_major = 1,
    .plugin.version_minor = 1,
    .plugin.id = "ddbspy",
    .plugin.name = "Message spy",
    .plugin.descr = "DeadBeef internal meesage end tracing spy-tool.\n"
                    "Need for developers and testers.\n",
    .plugin.copyright =
        "DeaDBeeF Player Spy\n"
        "Copyright (C) 2021 Alexander Smirnov \n"
        "\n"
        "This software is provided 'as-is', without any express or implied\n"
        "warranty.  In no event will the authors be held liable for any damages\n"
        "arising from the use of this software.\n"
        "\n"
        "Permission is granted to anyone to use this software for any purpose,\n"
        "including commercial applications, and to alter it and redistribute it\n"
        "freely, subject to the following restrictions:\n"
        "\n"
        " 1. The origin of this software must not be misrepresented; you must not\n"
        "    claim that you wrote the original software. If you use this software\n"
        "    in a product, an acknowledgment in the product documentation would be\n"
        "    appreciated but is not required.\n"
        " 2. Altered source versions must be plainly marked as such, and must not be\n"
        "    misrepresented as being the original software.\n"
        " 3. This notice may not be removed or altered from any source distribution.\n",
    .plugin.website = "",
    .plugin.configdialog = settings_dlg,
    .plugin.message = spy_messages,
    .plugin.start = spy_start,
    .plugin.stop = spy_stop,
};

DB_plugin_t*
ddb_spy_load(DB_functions_t* api) {
    deadbeef = api;
    return DB_PLUGIN(&plugin);
}
