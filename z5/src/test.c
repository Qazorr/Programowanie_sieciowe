#include <string.h>
#include <stdio.h>

int main()
{
    char test[128] = "1 2  3 4 5 6 7";
    char *number = strtok(test, " ");
    while(number) {
        printf("number = %s\n", number);
        number = strtok(NULL, " ");
    }
}