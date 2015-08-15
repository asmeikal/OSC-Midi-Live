#include <Debug.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void full_print(char *msg, unsigned int size)
{
#ifndef NDEBUG
    unsigned int i;
    for(i = 0; i < size; ++i) {
        if(isprint(msg[i]) && !isspace(msg[i])) {
            printf("%c ", msg[i]);
        }
        else {
            printf("\\%d ", (unsigned char) msg[i]);
        }
    }
    printf("\n");
#endif
}

unsigned long long Debug_timestamp_millisec(void) 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((unsigned long long)tv.tv_sec) * 1000 + 
        (((unsigned long long)tv.tv_usec) / 1000);
}
