#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

static int debug_mode = 0;

void log_message(LogLevel level, const char *format, ...) {
    va_list args;
    va_start(args, format);

    if (level != LOG_DEBUG || debug_mode == 1) vprintf(format, args);

    va_end(args);
}

void set_debug_mode(int mode) {
    debug_mode = mode;
}
