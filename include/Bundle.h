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
int addToBundle(const char *msg, const size_t msg_s);
void pokeBundle(void);

#endif
