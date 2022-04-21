#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //
#include <sys/socket.h> // socket functions
#include <sys/time.h> 
#include <arpa/inet.h> //sockaddr_in
#include <string.h> // strlen
#include <stdbool.h> // true false
#include <string.h>
#include <stdarg.h> // va_list, va_start, vprintf, va_end
#include <errno.h> // changing errno
#include <limits.h> // ULONG_MAX
#include <ctype.h> // isdigit

#define MODE_AMOUNT 5

#define PORT 2020
#define BACKLOG_SIZE 8
#define TIMEOUT_SEC 5
#define TIMEOUT_USEC 0

#define BUFFER 16
#define LINE_BUFFER 1024

#define CR 13
#define LF 10
#define SPACE 32
#define ERROR_MESSAGE "ERROR"

#define RESET "\033[0m"
#define RED "\033[31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[32m"
#define BOLD_GREEN "\033[1;32m"
#define YELLOW "\033[33m"

#define ul unsigned long

typedef enum error {
    NO_ERROR,
    BAD_ENDING_SEQUENCE,
    SINGLE_LF_DETECTED,
    BAD_WHITESPACE,
    EMPTY_LINE,
    NUMBER_OVERFLOW,
    SUM_OVERFLOW,
    BAD_CHARACTER
} error_t;

typedef struct buffer {
    char info[BUFFER];
    short amount_read;
    short stop_index;
    bool space_detected;
} buffer_t;

typedef struct line {
    char info[LINE_BUFFER];
    bool digit_detected;
    error_t status;
} line_t;

typedef enum mode {
    BAD_ARGUMENT,
    NORMAL,
    HELP_INFO,
    INFO,
    DEBUG
} mode;

#endif