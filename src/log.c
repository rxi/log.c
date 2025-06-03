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
  bool graphics; // Enable .nfo-style ASCII art
  bool colors;   // Enable colored text
  bool blink;    // Enable text blinking
  bool bold;     // Enable bold text
  bool underline;// Enable underlined text
  bool reverse;  // Enable reverse video
  Callback callbacks[MAX_CALLBACKS];
} L;

static const char *level_strings[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

static void stdout_callback(log_Event *ev) {
  // Apply attributes if enabled
  const char *attr = L.blink || L.bold || L.underline || L.reverse ? LEVEL_ATTRIBUTES[ev->level] : "";

  if (L.graphics) {
    // Print ASCII art, colored if colors are enabled
    fprintf(ev->udata, "%s%s%s%s",
            L.colors ? LEVEL_COLORS[ev->level] : "",
            attr,
            LOG_GRAPHICS[ev->level],
            L.colors ? ANSI_RESET : "");
  }

  // Colored output with attributes
  if (L.colors) {
    fprintf(
      ev->udata, "%s%s%-5s%s %s:%d: %s",
      attr,
      LEVEL_COLORS[ev->level], level_strings[ev->level],
      FILELINE_COLORS[ev->level], ev->file, ev->line,
      MESSAGE_COLORS[ev->level]);
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "%s\n", ANSI_RESET);
  } else {
    // Plain text output with attributes
    fprintf(
      ev->udata, "%s%-5s %s:%d: ",
      attr, level_strings[ev->level], ev->file, ev->line);
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "%s\n", L.blink || L.bold || L.underline || L.reverse ? ANSI_RESET : "");
  }
  fflush(ev->udata);
}

static void file_callback(log_Event *ev) {
  fprintf(
    ev->udata, "%-5s %s:%d: ",
    level_strings[ev->level], ev->file, ev->line);
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}

static void lock(void) {
  if (L.lock) { L.lock(true, L.udata); }
}

static void unlock(void) {
  if (L.lock) { L.lock(false, L.udata); }
}

const char* log_level_string(int level) {
  return level_strings[level];
}

void log_set_lock(log_LockFn fn, void *udata) {
  L.lock = fn;
  L.udata = udata;
}

void log_set_level(int level) {
  L.level = level;
}

void log_set_quiet(bool enable) {
  L.quiet = enable;
}

void log_style(uint32_t style) {
  L.graphics = (style & LOG_STYLE_NFO) != 0;
  L.colors = (style & LOG_STYLE_COLOR) != 0;
  L.blink = (style & LOG_STYLE_BLINK) != 0;
  L.bold = (style & LOG_STYLE_BOLD) != 0;
  L.underline = (style & LOG_STYLE_UNDERLINE) != 0;
  L.reverse = (style & LOG_STYLE_REVERSE) != 0;
}

int log_add_callback(log_LogFn fn, void *udata, int level) {
  for (int i = 0; i < MAX_CALLBACKS; i++) {
    if (!L.callbacks[i].fn) {
      L.callbacks[i] = (Callback) { fn, udata, level };
      return 0;
    }
  }
  return -1;
}

int log_add_fp(FILE *fp, int level) {
  return log_add_callback(file_callback, fp, level);
}

void log_log(int level, const char *file, int line, const char *fmt, ...) {
  log_Event ev = {
    .fmt   = fmt,
    .file  = file,
    .line  = line,
    .level = level,
    .udata = stderr
  };

  lock();

  if (!L.quiet && level >= L.level) {
    va_start(ev.ap, fmt);
    stdout_callback(&ev);
    va_end(ev.ap);
  }

  for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++) {
    Callback *cb = &L.callbacks[i];
    if (level >= cb->level) {
      ev.udata = cb->udata;
      va_start(ev.ap, fmt);
      cb->fn(&ev);
      va_end(ev.ap);
    }
  }

  unlock();
}
