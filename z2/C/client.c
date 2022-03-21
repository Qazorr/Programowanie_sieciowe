#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <string.h>
#include <stdbool.h>

#define BUFFER 64

char* drukowalne(const char *buf)
{
    int i = 0, j = 0;
    char *temp = malloc(sizeof(char)*BUFFER);
    while(*(buf + i) != '\0')
    {
        if(*(buf + i) >= 32 && *(buf + i) <= 126) {
            *(temp + j) = *(buf + i);
            j++;
        }
        i++;
    }
    return temp;
}

int main(int argc, char* argv[])
{
    if(argc != 3) {
        printf("Zle wywolanie: %s {ip} {port}", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port;
    port = atoi(argv[2]);

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    
    int server_fd;
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    socklen_t addrlen = sizeof(server);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if(connect(server_fd, (struct sockaddr*) &server, addrlen) == -1) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    char *card = malloc(sizeof(char) * BUFFER);

    if(read(server_fd, card, BUFFER) == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    card = drukowalne(card);
    printf("%s\n", card);

    if(close(server_fd) == -1) {
        perror("close error");
        exit(EXIT_FAILURE);
    }

    free(card);
}