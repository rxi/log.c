#include "log.h"

int main(void) {
  log_trace("Hello %s", "world");

  printf("------------------------------------\n");
  log_trace("Hello %s", "trace");
  log_debug("Hello %s", "debug");
  log_info("Hello %s", "info");
  log_warn("Hello %s", "warn");
  log_error("Hello %s", "error");
  log_fatal("Hello %s", "fatal");

  printf("------------------------------------\n");
  log_set_quiet(true);
  log_trace("Hello %s", "trace");
  log_debug("Hello %s", "debug");
  log_info("Hello %s", "info");
  log_warn("Hello %s", "warn");
  log_error("Hello %s", "error");
  log_fatal("Hello %s", "fatal");

  printf("------------------------------------\n");
  log_set_quiet(false);
  log_set_level(LOG_ERROR);
  log_trace("Hello %s", "trace");
  log_debug("Hello %s", "debug");
  log_info("Hello %s", "info");
  log_warn("Hello %s", "warn");
  log_error("Hello %s", "error");
  log_fatal("Hello %s", "fatal");

  printf("------------------------------------\n");
  FILE *fp;
  fp = fopen("my_log", "w+");
  log_add_fp(fp, LOG_INFO);
  log_debug("Hello %s", "debug");
  log_info("Hello %s", "info");
  log_warn("Hello %s", "warn");

  fclose(fp);
  return 0;
}
