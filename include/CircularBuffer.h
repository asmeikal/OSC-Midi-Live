#ifndef __CIRCULAR_BUFFER_H
#define __CIRCULAR_BUFFER_H

#include <stdio.h>

typedef struct circularBuffer *CircularBuffer;

// create buffer (size)
CircularBuffer createBuffer(const size_t size);
// add to buffer (void *, size)
int addToBuffer(CircularBuffer b, const void * el, const size_t el_s);
// void *pop top of buffer ()
int popTopOfBuffer(CircularBuffer b, void *res_c, size_t *res_s);

int isEmpty(CircularBuffer b);

#endif
