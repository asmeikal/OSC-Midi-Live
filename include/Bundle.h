#ifndef __BUNDLE_H
#define __BUNDLE_H

#include <stdio.h>      /* For type size_t */
#include <sys/time.h>   /* For struct timeval & gettimeofday */

// bundle must record:
// - birth time
// - payload
// - payload length
struct Bundle {
    char *content;
    size_t size;
    size_t length;
    struct timeval birth;
};

// add To Bundle
int addToBundle(char *msg, size_t msg_s);
void pokeBundle(void);

#endif
