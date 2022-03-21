#include <stdio.h>
#include <stdbool.h>

bool drukowalne_ind(const void *buf, int len)
{
    int i;
    for(i = 0; i<len; i++) {
        if(((const char*)buf)[i] < 32 || ((const char*)buf)[i] > 126)
            return false;
    }
    return true;
}

bool drukowalne_wsk(const void *buf, int len)
{
    int i;
    for(i = 0; i<len; i++) {
        if(*(((const char*)buf) + i) < 32 || *(((const char*)buf) + i) > 126)
            return false;
    }
    return true;
}

bool drukowalne_ind2(const char *buf)
{
    int i = 0;

    while(buf[i] != '\0')
    {
        if(buf[i] < 32 || buf[i] > 126)
            return false;
        i++;
    }
    return true;
}

bool drukowalne_wsk2(const char *buf)
{
    int i = 0;
    while(*(buf + i) != '\0')
    {
        if(*(buf + i) < 32 || *(buf + i) > 126)
            return false;
        i++;
    }
    return true;
}

int main()
{
    printf("%d\n", drukowalne_ind("aącd/;'-", 8));
    printf("%d\n", drukowalne_ind2("abcd/;'-"));
    printf("%d\n", drukowalne_wsk("aącd/;'-", 8));
    printf("%d\n", drukowalne_wsk2("abcd/;'-"));
}