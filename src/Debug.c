#include <Debug.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void full_print(const char *msg, const unsigned int size)
{
#ifndef NDEBUG
    unsigned int i;
    for(i = 0; i < size; ++i) {
        printf("%02X ", (unsigned char) msg[i]);
        if(((i+1) % 4) == 0) {
            printf(" ");
        }
        if(((i+1) % 16) == 0) {
            printf("\n");
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
