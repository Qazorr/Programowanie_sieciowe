#include "log.h"

extern void (*dbg)(char *args, ...);
extern void (*inf)(char *args, ...);
extern buffer_t buffer;
extern line_t line;
extern int client_fd, client_number;

void _debug(char *args, ...) {
  printf("\033[1;31m[DEBUG]\033[0m ");
  va_list _args;
  va_start(_args, args);
  vprintf(args, _args); 
  va_end(_args);
}
void _info(char *args, ...) {
  printf("\033[1;32m[INFO]\033[0m ");
  va_list _args;
  va_start(_args, args);

  vprintf(args, _args);
  
  va_end(_args);
}
void _mode(char *args, ...) {
  printf("\033[1;36m[MODE]\033[0m ");
  va_list _args;
  va_start(_args, args);

  vprintf(args, _args);
  
  va_end(_args);
}
void _no_debug(char *args, ...) {}
void _no_info(char *args, ...) {}

void whole_chunk_in_ascii()
{
    int i = 0;
    (*dbg)("------------------------------\n");
    (*dbg)("Chunk info [index] = CHAR(ASCII)\n");
    for(i = 0; i<BUFFER; i++) {
        if(buffer.info[i] == CR) (*dbg)("[%d] = CR\n", i);
        else if(buffer.info[i] == LF) (*dbg)("[%d] = LF\n", i); 
        else (*dbg)("[%d] = %c(%d)\n", i, buffer.info[i], buffer.info[i]);
    }
    (*dbg)("stop=%d | read=%d\n", buffer.stop_index, buffer.amount_read);
    (*dbg)("------------------------------\n");
}
void line_in_ascii()
{
    int i = 0;
    (*dbg)("------------------------------\n");
    (*dbg)("Line info - [index] = CHAR(ASCII)\n");
    for(i = 0; i<strlen(line.info); i++) {
        if(line.info[i] == CR) (*dbg)("[%d] = CR\n", i);
        else if(line.info[i] == LF) (*dbg)("[%d] = LF\n", i); 
        else (*dbg)("[%d] = %c(%d)\n", i, line.info[i], line.info[i]);
    }
    (*dbg)("error=%s\n", name(line.status));
    (*dbg)("------------------------------\n");
}
void bad_arg_info()
{
    printf("%s-------------------------------------%s\n", YELLOW, RESET);
    printf("\033[1;31mProvided bad flag.%s\n%s-h/-H - help info\n-d/-D - show server debug info\n-v/-V - verbose server informations\n", RESET, YELLOW);
    printf("-------------------------------------%s\n", RESET);
    exit(EXIT_FAILURE);
}
void help_info()
{
    printf("%s-------------------------------------\n", YELLOW);
    printf("Additional flags to use with the program:\n-h/-H - help info\n-d/-D - show server debug info\n-v/-V - verbose server informations\n");
    printf("-------------------------------------%s\n", RESET);
    exit(EXIT_SUCCESS);
}
void mode_info()
{
    _mode("Server on port: %s%d%s\n", BOLD_GREEN, PORT, RESET);
    _mode("Maximum one-time read size: %s%d%s\n",BOLD_GREEN, BUFFER, RESET);
    _mode("Line lenght: %s%d%s\n", BOLD_GREEN, LINE_BUFFER, RESET);
    _mode("Number type used: %sUNSIGNED LONG%s\n", BOLD_GREEN, RESET);
    _mode("Client will be timed out after: %s%d SECONDS%s and %s%d MICROSECONDS%s\n\n", BOLD_GREEN, TIMEOUT_SEC, RESET, BOLD_GREEN, TIMEOUT_USEC, RESET);
}

char* name(error_t error)
{
    switch(error)
    {
        case NO_ERROR: return "NO_ERROR";
        case BAD_ENDING_SEQUENCE: return "BAD_ENDING_SEQUENCE";
        case SINGLE_LF_DETECTED: return "SINGLE_LF_DETECTED";
        case BAD_WHITESPACE: return "BAD_WHITESPACE";
        case EMPTY_LINE: return "EMPTY_LINE";
        case NUMBER_OVERFLOW: return "NUMBER_OVERFLOW";
        case SUM_OVERFLOW: return "SUM_OVERFLOW";
        case BAD_CHARACTER: return "BAD_CHARACTER";
        default: return "BAD_STATE";       
    }
}