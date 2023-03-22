#include <stdio.h>
#include "../log.h"

extern const char *level_strings[];

void callback1(log_Event *ev)
{
    char logline[128];
    snprintf(logline, 128, "%-5s %s:%d: ",
        level_strings[ev->level], ev->file, ev->line);
    vsnprintf(logline, 128, ev->fmt, ev->ap);
    printf("%s: %s\n", __func__, logline);
}

void callback2(log_Event *ev)
{
    char logline[128];
    snprintf(logline, 128, "%-5s %s:%d: ",
        level_strings[ev->level], ev->file, ev->line);
    vsnprintf(logline, 128, ev->fmt, ev->ap);
    printf("%s: %s\n", __func__, logline);
}

int main()
{
    log_trace("hello");
    log_debug("hello");
    log_info("hello");
    log_warn("hello");
    log_error("hello");
    log_fatal("hello");
    log_debug("============== update loglevel to FATAL =============");
    log_set_level(LOG_FATAL);
    log_trace("hello");
    log_debug("hello");
    log_info("hello");
    log_warn("hello");
    log_error("hello");
    log_fatal("hello");
    log_debug("===================== init log =====================");
    log_add_callback(callback1, "progname", LOG_TRACE);
    log_add_callback(callback2, "progname", LOG_TRACE);
    log_trace("hello");
    log_debug("hello");
    log_info("hello");
    log_warn("hello");
    log_error("hello");
    log_fatal("hello");
    log_debug("============ update loglevels of callbacks ========");
    log_set_level_callback(callback1, LOG_FATAL);
    log_set_level_callback(callback2, LOG_WARN);
    return 0;
}