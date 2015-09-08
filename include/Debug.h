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

#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

void full_print(const char *msg, const unsigned int size);

unsigned long long Debug_timestamp_millisec(void);
#define TIMESTAMP (Debug_timestamp_millisec())

#define TV_DIFF_USEC(af,bf)   ((af).tv_sec - (bf).tv_sec) * 1000L * 1000L + \
                          ((af).tv_usec - (bf).tv_usec)

#ifndef NDEBUG
    #define ERROR_PRINT(...)    \
            do { \
                fprintf(stderr, "[timestamp=%llu] %s:%d: ", TIMESTAMP, __FILE__, __LINE__); \
                fprintf(stderr, __VA_ARGS__); \
                exit(EXIT_FAILURE); \
            } while(0)
#else
    #define ERROR_PRINT(...) \
            do { \
                fprintf(stderr, __VA_ARGS__); \
                exit(EXIT_FAILURE); \
            } while (0)
#endif

#define DEBUG_ASSERT_CRITICAL(cond, ...) \
        do { \
            if(!(cond)) {\
                ERROR_PRINT(__VA_ARGS__); \
            } \
        } while(0)

#ifndef NDEBUG

    #define SETUP_TIMER()       struct timeval __dbg_af = {0}, \
                                __dbg_bf = {0}; \
                                long long __dbg_diff
    #define START_TIMER()      do { gettimeofday(&__dbg_bf, NULL); } while(0)
    #define PRINT_TIMER(n)      do { gettimeofday(&__dbg_af, NULL); \
                                __dbg_diff = TV_DIFF_USEC(__dbg_af, __dbg_bf); \
                                DEBUG_PRINT("%s: %lld usecs\n", (n), __dbg_diff); \
                                } while (0)

    #define DEBUG_PRINT(...)    \
            do { \
                fprintf(stderr, "[timestamp=%llu] %s:%d: ", TIMESTAMP, __FILE__, __LINE__); \
                fprintf(stderr, __VA_ARGS__); \
            } while(0)

    #define DEBUG_ASSERT(cond, ...) \
        do { \
            if (! (cond) ) { \
                fprintf(stderr, "[timestamp=%llu] %s:%d: ", TIMESTAMP, __FILE__, __LINE__); \
                fprintf(stderr, __VA_ARGS__); \
            } \
            assert(cond); \
        } while(0)

#else

    #define SETUP_TIMER()           ((void) 0)
    #define START_TIMING()          ((void) 0)
    #define PRINT_TIMER(n)          ((void) 0)

    #define DEBUG_PRINT(...)        ((void) 0)
    #define DEBUG_ASSERT(cond, ...) ((void) 0)

#endif

#endif
