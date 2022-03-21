#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> //sockaddr_in
#include <string.h>

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Wywolaj z portem: %s {port}\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *card = "Hello World\r\n";
    struct sockaddr_in  server, client;

    int port = atoi(argv[1]);

    int server_fd;
    if((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error\n");
        exit(EXIT_FAILURE);
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("bind error\n");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(struct sockaddr);
    while(1)
    {
        if(recvfrom(server_fd, NULL, 0, 0, (struct sockaddr *) &client, (socklen_t *) &addrlen) == -1) {
            perror("recvfrom error\n");
            exit(EXIT_FAILURE);
        }
        if(sendto(server_fd, card, strlen(card), 0, (struct sockaddr* )&client, (socklen_t) addrlen) == -1) {
            perror("sendto error\n");
            exit(EXIT_FAILURE);
        }
    }
}