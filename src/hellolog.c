#include "log.h"

int main(int argc, char** argv) {
    log_set_quiet(true);
    FILE *f;
    f=fopen("/tmp/test.log", "a");
    log_add_fp(f, 1);
    log_trace("TRACE - Hello %s", "world");
    log_debug("DEBUG - Hello %s", "world");
    log_info("INFO - Hello %s", "world");
    log_warn("WARNING - Hello %s", "world");
    log_error("ERROR - Hello %s", "world");
    log_fatal("FATAL - Hello %s", "world");
}

