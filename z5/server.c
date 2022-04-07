#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h> //sockaddr_in
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define BACKLOG_SIZE 8
#define PORT 2020
#define BUFFER 16
#define LINE_BUFFER 256
#define CR 13
#define LF 10
#define SPACE 32
#define ul unsigned long
#define ERROR_MESSAGE "ERROR"

#define DEBUG
#define INFO

#ifdef DEBUG
void debug(char *args, ...) {
  printf("\033[1;31m[DEBUG]\033[0m ");
  va_list _args;
  va_start(_args, args);
  vprintf(args, _args); 
  va_end(_args);
}
#else
void debug(char *args, ...) { }
#endif

#ifdef INFO
void info(char *args, ...) {
  printf("\033[1;32m[INFO]\033[0m ");
  va_list _args;
  va_start(_args, args);

  vprintf(args, _args);
  
  va_end(_args);
}
#else
void info() { }
#endif

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
    bool digit_detected;
    bool finished;
} buffer_t;

typedef struct line {
    char info[LINE_BUFFER];
    error_t status;
} line_t;


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

int server_fd, client_fd;
bool CR_detected = false;
buffer_t buffer = {.amount_read = 0, .stop_index = -1, .finished = false, .space_detected = false, .digit_detected = false};
line_t line = {.status = NO_ERROR};

void buffer_reset()
{
    memset(buffer.info, 0, BUFFER);
    buffer.amount_read = 0;
    buffer.stop_index = -1;
    buffer.finished = false;
    buffer.digit_detected = false;
}

void line_reset()
{
    memset(line.info, 0, LINE_BUFFER);
    line.status = NO_ERROR;
}

void connection_lost(const char* cause)
{
    // char* mess = "Connection timed out.\r\n";
    char* mess = malloc(sizeof(*mess) * 64);
    sprintf(mess, "Connection %s\r\n", mess == NULL ? "lost." : cause);
    if(write(client_fd, mess, strlen(mess)) == -1) {
        perror("write error\n");
        exit(EXIT_FAILURE);
    }
    if(close(client_fd) == -1) {
        perror("close error\n");
        exit(EXIT_FAILURE);
    }
    free(mess);
} 

void whole_chunk_in_ascii()
{
    int i = 0;
    debug("------------------------------\n");
    debug("Chunk info [index] = CHAR(ASCII)\n");
    for(i = 0; i<BUFFER; i++) {
        if(buffer.info[i] == CR) debug("[%d] = CR\n", i);
        else if(buffer.info[i] == LF) debug("[%d] = LF\n", i); 
        else debug("[%d] = %c(%d)\n", i, buffer.info[i], buffer.info[i]);
    }
    debug("finished=%d | stop=%d | read=%d\n", buffer.finished, buffer.stop_index, buffer.amount_read);
    debug("------------------------------\n");
}

void line_in_ascii()
{
    int i = 0;
    debug("------------------------------\n");
    debug("Line info - [index] = CHAR(ASCII)\n");
    for(i = 0; i<strlen(line.info); i++) {
        if(line.info[i] == CR) debug("[%d] = CR\n", i);
        else if(line.info[i] == LF) debug("[%d] = LF\n", i); 
        else debug("[%d] = %c(%d)\n", i, line.info[i], line.info[i]);
    }
    debug("error=%s\n", name(line.status));
    debug("------------------------------\n");
}

bool err_check() { return line.status != NO_ERROR; }
void err_set(error_t error) { if(line.status == NO_ERROR) line.status = error; }

//signalize if the read was interrupted
bool line_read()
{
    int i = 0;
    bool interrupted = false;

    for(; i < buffer.amount_read; i++) {
        if(buffer.info[i] == CR) {
            CR_detected = true;
            debug("CR DETECTED ON [%d]\n", i);
            if(buffer.space_detected) {
                debug("ERROR OCCURED ON LINE 171\n");
                line.status = BAD_WHITESPACE;
            }
        } 
        else if(buffer.info[i] == LF) {
            debug("LF DETECTED ON [%d]\n", i);
            if(CR_detected) {
                buffer.stop_index = i;
                CR_detected = false;
                interrupted = true;
                break;
            } else {
                CR_detected = false;
                debug("ERROR OCCURED ON LINE 185\n");
                line.status = SINGLE_LF_DETECTED;
            }
        } else {
            if(CR_detected) {
                CR_detected = false;
                line.status = BAD_ENDING_SEQUENCE;
                buffer.space_detected = false;
                debug("ERROR OCCURED ON LINE 193\n");
            } else if(!isdigit(buffer.info[i])) {
                if(buffer.info[i] == SPACE) {
                    if(!buffer.space_detected) {
                        buffer.space_detected = true;
                    } else {
                        line.status = BAD_WHITESPACE;
                        debug("ERROR OCCURED ON LINE 198\n");  
                    }
                } else {
                    debug("NON-DIGIT FOUND ON [%d]\n", i);
                    line.status = BAD_CHARACTER;
                    buffer.space_detected = false;
                    debug("ERROR OCCURED ON LINE 202");
                } 
            } else {
                buffer.space_detected = false;
                buffer.digit_detected = true;
            }
        }
    }
    if(!interrupted) {
        strncat(line.info, buffer.info, buffer.amount_read);
        buffer_reset();
        buffer.finished = true;
    } else {
        strncat(line.info, buffer.info, buffer.stop_index+1);
        strncpy(buffer.info, buffer.info+buffer.stop_index+1, buffer.amount_read);
        buffer.amount_read -= (buffer.stop_index+1);
        buffer.stop_index = 0;
        buffer.finished = false;
    } 
    return interrupted;
}

bool will_overflow(ul a, ul b) { return a > ULONG_MAX - b; }

void safe_add(char* number, ul* sum)
{
    ul num = strtoul(number, NULL, 10);
    if(errno == ERANGE) {
        line.status = NUMBER_OVERFLOW;
        errno = 0;
    }
    else {
        if(will_overflow(num, *sum))
            line.status = SUM_OVERFLOW;
        else
            *sum += num;
    }
}

void line_interpret()
{
    ul sum = 0;
    if(buffer.digit_detected) {
        if(line.info[0] == SPACE)
            line.status = BAD_WHITESPACE;
        else {
            char* number = strtok(line.info, " ");
            while(number) {
                debug("NUMBER = %s\n", number);
                safe_add(number, &sum);
                number = strtok(NULL, " ");
            }
        }
    } else {
        line.status = EMPTY_LINE;  
    }
    err_check() ? sprintf(line.info, "%s\r\n", ERROR_MESSAGE) : sprintf(line.info, "%lu\r\n", sum);
}

void set_timeout(int seconds, int microseconds)
{
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = microseconds;
    socklen_t timeout_len = sizeof(struct timeval);

    if(setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timeout, timeout_len) == -1) {
        perror("setsockopt error\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[])
{
    struct sockaddr_in  server, client;
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error\n");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("bind error\n");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, BACKLOG_SIZE) == -1) {
        perror("listen error\n");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(struct sockaddr);

    while(1)
    {
        if((client_fd = accept(server_fd, (struct sockaddr *) &client, (socklen_t *) &addrlen)) == -1) {
            perror("accept error\n");
            exit(EXIT_FAILURE);
        }

        // set_timeout(5, 0); //set timeout on 5 seconds
    
        while((buffer.amount_read = read(client_fd, buffer.info, BUFFER)) != 0)
        {
            if(buffer.amount_read == -1) break; //timeout
            info("READ \033[1;32m%d\033[0m BYTES OF DATA\n", buffer.amount_read);
            //whole_chunk_in_ascii();
            if(line_read()) { 
                debug("LINE = %s\n", line.info);
                line_in_ascii();
                //whole_chunk_in_ascii();

                line_interpret();
                if(err_check()) info("--\033[1;33m%s\033[0m OCCURED--\n", name(line.status));
                write(client_fd, line.info, strlen(line.info));
                info("LINE SENT\n\n");
                line_reset();

                memset(line.info, 0, LINE_BUFFER);
                debug("FINISHED = %d\n", buffer.finished);
                while(buffer.amount_read != 0) {
                    debug("Inside while loop\n");
                    if(line_read()) {
                        debug("LINE = %s\n", line.info);                
                        //line_in_ascii();
                        line_interpret();
                        info("LINE SENT\n");
                        write(client_fd, line.info, strlen(line.info));
                        line_reset();

                    } else {
                        //line_in_ascii();
                    }
                }
            } else {
                line_in_ascii();
            }
        }
        
        if(buffer.amount_read == -1) {
            connection_lost("timed out");
        } else {
            connection_lost(NULL);
        }
    }
}