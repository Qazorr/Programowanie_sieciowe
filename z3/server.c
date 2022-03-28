#include "handler.h"

#define PORT 2020

int socket_fd;  //file descriptor for the server socket
message_t message;  //wrapper for the message from client
char* output_message;   //message to client

// denominates what'll happen when exit() is called
void ending_sequence(void)
{
    close(socket_fd);
    msg_free(&message);
    if(output_message == NULL) free(output_message);
    printf("\033[0m\nServer has been closed...\n");
}

// handler for the signal
static void sig_handler(int sigNum) { exit(EXIT_SUCCESS); }

int main(int argc, char* argv[])
{
    struct sockaddr_in server, client;

    if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error\n");
        exit(EXIT_FAILURE);
    }

    msg_init(&message);
    error_t status;
    status_init(&status);

    if(atexit(ending_sequence) == -1) {
        perror("atexit error\n");
        _exit(EXIT_FAILURE);
    }

    if(signal(SIGINT, sig_handler) == SIG_ERR) {
        perror("signal error\n");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(socket_fd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("bind error\n");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(socklen_t*);

    while(1)
    {
        if(recvfrom(socket_fd, message.info, BUFFER, 0, (struct sockaddr *) &client, (socklen_t *) &addrlen) == -1) {
            perror("recvfrom error\n");
            exit(EXIT_FAILURE);
        }

        output_message = summation_protocol(message, status);
        if(sendto(socket_fd, output_message, strlen(output_message), 0, (struct sockaddr* )&client, (socklen_t) addrlen) == -1) {
            perror("sendto error\n");
            exit(EXIT_FAILURE);
        }

        reset(&message, &status, output_message);
        memset(&client, 0, sizeof(client));
    }
}