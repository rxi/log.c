#include <stdio.h>
#include "../log.h"

extern const char *level_strings[];

void callback1(log_Event *ev)
{
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
    fprintf(stdout, "callback 1 %s %-5s %s:%d: ", buf, level_strings[ev->level], ev->file, ev->line);
    vfprintf(stdout, ev->fmt, ev->ap);
    fprintf(stdout, "\n");
    fflush(stdout);
}

void callback2(log_Event *ev)
{
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
    fprintf(stdout, "callback 2 %s %-5s %s:%d: ", buf, level_strings[ev->level], ev->file, ev->line);
    vfprintf(stdout, ev->fmt, ev->ap);
    fprintf(stdout, "\n");
    fflush(stdout);
}

int main()
{
    log_trace("hello");
    log_debug("hello");
    log_info("hello");
    log_warn("hello");
    log_error("hello");
    log_fatal("hello");
    printf("============== update loglevel to FATAL =============\n");
    log_set_level(LOG_FATAL);
    log_trace("hello");
    log_debug("hello");
    log_info("hello");
    log_warn("hello");
    log_error("hello");
    log_fatal("hello");
    printf("===================== add callback =================\n");
    log_add_callback(callback1, "progname", LOG_TRACE);
    log_add_callback(callback2, "progname", LOG_TRACE);
    log_trace("hello");
    log_debug("hello");
    log_info("hello");
    log_warn("hello");
    log_error("hello");
    log_fatal("hello");
    printf("============ update loglevels of callbacks ========\n");
    log_set_level_callback(callback1, LOG_FATAL);
    log_set_level_callback(callback2, LOG_WARN);
    log_trace("hello");
    log_debug("hello");
    log_info("hello");
    log_warn("hello");
    log_error("hello");
    log_fatal("hello");
    return 0;
}