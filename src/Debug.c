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
