#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> //sockaddr_in
#include <string.h>

#define BUFFER 128

int main(int argc, char* argv[])
{
    if(argc != 3) {
        printf("Wywolaj z portem i ip: %s {ip} {port}\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in  server;

    int port = atoi(argv[2]);

    int server_fd;
    if((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error\n");
        exit(EXIT_FAILURE);
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    int addrlen = sizeof(struct sockaddr), card_len;
    char *card = malloc(sizeof(char) * BUFFER);
    
    if(sendto(server_fd, NULL, 0, 0, (struct sockaddr* )&server, (socklen_t) addrlen) == -1) {
        perror("sendto error\n");
        exit(EXIT_FAILURE);
    }
    if((card_len = recvfrom(server_fd, card, BUFFER, 0, (struct sockaddr *) &server, (socklen_t *) &addrlen)) == -1) {
        perror("recvfrom error\n");
        exit(EXIT_FAILURE);
    }
    card[card_len] = '\0';
    printf("%s", card);

    if(close(server_fd) == -1) {
        perror("close error");
        exit(EXIT_FAILURE);
    }
    free(card);
}