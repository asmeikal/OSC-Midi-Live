#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

void full_print(const char *msg, const unsigned int size);

unsigned long long Debug_timestamp_millisec(void);
#define TIMESTAMP (Debug_timestamp_millisec())

#define ERROR_PRINT(...)    \
        do { \
            fprintf(stderr, "[timestamp=%llu] %s:%d: ", TIMESTAMP, __FILE__, __LINE__); \
            fprintf(stderr, __VA_ARGS__); \
            exit(EXIT_FAILURE); \
        } while(0)

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
    #define START_TIMING()      do { gettimeofday(&__dbg_bf, NULL); } while(0)
    #define PRINT_TIMER(n)      do { gettimeofday(&__dbg_af, NULL); \
                                __dbg_diff = (__dbg_af.tv_sec - __dbg_bf.tv_sec) * 1000 * 1000 + (__dbg_af.tv_usec - __dbg_bf.tv_usec); \
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
