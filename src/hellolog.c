#include "log.h"

int main(int argc, char** argv) {
    FILE *f1;
    FILE *f2;
    FILE *f3;
    FILE *f4;
    FILE *f5;
    FILE *f6;

    log_set_quiet(true);

    f1=fopen("/tmp/hellolog_level_trace.log", "w");
    f2=fopen("/tmp/hellolog_level_debug.log", "w");
    f3=fopen("/tmp/hellolog_level_info.log", "w");
    f4=fopen("/tmp/hellolog_level_warn.log", "w");
    f5=fopen("/tmp/hellolog_level_error.log", "w");
    f6=fopen("/tmp/hellolog_level_fatal.log", "w");

    log_add_fp(f1, LOG_TRACE);
    log_add_fp(f2, LOG_DEBUG);
    log_add_fp(f3, LOG_INFO);
    log_add_fp(f4, LOG_WARN);
    log_add_fp(f5, LOG_ERROR);
    log_add_fp(f6, LOG_FATAL);

    log_trace2("TRACE - Hello %s", "world");
    log_debug2("DEBUG - Hello %s", "world");
    log_info2("INFO - Hello %s", "world");
    log_warn2("WARNING - Hello %s", "world");
    log_error2("ERROR - Hello %s", "world");
    log_fatal2("FATAL - Hello %s", "world");

    /* Pre-C99 usage example
    log_trace3("test 3 args: %s %s", "arg1", "arg2");
    */
    log_log(LOG_ERROR, __FILE__, __LINE__, "test 3 args: %s %s", "arg1", "arg2");

    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
    fclose(f5);
    fclose(f6);

    return 0;
}

