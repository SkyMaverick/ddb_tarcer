#include <deadbeef/deadbeef.h>

DB_functions_t* deadbeef = NULL;
static DB_misc_t plugin;

/*============= DEADBEEF API =======================*/

static int
spy_start(void) {
    return 0;
}
static int
spy_stop(void) {
    return 0;
}

static int
spy_connect(void) {
    return 0;
}
static int
spy_disconnect(void) {
    return 0;
}

static int
spy_message(uint32_t id, uintptr_t ctx, uint32_t p1, uint32_t p2) {
    switch (id) {
    case DB_EV_NEXT:
        break;
    case DB_EV_PREV:
        break;
    case DB_EV_PLAY_CURRENT:
        break;
    case DB_EV_PLAY_NUM:
        break;
    case DB_EV_STOP:
        break;
    case DB_EV_PAUSE:
        break;
    case DB_EV_PLAY_RANDOM:
        break;
    case DB_EV_TERMINATE:
        break;
    case DB_EV_PLAYLIST_REFRESH DEPRECATED_18:
        break;
    case DB_EV_REINIT_SOUND:
        break;
    case DB_EV_CONFIGCHANGED:
        break;
    case DB_EV_TOGGLE_PAUSE:
        break;
    case DB_EV_ACTIVATED:
        break;
    case DB_EV_PAUSED:
        break;

    case DB_EV_PLAYLISTCHANGED:
        break;

    case DB_EV_VOLUMECHANGED:
        break;
    case DB_EV_OUTPUTCHANGED:
        break;
    case DB_EV_PLAYLISTSWITCHED:
        break;
    case DB_EV_SEEK:
        break;
    case DB_EV_ACTIONSCHANGED:
        break;
    case DB_EV_DSPCHAINCHANGED:
        break;

#if (DDB_API_LEVEL >= 5)
    case DB_EV_SELCHANGED:
        break;
    case DB_EV_PLUGINSLOADED:
        break;
#endif

#if (DDB_API_LEVEL >= 8)
    case DB_EV_FOCUS_SELECTION:
        break;
#endif
    case DB_EV_SONGCHANGED:
        break;
    case DB_EV_SONGSTARTED:
        break;
    case DB_EV_SONGFINISHED:
        break;

    case DB_EV_TRACKINFOCHANGED:
        break;

    case DB_EV_SEEKED:
        break;

#if (DDB_API_LEVEL >= 5)
    case DB_EV_TRACKFOCUSCURRENT:
        break;
#endif

#if (DDB_API_LEVEL >= 10)
    case DB_EV_CURSOR_MOVED:
        break;
#endif
    case DB_EV_MAX:
        break;
    default:
        break;
    }
    return 0;
}

static const char settings_dlg[] =
    "property \"Print trace in application log\" checkbox ddbspy.app_log_redirect 1;\n";

static DB_misc_t plugin = {
    .plugin.type = DB_PLUGIN_MISC,
    .plugin.flags = DDB_PLUGIN_FLAG_LOGGING,
    .plugin.api_vmajor = 1,
    .plugin.api_vminor = 11,
    .plugin.version_major = 0,
    .plugin.version_minor = 1,
    .plugin.id = "ddbspy",
    .plugin.name = "DeadBeef message spy",
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
    .plugin.message = spy_message,
    .plugin.start = spy_start,
    .plugin.stop = spy_stop,
    .plugin.connect = spy_connect,
    .plugin.disconnect = spy_disconnect,
};

DB_plugin_t*
plugin_load(DB_functions_t* api) {
    deadbeef = api;
    return DB_PLUGIN(&plugin);
}
