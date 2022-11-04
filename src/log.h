/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifndef bool

#define bool    int
#define true    1
#define false   0

#endif


#define LOG_VERSION "0.1.0"

#ifndef RXI_LOGC_DEFAULT_LEVEL
#define RXI_LOGC_DEFAULT_LEVEL LOG_TRACE
#endif

#if defined __GNUC__
#define RXI_LOGC_PRINTF_ATTRIB(n, m) __attribute__((format(printf, n, m)))
#else
#define RXI_LOGC_PRINTF_ATTRIB(n, m)
#endif

enum {
    LOG_TRACE = -32,
    LOG_DEBUG = 0,
    LOG_INFO = 32,
    LOG_WARN = 64,
    LOG_ERROR = 96,
    LOG_FATAL = 128,
};

typedef struct {
    va_list ap;
    const char *fmt;
    const char *file;
    struct tm *time;
    struct tm time_buf;
    char      time_str[32];
    void *udata;
    int line;
    int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

#define log_trace(...)                                                         \
    do {                                                                       \
        if (LOG_TRACE >= RXI_LOGC_DEFAULT_LEVEL)                               \
            log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__);               \
    } while (0)
#define log_debug(...)                                                         \
    do {                                                                       \
        if (LOG_DEBUG >= RXI_LOGC_DEFAULT_LEVEL)                               \
            log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__);               \
    } while (0)
#define log_info(...)                                                          \
    do {                                                                       \
        if (LOG_INFO >= RXI_LOGC_DEFAULT_LEVEL)                                \
            log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__);                \
    } while (0)
#define log_warn(...)                                                          \
    do {                                                                       \
        if (LOG_WARN >= RXI_LOGC_DEFAULT_LEVEL)                                \
            log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__);                \
    } while (0)
#define log_error(...)                                                         \
    do {                                                                       \
        if (LOG_ERROR >= RXI_LOGC_DEFAULT_LEVEL)                               \
            log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__);               \
    } while (0)
#define log_fatal(...)                                                         \
    do {                                                                       \
        if (LOG_FATAL >= RXI_LOGC_DEFAULT_LEVEL)                               \
            log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__);               \
    } while (0)

const char *log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...)
    RXI_LOGC_PRINTF_ATTRIB(4, 5);

#endif
