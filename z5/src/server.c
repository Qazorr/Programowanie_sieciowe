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

#define BACKLOG_SIZE 8
#define PORT 2020
#define BUFFER 4
#define LINE_BUFFER 256
#define CR 13
#define LF 10
#define ul unsigned long
#define ERROR_MESSAGE "ERROR"

// #define DEBUG
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

typedef struct buffer {
    char info[BUFFER];
    short amount_read;
    short stop_index;
    bool finished;
} buffer_t;

typedef struct line {
    char info[LINE_BUFFER];
    bool error;
} line_t;

typedef enum error {
    NO_ERROR,
    BAD_ENDING_SEQUENCE,
    BAD_WHITESPACE,
    NUMBER_OVERFLOW,
    SUM_OVERFLOW
} error_t;

int counter = 0;
int server_fd, client_fd;
bool CR_detected = false;
buffer_t buffer = {.amount_read = 0, .stop_index = -1, .finished = false};
line_t line = {.error = false};

void buffer_reset()
{
    memset(buffer.info, 0, BUFFER);
    buffer.amount_read = 0;
    buffer.stop_index = -1;
    buffer.finished = false;
}

void line_reset()
{
    memset(line.info, 0, LINE_BUFFER);
    line.error = false;
}

void connection_ended()
{
    char* mess = "Connection timed out.\r\n";
    if(write(client_fd, mess, strlen(mess)) == -1) {
        perror("write error\n");
        exit(EXIT_FAILURE);
    }
    if(close(client_fd) == -1) {
        perror("close error\n");
        exit(EXIT_FAILURE);
    }
} 

void chunk_in_ascii()
{
    int i = 0;
    debug("\n------------------------------\n");
    for(i = 0; i<buffer.amount_read; i++) {
        printf("%c(%d) ", buffer.info[i], buffer.info[i]);
    }
    debug("------------------------------\n");
}

void whole_chunk_in_ascii()
{
    int i = 0;
    debug("\n------------------------------\n");
    debug("Chunk info [%d]\n", counter++);
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
    debug("\n------------------------------\n");
    debug("Line info\n");
    for(i = 0; i<strlen(line.info); i++) {
        if(line.info[i] == CR) debug("[%d] = CR\n", i);
        else if(line.info[i] == LF) debug("[%d] = LF\n", i); 
        else debug("[%d] = %c(%d)\n", i, line.info[i], line.info[i]);
    }
    debug("error=%d\n", line.error);
    debug("------------------------------\n");
}

//signalize if the read was interrupted
bool line_read()
{
    int i = 0;
    bool interrupted = false;

    // printf("CHUNK:\n%s", buffer.info);
    // printf("CHUNK: ");
    // chunk_in_ascii();
    // if(buffer.stop_index != -1) {
    //     strncpy(buffer.info, buffer.info+buffer.stop_index+1, buffer.amount_read);
    //     buffer.stop_index++;
    //     condition = buffer.amount_read-buffer.stop_index;
    //     buffer.stop_index = 0;
    //     whole_chunk_in_ascii();
    //     // i = buffer.stop_index+1;
    // }

    for(; i < buffer.amount_read; i++) {
        if(buffer.info[i] == CR) {
            CR_detected = true;
            debug("CR DETECTED\n");
        } 
        else if(buffer.info[i] == LF) {
            debug("LF DETECTED\n");
            if(CR_detected) {
                buffer.stop_index = i;
                CR_detected = false;
                interrupted = true;
                break;
            } else {
                CR_detected = false;
                line.error = true;
            }
        } else {
            if(CR_detected) {
                CR_detected = false;
                line.error = true;
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
    if(errno == ERANGE)
        line.error = true;
    else {
        if(will_overflow(num, *sum))
            line.error = true;
        else
            *sum += num;
    }
}

void line_interpret()
{
    ul sum = 0;
    char* number = strtok(line.info, " ");
    while(number) {
        debug("NUMBER = %s\n", number);
        safe_add(number, &sum);
        number = strtok(NULL, " ");
    }
    line.error ? sprintf(line.info, "%s\r\n", ERROR_MESSAGE) : sprintf(line.info, "%lu\r\n", sum);
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
            whole_chunk_in_ascii();
            if(line_read()) { 
                debug("LINE = %s\n", line.info);
                line_in_ascii();
                whole_chunk_in_ascii();

                line_interpret();
                write(client_fd, line.info, strlen(line.info));
                if(line.error) info("--AN ERROR OCCURED--\n");
                info("LINE SENT\n\n");

                memset(line.info, 0, LINE_BUFFER);
                debug("FINISHED = %d\n", buffer.finished);
                while(buffer.amount_read != 0) {
                    debug("Inside while loop\n");
                    if(line_read()) {
                        debug("LINE = %s\n", line.info);                
                        line_in_ascii();
                        line_interpret();
                        info("LINE SENT\n");
                        write(client_fd, line.info, strlen(line.info));
                        line_reset();

                    } else {
                        line_in_ascii();
                    }
                }
            } else {
                line_in_ascii();
            }
        }
        
        connection_ended();
    }
}