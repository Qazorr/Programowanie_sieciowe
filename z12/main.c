#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Content-type:text/html\r\n\r\n");
    printf("<html>");
    printf("<head>");
    printf("<title>Hello from C</title>");
    printf("</head>");
    printf("<body>");
    printf("<h2>Hello from C!</h2>");
    printf("</body>");
    char* s = getenv("SERVER_PROTOCOL");
    printf("SERVER_PROTOCOL :%s\r\n",(s!=NULL)? s : "getenv returned NULL");
    s = getenv("REQUEST_METHOD");
    printf("REQUEST_METHOD :%s\r\n",(s!=NULL)? s : "getenv returned NULL");
    printf("</html>");
}