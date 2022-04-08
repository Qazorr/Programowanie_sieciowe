#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <string.h>
#include <stdbool.h>

#define BUFFER 64
#define IP "127.0.0.1"
#define PORT 2020

int main(int argc, char* argv[])
{
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    
    int server_fd;
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    socklen_t addrlen = sizeof(server);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);

    if(connect(server_fd, (struct sockaddr*) &server, addrlen) == -1) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    char card[BUFFER] = "1 2 3";
    char card2[BUFFER]= " 4 5\r\n";

    if(write(server_fd, card, strlen(card)) == -1) {
        perror("write error");
        exit(EXIT_FAILURE);
    }

    sleep(2);

    if(write(server_fd, card2, strlen(card2)) == -1) {
        perror("write error");
        exit(EXIT_FAILURE);
    }

    if(read(server_fd, card, BUFFER) == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", card);

    if(close(server_fd) == -1) {
        perror("close error");
        exit(EXIT_FAILURE);
    }
}