/**
 * OSC-Midi-Live
 * Copyright (C) 2015 Michele Laurenti
 * email: asmeikal [at] me [dot] com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You can find a copy of the GNU General Public License at 
 * http://www.gnu.org/licenses/gpl.html
 */

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
