/*
 * Copyright (c) 2020 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <sys/time.h>
#include "log.h"

#define MAX_CALLBACKS 32

typedef struct {
    log_LogFn fn;
    void *udata;
    int level;
} Callback;

static struct {
    void *udata;
    log_LockFn lock;
    int level;
    bool quiet;
    Callback callbacks[MAX_CALLBACKS];
} L = {
    NULL, NULL, RXI_LOGC_DEFAULT_LEVEL, false, {{0}},
};

static void time_to_str(char *);

static int year;
static int month;
static int day;
static int hour;
static int minutes;
static int seconds;
static int usec;

static struct timeval tv;
static struct tm *tm;


static const char *level_strings[] = {"[[TRACE]]", "[[DEBUG]]", "[[ INFO]]",
                                      "[[ WARN]]", "[[ERROR]]", "[[FATAL]]"};

static inline const char *get_level_string(int level) {
    return level_strings[(level + 32) / 32];
}

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {"\x1b[94m", "\x1b[36m", "\x1b[32m",
                                     "\x1b[33m", "\x1b[31m", "\x1b[35m"};

static inline const char *get_level_color(int level) {
    return level_colors[(level + 32) / 32];
}
#endif

static void stdout_callback(log_Event *ev) {
    //char buf[16];
    //buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
    char *buf = ev->time_str;

#ifdef LOG_USE_COLOR
    fprintf(ev->udata, "%s %s%-7s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", buf,
            get_level_color(ev->level), get_level_string(ev->level), ev->file,
            ev->line);
#else
    fprintf(ev->udata, "%s %-7s %s:%d: ", buf, get_level_string(ev->level),
            ev->file, ev->line);
#endif
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void file_callback(log_Event *ev) {
    //char buf[64];
    //buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
    char *buf = ev->time_str;

    fprintf(ev->udata, "%s %-7s %s:%d: ", buf, get_level_string(ev->level), ev->file, ev->line);

    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void lock(void) {
    if (L.lock) {
        L.lock(true, L.udata);
    }
}

static void unlock(void) {
    if (L.lock) {
        L.lock(false, L.udata);
    }
}

void log_set_lock(log_LockFn fn, void *udata) {
    L.lock = fn;
    L.udata = udata;
}

void log_set_level(int level) { L.level = level; }

void log_set_quiet(bool enable) { L.quiet = enable; }

int log_add_callback(log_LogFn fn, void *udata, int level) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (!L.callbacks[i].fn) {
            L.callbacks[i] = (Callback){fn, udata, level};
            return 0;
        }
    }
    return -1;
}

int log_add_fp(FILE *fp, int level) {
    return log_add_callback(file_callback, fp, level);
}

static void init_event(log_Event *ev, void *udata) {
    if (!ev->time) {
        time_t t = time(NULL);
        ev->time = localtime_r(&t, &ev->time_buf);
    }
    ev->udata = udata;
}

void log_log(int level, const char *file, int line, const char *fmt, ...) {
    log_Event ev = {
        .fmt = fmt,
        .file = file,
        .line = line,
        .level = level,
    };

    lock();

    time_to_str(ev.time_str);

    if (!L.quiet && level >= L.level) {
        init_event(&ev, stderr);
        va_start(ev.ap, fmt);
        stdout_callback(&ev);
        va_end(ev.ap);
    }

    for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++) {
        Callback *cb = &L.callbacks[i];
        if (level >= cb->level) {
            init_event(&ev, cb->udata);
            va_start(ev.ap, fmt);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    unlock();
}

/* Write time to buf in format YYYY-MM-DD HH:MM:SS.ms */
static void time_to_str(char *buf)
{
    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    /* Add 1900 to get the right year value read the manual page for localtime() */
    year    = tm->tm_year + 1900;
    /* Months are 0 indexed in struct tm */
    month   = tm->tm_mon + 1;
    day     = tm->tm_mday;
    hour    = tm->tm_hour;
    minutes = tm->tm_min;
    seconds = tm->tm_sec;
    usec    = tv.tv_usec;
    // msec
    // buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
    int len = sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d.%06d ",
                      year,
                      month,
                      day,
                      hour,
                      minutes,
                      seconds,
                      usec);
    buf[len] = '\0';
}

