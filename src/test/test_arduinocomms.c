#include <ArduinoComms.h>

#include <Debug.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MSG_END "\x00\x00\xFF"

int getValues(unsigned char *dst, unsigned char *src, unsigned int var_n, int end);

int main(void) {

    int i, rv;

    unsigned int length = 55, var_n = 4;
    unsigned char msg[] = "\x40\x00\x43\x00\x42\x00\x48\x00\x00\xFF\x55\x00\x00\xFF\x51\x00\x4F\x00\x50\x00\x55\x00\x00\xFF\x89\x00\x00\x78\x00\x45\x00\x99\x00\x54\x00\x00\xFF\x65\x00\x45\x00\x65\x00\x45\x00\x65\x00\x45\x00\x65\x00\x45\x00\x00\xFF";
    full_print((char *) msg, length);

    unsigned char *values = malloc(var_n);

    int done = 0;

    while((0 < length) && (!done)) {
        printf("current length: %d\n", length);
        for(i = 0; i <= length - 3; ++i) {
            if(0 == memcmp(msg+i, MSG_END, 3)) {
                rv = getValues(values, msg, var_n, i);

                printf("getValues : %d\n", rv);
                memmove(msg, msg+i+3, length - i - 3);
                length -= i + 3;
                break;
            }
            else if(i == length - 3) {
                done = 1;
            }
        }
        printf("String after function call:\n");
        full_print((char *) msg, length);
        printf("Values returned:\n");
        full_print((char *) values, var_n);
        printf("\n");
    }

    printf("Done!\n");
    full_print((char *) msg, length);

    free(values);

    return 0;
}

int getValues(unsigned char *dst, unsigned char *src, unsigned int var_n, int end)
{
    if(end != ((var_n * 2) - 1)) {
        memset(dst, 0, var_n);
        return -1;
    }

    int i, k;

    for(i = 0, k = 0; i < end; ++i) {
        if(i % 2) { // pari, == 0
            if(0 != src[i]) {
                memset(dst, 0, var_n);
                return -1;
            }
        }
        else { // dispari, valore
            dst[k] = src[i];
            ++k;
        }
    }

    return 0;
}

