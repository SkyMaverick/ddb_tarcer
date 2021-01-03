#define DDB_EVENTS_COUNT 32

typedef struct {
    uint32_t ev;
    char txt[50];
} ev_hash_item_t;

#define EVENT_UKNOWN_MSG "UNKNOWN EVENT"
static ev_hash_item_t ev_table[DDB_EVENTS_COUNT] = {
    {DB_EV_NEXT, "DB_EV_NEXT"},
    {DB_EV_PREV, "DB_EV_PREV"},
    {DB_EV_PLAY_CURRENT, "DB_EV_PLAY_CURRENT"},
    {DB_EV_PLAY_NUM, "DB_EV_PLAY_NUM"},
    {DB_EV_STOP, "DB_EV_STOP"},
    {DB_EV_PAUSE, "DB_EV_PAUSE"},
    {DB_EV_PLAY_RANDOM, "DB_EV_PLAY_RANDOM"},
    {DB_EV_TERMINATE, "DB_EV_TERMINATE"},
    {DB_EV_PLAYLIST_REFRESH, "DB_EV_PLAYLIST_REFRESH"},
    {DB_EV_REINIT_SOUND, "DB_EV_REINIT_SOUND"},
    {DB_EV_CONFIGCHANGED, "DB_EV_CONFIGCHANGED"},
    {DB_EV_TOGGLE_PAUSE, "DB_EV_TOGGLE_PAUSE"},
    {DB_EV_ACTIVATED, "DB_EV_ACTIVATED"},
    {DB_EV_PAUSED, "DB_EV_PAUSED"},
    {DB_EV_PLAYLISTCHANGED, "DB_EV_PLAYLISTCHANGED"},
    {DB_EV_VOLUMECHANGED, "DB_EV_VOLUMECHANGED"},
    {DB_EV_OUTPUTCHANGED, "DB_EV_OUTPUTCHANGED"},
    {DB_EV_PLAYLISTSWITCHED, "DB_EV_PLAYLISTSWITCHED"},
    {DB_EV_SEEK, "DB_EV_SEEK"},
    {DB_EV_ACTIONSCHANGED, "DB_EV_ACTIONSCHANGED"},
    {DB_EV_DSPCHAINCHANGED, "DB_EV_DSPCHAINCHANGED"},
    {DB_EV_SELCHANGED, "DB_EV_SELCHANGED"},
    {DB_EV_PLUGINSLOADED, "DB_EV_PLUGINSLOADED"},
    {DB_EV_FOCUS_SELECTION, "DB_EV_FOCUS_SELECTION"},
    {DB_EV_SONGCHANGED, "DB_EV_SONGCHANGED"},
    {DB_EV_SONGSTARTED, "DB_EV_SONGSTARTED"},
    {DB_EV_SONGFINISHED, "DB_EV_SONGFINISHED"},
    {DB_EV_TRACKINFOCHANGED, "DB_EV_TRACKINFOCHANGED"},
    {DB_EV_SEEKED, "DB_EV_SEEKED"},
    {DB_EV_TRACKFOCUSCURRENT, "DB_EV_TRACKFOCUSCURRENT"},
    {DB_EV_CURSOR_MOVED, "DB_EV_CURSOR_MOVED"},
    {DB_EV_MAX, "DB_EV_MAX"},
};

static inline const char*
search_ev_descr(const uint32_t id) {
    for (uint32_t i = 0; i < DDB_EVENTS_COUNT; i++)
        if (ev_table[i].ev == id)
            return ev_table[i].txt;
    return EVENT_UKNOWN_MSG;
}

#define HEADER_FORMAT "%s (%u) <%p,%u,%u>"
#define SIMPLE_MESSAGE_PUSH                                                                        \
    push_message(SPY_TYPE_MESSAGE_PUMP, HEADER_FORMAT "\n", search_ev_descr(id), id, (void*)ctx,   \
                 p1, p2);

#define COMPLEX_MESSAGE_PUSH(F, ...)                                                               \
    do {                                                                                           \
        deadbeef->conf_get_int("ddbspy.msg_extension", 1)                                          \
            ? push_message(SPY_TYPE_MESSAGE_PUMP, HEADER_FORMAT " | " F "\n", search_ev_descr(id), \
                           id, (void*)ctx, p1, p2, __VA_ARGS__)                                    \
            : SIMPLE_MESSAGE_PUSH;                                                                 \
    } while (0)

static inline const char*
changed_to_str(const uint32_t type) {
    switch (type) {
    case DDB_PLAYLIST_CHANGE_CONTENT:
        return "content";
    case DDB_PLAYLIST_CHANGE_CREATED:
        return "create";
    case DDB_PLAYLIST_CHANGE_DELETED:
        return "deletes";
    case DDB_PLAYLIST_CHANGE_POSITION:
        return "position";
    case DDB_PLAYLIST_CHANGE_TITLE:
        return "title";
    case DDB_PLAYLIST_CHANGE_SELECTION:
        return "selection";
    case DDB_PLAYLIST_CHANGE_SEARCHRESULT:
        return "search result";
    case DDB_PLAYLIST_CHANGE_PLAYQUEUE:
        return "play queue";
    }
    return "unknown";
}

static inline void
trackinfo_to_str(ddb_event_track_t* T, uint32_t type, char* buf, size_t size) {
    switch (T->ev.event) {
    case DB_EV_SONGFINISHED:
        snprintf(buf, size, "%s: [%p], %s: [%.3f]", "from", T->track, "sec", T->playtime);
        break;
    case DB_EV_TRACKINFOCHANGED:
        snprintf(buf, size, "%s: [%p], %s: [%s]", "from", T->track, "type", changed_to_str(type));
        break;
    case DB_EV_CURSOR_MOVED:
        snprintf(buf, size, "%s: [%p], %s: [%s]", "from", T->track, "iter",
                 (type) ? "search" : "main");
        break;
    default:
        snprintf(buf, size, "%s: [%p]", "from", T->track);
    }
}

static inline void
trackchange_to_str(ddb_event_trackchange_t* T, char* buf, size_t size) {
    snprintf(buf, size, "%s: [%p], %s: [%p], %s: [%.3f]", "from", T->from, "to", T->to, "sec",
             T->playtime);
}

static int
spy_process(uint32_t id, uintptr_t ctx, uint32_t p1, uint32_t p2) {
    switch (id) {
    case DB_EV_NEXT:
    case DB_EV_PREV:
    case DB_EV_PLAY_CURRENT:
    case DB_EV_STOP:
    case DB_EV_PAUSE:
    case DB_EV_PLAY_RANDOM:
    case DB_EV_TERMINATE:
    case DB_EV_PLAYLIST_REFRESH:
    case DB_EV_REINIT_SOUND:
    case DB_EV_TOGGLE_PAUSE:
    case DB_EV_ACTIVATED:
    case DB_EV_VOLUMECHANGED:
    case DB_EV_OUTPUTCHANGED:
    case DB_EV_PLAYLISTSWITCHED:
    case DB_EV_DSPCHAINCHANGED:
#if (DDB_API_LEVEL >= 5)
    case DB_EV_SELCHANGED:
    case DB_EV_PLUGINSLOADED:
#endif
#if (DDB_API_LEVEL >= 8)
    case DB_EV_FOCUS_SELECTION:
#endif
#if (DDB_API_LEVEL >= 5)
    case DB_EV_TRACKFOCUSCURRENT:
#endif
        SIMPLE_MESSAGE_PUSH;
        break;

    case DB_EV_PLAY_NUM:
        COMPLEX_MESSAGE_PUSH("%s: [%u]", "num", p1);
        break;
    case DB_EV_PAUSED:
        COMPLEX_MESSAGE_PUSH("%s: [%s]", "сhange", (p1) ? "paused" : "unpaused");
        break;
    case DB_EV_PLAYLISTCHANGED:
        COMPLEX_MESSAGE_PUSH("%s: [%s]", "type", changed_to_str(p1));
        break;
    case DB_EV_SEEK:
        COMPLEX_MESSAGE_PUSH("%s: [%u]", "sec", p1 / 1000);
        break;
    case DB_EV_ACTIONSCHANGED:
        // TODO maybe more info of action
        SIMPLE_MESSAGE_PUSH;
        break;
    case DB_EV_CONFIGCHANGED:
        SIMPLE_MESSAGE_PUSH;
        // TODO diff config functionality
        break;
    case DB_EV_SONGCHANGED: {
        if (ctx > 0) {
            char* info = malloc(SPY_MESSAGE_SIZE * sizeof(char));
            if (info) {
                trackchange_to_str((ddb_event_trackchange_t*)ctx, info,
                                   SPY_MESSAGE_SIZE * sizeof(char));
                COMPLEX_MESSAGE_PUSH("%s", info);
                free(info);
            }
        } else {
            SIMPLE_MESSAGE_PUSH;
        }
        break;
    }
    case DB_EV_SONGSTARTED:
    case DB_EV_SONGFINISHED:
    case DB_EV_TRACKINFOCHANGED:
#if (DDB_API_LEVEL >= 10)
    case DB_EV_CURSOR_MOVED: {
#endif
        if (ctx > 0) {
            char* info = malloc(SPY_MESSAGE_SIZE * sizeof(char));
            if (info) {
                trackinfo_to_str((ddb_event_track_t*)ctx, p1, info,
                                 SPY_MESSAGE_SIZE * sizeof(char));
                COMPLEX_MESSAGE_PUSH("%s", info);
                free(info);
            }
        } else {
            SIMPLE_MESSAGE_PUSH;
        }
        break;
    }
    case DB_EV_SEEKED: {
        ddb_event_playpos_t* P = (ddb_event_playpos_t*)ctx;
        COMPLEX_MESSAGE_PUSH("%s: [%p], %s: [%.3f]", "from", P->track, "position", P->playpos);
        break;
    }
    default:
        SIMPLE_MESSAGE_PUSH;
    }

    return 0;
}
