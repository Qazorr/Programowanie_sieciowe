#include <stdio.h>
#include <unistd.h> //open, read, write, close
#include <stdlib.h> //exit, EXIT_FAILURE
#include <fcntl.h> //flagi
#include <string.h> //memset
#include <stdbool.h>

#define BUFF 1

int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("WYWOLANIE: %s file_in file_out\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int source;
    if((source = open(argv[1], O_RDONLY)) == -1) {
        perror("SOURCE OPEN ERROR\n");
        exit(EXIT_FAILURE);
    }
    
    int des;
    if((des = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1) {
        perror("DESTINATION OPEN ERROR\n");
        exit(EXIT_FAILURE);
    }

    char copy_buffer[BUFF];
    int amount_read = 0;

    bool skip = false;

    while((amount_read = read(source, copy_buffer, BUFF))) {
        if(amount_read == -1) {
            perror("READ ERROR\n");
            exit(EXIT_FAILURE);
        }

        if(!skip) {
            if(write(des, copy_buffer, amount_read) == -1) {
                perror("WRITE ERROR");
                exit(EXIT_FAILURE);
            }
        }

        if(copy_buffer[0] == '\n') {
            skip = !skip;
        }

        memset(copy_buffer, 0, BUFF);    
    }

    if(close(source) == -1 || close(des) == -1) {
        perror("CLOSE ERROR\n");
        exit(EXIT_FAILURE);
    }
}