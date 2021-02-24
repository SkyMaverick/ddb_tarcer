#if defined(__linux__) || defined(__gnu_linux__)
    #include <linux/limits.h>
#endif

#ifndef NAME_MAX
    #define NAME_MAX 128
#endif
#define FDFILE(X) (FILE *)(X)

static inline size_t
filename_process(char *buf, size_t sz, const char *ext) {
    char date[sz / 2];
    time_t now = time(NULL);

    return (strftime(date, sz / 2, "%d.%m.%Y-%X", localtime(&now)))
               ? snprintf(buf, sz, "deadbeef_spy-%s.%s", date, ext)
               : 0;
}
