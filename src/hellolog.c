#include "log.h"

int main(int argc, char** argv) {
    //log_set_quiet(true);
    FILE *f1;
    FILE *f2;
    FILE *f3;
    FILE *f4;
    FILE *f5;
    FILE *f6;

    f1=fopen("/tmp/hellolog_level_debug.log", "w");
    f2=fopen("/tmp/hellolog_level_trace.log", "w");
    f3=fopen("/tmp/hellolog_level_info.log", "w");
    f4=fopen("/tmp/hellolog_level_warn.log", "w");
    f5=fopen("/tmp/hellolog_level_error.log", "w");
    f6=fopen("/tmp/hellolog_level_fatal.log", "w");

    log_add_fp(f1, LOG_DEBUG);
    log_add_fp(f2, LOG_TRACE);
    log_add_fp(f3, LOG_INFO);
    log_add_fp(f4, LOG_WARN);
    log_add_fp(f5, LOG_ERROR);
    log_add_fp(f6, LOG_FATAL);

    log_trace("TRACE - Hello %s", "world");
    log_debug("DEBUG - Hello %s", "world");
    log_info("INFO - Hello %s", "world");
    log_warn("WARNING - Hello %s", "world");
    log_error("ERROR - Hello %s", "world");
    log_fatal("FATAL - Hello %s", "world");

    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
    fclose(f5);
    fclose(f6);
}

