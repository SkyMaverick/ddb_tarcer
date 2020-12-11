#include <deadbeef/deadbeef.h>

DB_functions_t* deadbeef = NULL;
static DB_misc_t plugin;

static const char settings_dlg[] =
    "property \"Print trace in application log\" checkbox ddbspy.app_log_redirect 1;\n";

static DB_misc_t plugin =
    {
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
        .plugin.message = NULL,
        .plugin.connect = NULL,
        .plugin.disconnect = NULL,
};

    DB_plugin_t *
    plugin_load(DB_functions_t * api) {
    deadbeef = api;
    return DB_PLUGIN(&plugin);
}
