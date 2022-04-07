#include <string.h>
#include <stdio.h>

int main()
{
    char test[128] = " \r\n";
    printf("%ld\n", strlen(test));
    int xd = 0;
    char *number = strtok(test, " ");
    while(number) {
        printf("%d\n", xd);
        xd++;
        printf("number = %s\n", number);
        number = strtok(NULL, " ");
    }
}