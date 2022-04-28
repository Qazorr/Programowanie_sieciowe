#include "protocol.h"

struct sockaddr_in server, client;
int server_fd = 0, client_fd = 0, client_number = 0;
bool CR_detected = false;
buffer_t buffer = {.amount_read = 0, .stop_index = -1, .space_detected = false, };
line_t line = {.status = NO_ERROR, .digit_detected = false};
void (*dbg)(char *args, ...);
void (*inf)(char *args, ...);

void err_set(error_t error) { if(line.status == NO_ERROR) line.status = error; }
bool err_check() { return line.status != NO_ERROR; }
void buffer_reset()
{
    memset(buffer.info, 0, BUFFER);
    buffer.amount_read = 0;
    buffer.stop_index = -1;
}
void line_reset()
{
    memset(line.info, 0, LINE_BUFFER);
    line.digit_detected = false;
    line.status = NO_ERROR;
}

bool will_overflow(ul a, ul b) { return a > ULONG_MAX - b; }

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
void connection_lost(const char* cause)
{
    (*inf)("[%d] CONNECTION WITH CLIENT %s\n\n", client_number, cause);
    if(close(client_fd) == -1) {
        perror("close error\n");
        exit(EXIT_FAILURE);
    }
} 

char* my_strncpy(char* destination, const char* source, size_t num)
{
    // return NULL if no memory is allocated to the destination
    if (destination == NULL)
        return NULL;
 
    char* ptr = destination; // store the pointer to the c-string beginning
 
    // copy first 'num' characters of c-string pointed by source to destination
    while (*source && num--)
    {
        if(*destination != '\0') {
            *destination = *source;
            destination++;
        }
        source++;
    }
 
    *destination = '\0';
 
    return ptr;
}
char* my_strncat(char* destination, const char* source, size_t num)
{
    char* ptr = destination + strlen(destination); // go to the end of the destination c-string
 
    while (*source != '\0' && num--) //while we can append the source characters we append them to the end of a destination
        *ptr++ = *source++;
 
    *ptr = '\0';
 
    return destination;
}

bool line_read()
{
    int i = 0;
    bool interrupted = false;

    for(; i < buffer.amount_read; i++) {
        if(buffer.info[i] == '\0') {
            (*dbg)("\\0 DETECTED ON [%d]\n", i);
            err_set(BAD_CHARACTER);
        }
        if(buffer.info[i] == CR) {
            if(!CR_detected) {
                CR_detected = true;
                (*dbg)("CR DETECTED ON [%d]\n", i);
                if(buffer.space_detected) {
                    err_set(BAD_WHITESPACE);
                }
            } 
            else {
                (*dbg)("SECOND CR DETECTED ON [%d]", i);
                err_set(BAD_ENDING_SEQUENCE);
            }
        } 
        else if(buffer.info[i] == LF) {
            (*dbg)("LF DETECTED ON [%d]\n", i);
            if(CR_detected) {
                buffer.stop_index = i;
                CR_detected = false;
                interrupted = true;
                break;
            } else {
                CR_detected = false;
                err_set(SINGLE_LF_DETECTED);
            }
        } else {
            if(CR_detected) {
                CR_detected = false;
                err_set(BAD_ENDING_SEQUENCE);
                buffer.space_detected = false;
            } else if(!isdigit(buffer.info[i])) {
                if(buffer.info[i] == SPACE) {
                    if(!buffer.space_detected) {
                        buffer.space_detected = true;
                    } else {
                        err_set(BAD_WHITESPACE);
                    }
                } else {
                    (*dbg)("NON-DIGIT FOUND ON [%d]\n", i);
                    err_set(BAD_CHARACTER);
                    buffer.space_detected = false;
                } 
            } else {
                buffer.space_detected = false;
                line.digit_detected = true;
            }
        }
    }
    if(!interrupted) {
        my_strncat(line.info, buffer.info, buffer.amount_read);
        buffer_reset();
    } else {
        my_strncat(line.info, buffer.info, buffer.stop_index+1);
        my_strncpy(buffer.info, buffer.info+buffer.stop_index+1, buffer.amount_read);
        buffer.amount_read -= (buffer.stop_index+1);
        buffer.stop_index = 0;
    } 
    return interrupted;
}
void safe_add(char* number, ul* sum)
{
    ul num = strtoul(number, NULL, 10);
    if(errno == ERANGE) {
        err_set(NUMBER_OVERFLOW);
        errno = 0;
    }
    else {
        if(will_overflow(num, *sum))
            err_set(SUM_OVERFLOW);
        else
            *sum += num;
    }
}
void line_interpret()
{
    ul sum = 0;
    if(line.digit_detected) {
        if(line.info[0] == SPACE) {
            err_set(BAD_WHITESPACE);
        }
        else {
            char* number = strtok(line.info, " ");
            while(number) {
                safe_add(number, &sum);
                number = strtok(NULL, " ");
            }
        }
    } else
        err_set(EMPTY_LINE);

    err_check() ? sprintf(line.info, "%s\r\n", ERROR_MESSAGE) : sprintf(line.info, "%lu\r\n", sum);
}

int convert_to_mode(char* argv)
{
    if(!strcmp(argv, "-h") || !strcmp(argv, "-H")) return HELP_INFO;
    else if(!strcmp(argv, "-d") || !strcmp(argv, "-D")) return DEBUG;
    else if(!strcmp(argv, "-v") || !strcmp(argv, "-V")) return INFO;
    else return BAD_ARGUMENT;
}
void handle_arguments(int argc, char* argv[])
{
    if(argc > 1) {
        int i;
        bool modes[MODE_AMOUNT] = {false, false, false, false, false};
        for(i = 1; i<argc; i++) modes[convert_to_mode(argv[i])] = true;
        if(modes[BAD_ARGUMENT]) bad_arg_info();
        if(modes[HELP_INFO]) help_info();
        if(modes[INFO]) { _mode("SERVER-INFO %sENABLED%s\n", BOLD_GREEN, RESET); inf = &_info; } else inf = &_no_info;
        if(modes[DEBUG]) { _mode("SERVER-DEBUG %sENABLED%s\n", BOLD_GREEN, RESET); dbg = &_debug; } else dbg = &_no_debug;
    } else {
        printf("%s[HELP] You can run the program with -h or -H flag for additional help info.%s\n", YELLOW, RESET);
        inf = &_no_info;
        dbg = &_no_debug;
    }
}

void server_init(int argc, char* argv[])
{
    handle_arguments(argc, argv);
    mode_info();

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
}
void summation_protocol()
{
    int addrlen = sizeof(struct sockaddr);
    while(++client_number)
    {
        if((client_fd = accept(server_fd, (struct sockaddr *) &client, (socklen_t *) &addrlen)) == -1) {
            perror("accept error\n");
            exit(EXIT_FAILURE);
        }

        (*inf)("[%d] CLIENT CONNECTED\n", client_number);

        set_timeout(TIMEOUT_SEC, TIMEOUT_USEC); //set timeout on 5 seconds
    
        while((buffer.amount_read = read(client_fd, buffer.info, BUFFER)) != 0)
        {
            if(buffer.amount_read == -1) break; //timeout
            (*inf)("[%d] READ \033[1;32m%d\033[0m BYTES OF DATA\n", client_number, buffer.amount_read);
            if(line_read()) { 
                (*dbg)("LINE = %s\n", line.info);
                line_in_ascii();
                line_interpret();
                fflush(stdout);
                if(err_check()) (*inf)("[%d] --\033[1;33m%s\033[0m OCCURED--\n", client_number, name(line.status));
                else (*inf)("[%d] --%sPROTOCOL SUCCEDED%s--\n", client_number, BOLD_GREEN, RESET);
                write(client_fd, line.info, strlen(line.info));
                (*inf)("[%d] LINE SENT: %s", client_number, line.info);
                line_reset();

                memset(line.info, 0, LINE_BUFFER);
                while(buffer.amount_read != 0) {
                    if(line_read()) {
                        (*dbg)("LINE = %s\n", line.info);                
                        line_interpret();
                        (*inf)("LINE SENT: %s", line.info);
                        write(client_fd, line.info, strlen(line.info));
                        line_reset();
                    }
                }
            } else {
                line_in_ascii();
            }
        }
        
        if(buffer.amount_read == -1) {
            connection_lost("TIMED OUT");
        } else {
            connection_lost("LOST");
        }
    }
}