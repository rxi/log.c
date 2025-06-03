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
#include <stdbool.h>
#include <stdint.h>

#define LOG_VERSION "0.1.0"

// Style flags (Windows-like bitmasks)
#define LOG_STYLE_NFO      0x00000001 // Enable .nfo-style ASCII art
#define LOG_STYLE_COLOR    0x00000002 // Enable colored text
#define LOG_STYLE_BLINK    0x00000004 // Enable text blinking
#define LOG_STYLE_BOLD     0x00000008 // Enable bold text
#define LOG_STYLE_UNDERLINE 0x00000010 // Enable underlined text
#define LOG_STYLE_REVERSE  0x00000020 // Enable reverse video

// ANSI color and attribute codes
#define ANSI_COLOR_BLACK        "\x1b[30m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_CYAN         "\x1b[36m"
#define ANSI_COLOR_WHITE        "\x1b[37m"
#define ANSI_COLOR_BRIGHT_BLACK "\x1b[90m"
#define ANSI_COLOR_BRIGHT_RED   "\x1b[91m"
#define ANSI_COLOR_BRIGHT_GREEN "\x1b[92m"
#define ANSI_COLOR_BRIGHT_YELLOW "\x1b[93m"
#define ANSI_COLOR_BRIGHT_BLUE  "\x1b[94m"
#define ANSI_COLOR_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_COLOR_BRIGHT_CYAN  "\x1b[96m"
#define ANSI_COLOR_BRIGHT_WHITE "\x1b[97m"
#define ANSI_BG_BLACK           "\x1b[40m"
#define ANSI_BG_RED             "\x1b[41m"
#define ANSI_BG_GREEN           "\x1b[42m"
#define ANSI_BG_YELLOW          "\x1b[43m"
#define ANSI_BG_BLUE            "\x1b[44m"
#define ANSI_BG_MAGENTA         "\x1b[45m"
#define ANSI_BG_CYAN            "\x1b[46m"
#define ANSI_BG_WHITE           "\x1b[47m"
#define ANSI_BG_BRIGHT_BLACK    "\x1b[100m"
#define ANSI_BG_BRIGHT_RED      "\x1b[101m"
#define ANSI_BG_BRIGHT_GREEN    "\x1b[102m"
#define ANSI_BG_BRIGHT_YELLOW   "\x1b[103m"
#define ANSI_BG_BRIGHT_BLUE     "\x1b[104m"
#define ANSI_BG_BRIGHT_MAGENTA  "\x1b[105m"
#define ANSI_BG_BRIGHT_CYAN     "\x1b[106m"
#define ANSI_BG_BRIGHT_WHITE    "\x1b[107m"
#define ANSI_BOLD               "\x1b[1m"
#define ANSI_FAINT              "\x1b[2m"
#define ANSI_ITALIC             "\x1b[3m"
#define ANSI_UNDERLINE          "\x1b[4m"
#define ANSI_BLINK_SLOW         "\x1b[5m"
#define ANSI_BLINK_RAPID        "\x1b[6m"
#define ANSI_REVERSE            "\x1b[7m"
#define ANSI_HIDDEN             "\x1b[8m"
#define ANSI_STRIKETHROUGH      "\x1b[9m"
#define ANSI_RESET              "\x1b[0m"

// ASCII art for each log level (simplified .nfo-style graphics)
static const char *LOG_GRAPHICS[] = {
    "════ TRACE ════\n", // TRACE
    "════ DEBUG ════╗\n", // DEBUG
    "════ INFO  ════\n", // INFO
    "════ WARN  ═\n", // WARN
    "════ ERROR ════\n", // ERROR
    "════ FATAL ════╗\n"  // FATAL
};

//#define Color arrays for log level, file:line, and message
static const char *LEVEL_COLORS[] = {
    ANSI_COLOR_BRIGHT_GREEN,  // TRACE
    ANSI_COLOR_CYAN,         // DEBUG
    ANSI_COLOR_YELLOW,        // INFO
    ANSI_COLOR_RED,       // WARN
    ANSI_COLOR_MAGENTA,          // ERROR
    ANSI_COLOR_BRIGHT_RED   // FATAL
};
static const char* FILELINE_COLORS[] = {
    ANSI_COLOR_BRIGHT_BLACK, // TRACE
    ANSI_COLOR_BRIGHT_BLACK, // DEBUG
    ANSI_COLOR_BRIGHT_BLACK, // INFO
    ANSI_COLOR_RED, // WARN
    ANSI_COLOR_RED, // ERROR
    ANSI_COLOR_RED // FATAL
};
static const char *MESSAGE_COLORS[] = {
    ANSI_COLOR_CYAN,         // TRACE
    ANSI_COLOR_BLUE,         // DEBUG
    ANSI_COLOR_CYAN,         // INFO
    ANSI_COLOR_YELLOW,       // WARN
    ANSI_COLOR_RED,          // ERROR
    ANSI_COLOR_MAGENTA       // FATAL
};

// Attribute arrays for log levels (e.g., blink, bold)
static const char *LEVEL_ATTRIBUTES[] = {
    "",                     // TRACE: none
    ANSI_BOLD,              // DEBUG: bold
    "",                     // INFO: none
    ANSI_UNDERLINE,         // WARN: underline
    ANSI_BLINK_SLOW,        // ERROR: slow blink
    ANSI_REVERSE            // FATAL: reverse video
};

typedef struct {
    va_list ap;
    const char *fmt;
    const char *file;
    void *udata;
    int line;
    int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

const char* log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
void log_style(uint32_t style);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif
