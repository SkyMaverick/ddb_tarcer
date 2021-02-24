static inline size_t
filename_process(char *buf, size_t sz, const char* ext) {
    char date[sz / 2];
    time_t now = time(NULL);

    return (strftime(date, sz / 2, "%d.%m.%Y-%X", localtime(&now)))
               ? snprintf(buf, sz, "deadbeef_spy-%s.%s", date, ext)
               : 0;
}

