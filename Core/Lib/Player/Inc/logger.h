#ifndef STM32_FLAC_PLAYER_LOGGER_H

typedef enum {
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_SUCCESS,
    LOG_DEBUG
} LogLevel;

#define LOG_COLOR_ERROR   "\x1b[31m"
#define LOG_COLOR_WARN    "\x1b[33m"
#define LOG_COLOR_INFO    "\x1b[34m"
#define LOG_COLOR_SUCCESS "\x1b[32m"
#define LOG_COLOR_RESET   "\x1b[0m"

void log_message(LogLevel level, const char *format, ...);
void set_debug_mode(int mode);

#define log_error(format, ...) log_message(LOG_ERROR, LOG_COLOR_ERROR "[ERROR]: " format LOG_COLOR_RESET "\n", ##__VA_ARGS__)
#define log_warn(format, ...) log_message(LOG_WARN, LOG_COLOR_WARN "[WARN]: " format LOG_COLOR_RESET "\n", ##__VA_ARGS__)
#define log_info(format, ...) log_message(LOG_INFO, LOG_COLOR_INFO "[INFO]: " format LOG_COLOR_RESET "\n", ##__VA_ARGS__)
#define log_success(format, ...) log_message(LOG_SUCCESS, LOG_COLOR_SUCCESS "[SUCCESS]: " format LOG_COLOR_RESET "\n", ##__VA_ARGS__)
#define log_debug(format, ...) log_message(LOG_DEBUG, "[DEBUG]: " format "\n", ##__VA_ARGS__)

#endif //STM32_FLAC_PLAYER_LOGGER_H
