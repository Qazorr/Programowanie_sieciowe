#include "handler.h"

#define PORT 2020

int main(int argc, char* argv[])
{
    int socket_fd;
    struct sockaddr_in server, client;

    if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error\n");
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
    
    char** values;
    uint32_t output_sum, no_message = 0;
    
    message_t message;
    error_t status;
    msg_init(&message);
    status_init(&status);

    char* output_message = malloc(sizeof(*output_message) * BUFFER);
    memset(output_message, 0, strlen(output_message));

    while(++no_message)
    {
        if(recvfrom(socket_fd, message.info, BUFFER, 0, (struct sockaddr *) &client, (socklen_t *) &addrlen) == -1) {
            perror("recvfrom error\n");
            exit(EXIT_FAILURE);
        }

        printf("[%d] RECEIVED: %s%s%s", no_message, GREEN, message.info, RESET);
        values = split(&message, &status);
        if(err_check(status)) 
            sprintf(output_message, "[%d] %s%s%s\r\n", no_message, RED, err_name(status.err), RESET);
        else {
            output_sum = sum(values, message.no_values, &status);
            if(err_check(status)) 
                sprintf(output_message, "[%d] %s%s%s\r\n", no_message, RED, err_name(status.err), RESET);
            else
                sprintf(output_message, "[%d] SUM = %s%u%s\r\n", no_message, GREEN, output_sum, RESET);
        }

        if(sendto(socket_fd, output_message, strlen(output_message), 0, (struct sockaddr* )&client, (socklen_t) addrlen) == -1) {
            perror("sendto error\n");
            exit(EXIT_FAILURE);
        }

        //clear everything
        reset(values, message, status, output_message);
        memset(&client, 0, sizeof(client));
    }
}