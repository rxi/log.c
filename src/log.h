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

/* Pre-C99 compatible definitions - BEGIN */
#define log_internal1(level, file, line, arg1)                                 \
            log_log(level, file, line, arg1);

#define log_internal2(level, file, line, arg1, arg2)                           \
            log_log(level, file, line, arg1, arg2);

#define log_internal3(level, file, line, arg1, arg2, arg3)                     \
            log_log(level, file, line, arg1, arg2, arg3);

#define log_internal4(level, file, line, arg1, arg2, arg3, arg4)               \
            log_log(level, file, line, arg1, arg2, arg3, arg4);

#define log_internal5(level, file, line, arg1, arg2, arg3, arg4, arg5)         \
            log_log(level, file, line, arg1, arg2, arg3, arg4, arg5);

#define log_trace1(arg1)                            log_internal1(LOG_TRACE, __FILE__, __LINE__, arg1);
#define log_trace2(arg1, arg2)                      log_internal2(LOG_TRACE, __FILE__, __LINE__, arg1, arg2);
#define log_trace3(arg1, arg2, arg3)                log_internal3(LOG_TRACE, __FILE__, __LINE__, arg1, arg2, arg3);
#define log_trace4(arg1, arg2, arg3, arg4)          log_internal4(LOG_TRACE, __FILE__, __LINE__, arg1, arg2, arg3, arg4);
#define log_trace5(arg1, arg2, arg3, arg4, arg5)    log_internal5(LOG_TRACE, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5);

#define log_debug1(arg1)                            log_internal1(LOG_DEBUG, __FILE__, __LINE__, arg1);
#define log_debug2(arg1, arg2)                      log_internal2(LOG_DEBUG, __FILE__, __LINE__, arg1, arg2);
#define log_debug3(arg1, arg2, arg3)                log_internal3(LOG_DEBUG, __FILE__, __LINE__, arg1, arg2, arg3);
#define log_debug4(arg1, arg2, arg3, arg4)          log_internal4(LOG_DEBUG, __FILE__, __LINE__, arg1, arg2, arg3, arg4);
#define log_debug5(arg1, arg2, arg3, arg4, arg5)    log_internal5(LOG_DEBUG, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5);

#define log_info1(arg1)                            log_internal1(LOG_INFO, __FILE__, __LINE__, arg1);
#define log_info2(arg1, arg2)                      log_internal2(LOG_INFO, __FILE__, __LINE__, arg1, arg2);
#define log_info3(arg1, arg2, arg3)                log_internal3(LOG_INFO, __FILE__, __LINE__, arg1, arg2, arg3);
#define log_info4(arg1, arg2, arg3, arg4)          log_internal4(LOG_INFO, __FILE__, __LINE__, arg1, arg2, arg3, arg4);
#define log_info5(arg1, arg2, arg3, arg4, arg5)    log_internal5(LOG_INFO, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5);

#define log_warn1(arg1)                            log_internal1(LOG_WARN, __FILE__, __LINE__, arg1);
#define log_warn2(arg1, arg2)                      log_internal2(LOG_WARN, __FILE__, __LINE__, arg1, arg2);
#define log_warn3(arg1, arg2, arg3)                log_internal3(LOG_WARN, __FILE__, __LINE__, arg1, arg2, arg3);
#define log_warn4(arg1, arg2, arg3, arg4)          log_internal4(LOG_WARN, __FILE__, __LINE__, arg1, arg2, arg3, arg4);
#define log_warn5(arg1, arg2, arg3, arg4, arg5)    log_internal5(LOG_WARN, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5);

#define log_error1(arg1)                            log_internal1(LOG_ERROR, __FILE__, __LINE__, arg1);
#define log_error2(arg1, arg2)                      log_internal2(LOG_ERROR, __FILE__, __LINE__, arg1, arg2);
#define log_error3(arg1, arg2, arg3)                log_internal3(LOG_ERROR, __FILE__, __LINE__, arg1, arg2, arg3);
#define log_error4(arg1, arg2, arg3, arg4)          log_internal4(LOG_ERROR, __FILE__, __LINE__, arg1, arg2, arg3, arg4);
#define log_error5(arg1, arg2, arg3, arg4, arg5)    log_internal5(LOG_ERROR, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5);

#define log_fatal1(arg1)                            log_internal1(LOG_FATAL, __FILE__, __LINE__, arg1);
#define log_fatal2(arg1, arg2)                      log_internal2(LOG_FATAL, __FILE__, __LINE__, arg1, arg2);
#define log_fatal3(arg1, arg2, arg3)                log_internal3(LOG_FATAL, __FILE__, __LINE__, arg1, arg2, arg3);
#define log_fatal4(arg1, arg2, arg3, arg4)          log_internal4(LOG_FATAL, __FILE__, __LINE__, arg1, arg2, arg3, arg4);
#define log_fatal5(arg1, arg2, arg3, arg4, arg5)    log_internal5(LOG_FATAL, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5);
/* Pre-C99 compatible definitions - END */


#define log_trace(...)                                                         \
            log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__);

#define log_debug(...)                                                         \
            log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__);

#define log_info(...)                                                          \
            log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__);

#define log_warn(...)                                                          \
            log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__);

#define log_error(...)                                                         \
            log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__);

#define log_fatal(...)                                                         \
            log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__);

const char *log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif
