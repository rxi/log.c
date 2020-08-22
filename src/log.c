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
  rolling_appender ra;
} Callback;

static struct {
  void *udata;
  log_LockFn lock;
  int level;
  bool quiet;
  Callback callbacks[MAX_CALLBACKS];
} L;


static const char *level_strings[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif


int get_next_available_callback() {
  for (int i = 0; i < MAX_CALLBACKS; i++)
    if (!L.callbacks[i].fn)
      return i;
  return -1;
}

static void stdout_callback(log_Event *ev) {
  char buf[16];
  buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#ifdef LOG_USE_COLOR
  fprintf(
    ev->udata, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
    buf, level_colors[ev->level], level_strings[ev->level],
    ev->file, ev->line);
#else
  fprintf(
    ev->udata, "%s %-5s %s:%d: ",
    buf, level_strings[ev->level], ev->file, ev->line);
#endif
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}


static void file_callback(log_Event *ev) {
  char buf[64];
  buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
  fprintf(
    ev->udata, "%s %-5s %s:%d: ",
    buf, level_strings[ev->level], ev->file, ev->line);
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}


static void rolling_appender_callback(log_Event *ev) {
  char* msg = (char*)calloc(strlen(ev->ra.file_name)+1024, sizeof(char));

  if (ev->udata == NULL)
    ev->udata = fopen(ev->ra.file_name, "a");
  if (ev->udata == NULL) {
    sprintf(msg, "Unable to open log file: %s", ev->ra.file_name);
    perror(msg);
    free(msg);
    return;
  }

  file_callback(ev);

  struct stat buf;
  if (stat(ev->ra.file_name, &buf) < 0) {
    sprintf(msg, "Unable to stat log file: %s", ev->ra.file_name);
    perror(msg);
    free(msg);
    return;
  }

  free(msg);

  if (buf.st_size >= ev->ra.max_log_size) {
    char* old = (char*)calloc(strlen(ev->ra.file_name)+10, sizeof(char));
    char* new = (char*)calloc(strlen(ev->ra.file_name)+10, sizeof(char));
    fclose(ev->udata);
    ev->udata = NULL;
    for (unsigned int i = ev->ra.max_logs-1; i >= 1; i--) {
      sprintf(old, "%s.%u", ev->ra.file_name, i);
      sprintf(new, "%s.%u", ev->ra.file_name, i+1);
      rename(old, new);
    }
    sprintf(new, "%s.1", ev->ra.file_name);
    rename(ev->ra.file_name, new);

    free(old);
    free(new);
  }
}


static void lock(void)   {
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


int log_add_callback(log_LogFn fn, void *udata, int level) {
  int nac = get_next_available_callback();
  if (nac < 0)
    return nac;

  L.callbacks[nac] = (Callback) { fn, udata, level, {0} };
  return 0;
}


int log_add_rolling_appender(rolling_appender ra, int level) {
  int nac = get_next_available_callback();
  if (nac < 0)
    return nac;

  L.callbacks[nac] = (Callback) { rolling_appender_callback, NULL, level, ra };
  return 0;
}


int log_add_fp(FILE *fp, int level) {
  return log_add_callback(file_callback, fp, level);
}


static void init_event(log_Event *ev, void *udata) {
  if (!ev->time) {
    time_t t = time(NULL);
    ev->time = localtime(&t);
  }
  ev->udata = udata;
}


void log_log(int level, const char *file, int line, const char *fmt, ...) {
  log_Event ev = {
    .fmt   = fmt,
    .file  = file,
    .line  = line,
    .level = level,
  };

  lock();

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
      ev.ra = cb->ra;
      cb->fn(&ev);
      cb->udata = ev.udata;
      va_end(ev.ap);
    }
  }

  unlock();
}
