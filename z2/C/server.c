#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> //sockaddr_in
#include <string.h>

#define BACKLOG_SIZE 8

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Wywolaj z portem: %s {port}\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *card = "Hello World\r\n";
    struct sockaddr_in  server, client;
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));


    int port = atoi(argv[1]);
    int server_fd; //deskryptor do socketu serwera
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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
    if(listen(server_fd, BACKLOG_SIZE) == -1) {
        perror("listen error\n");
        exit(EXIT_FAILURE);
    }

    int client_fd, bytes, addrlen = sizeof(struct sockaddr);
    while(1)
    {
        if((client_fd = accept(server_fd, (struct sockaddr *) &client, (socklen_t *) &addrlen)) == -1) {
            perror("accept error\n");
            exit(EXIT_FAILURE);
        }

        if((bytes = write(client_fd, card, strlen(card))) == -1) {
            perror("write error\n");
            exit(EXIT_FAILURE);
        }

        if(close(client_fd) == -1) {
            perror("close error\n");
            exit(EXIT_FAILURE);
        }
    }
}