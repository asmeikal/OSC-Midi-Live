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

#include <StringSplit.h>

#include <Debug.h>
#include <MemoryManipulation.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

static char **increase(char **array, unsigned int *current_size);

/************************************
* Debug Utiliy Function
************************************/

void printarray(char **strings) 
{
    unsigned int i = 0;
    printf("{\n");

    while (strings[i] != NULL) {
        printf("\t\"%s\",\n", strings[i]);
        ++i;
    }

    printf("\tNULL\n}\n");
}

/************************************
* Split function
************************************/

char **stringSplit(const char *string) 
{
    DEBUG_ASSERT(NULL != string, "stringSplit: null pointer argument\n");

    if(NULL == string) {
        errno = EINVAL;
        return NULL;
    }

    unsigned int i, w_start, w_count, w_size, size;
    i       = 0;
    w_start = 0;
    w_count = 0;
    size    = 2;

    char **result =  malloc(size * sizeof(char *));

    DEBUG_ASSERT(NULL != result, "stringSplit: malloc failed\n");
    if(NULL == result) {
        errno = ENOMEM;
        return NULL;
    }

    do {
        
        if((isspace(string[i])) || ('\0' == string[i])) {
            if(w_start != i) {
                w_size = i - w_start;
                result[w_count] = malloc(w_size+1);
                strncpy(result[w_count], &string[w_start], w_size);
                result[w_count][w_size] = '\0';

                ++w_count;
            }
            w_start = i+1;
        }

        if(w_count == size-1) {
            result = increase(result, &size);
            DEBUG_ASSERT(NULL != result, "stringSplit: malloc failed\n");
            if(NULL == result) {
                errno = ENOMEM;
                return NULL;
            }
        }

    } while (string[i++] != '\0');

    result[w_count] = NULL;

    return result;
}

/************************************
* Array resize function
************************************/

static char **increase(char **array, unsigned int *current_size) 
{
    DEBUG_ASSERT(NULL != array, "increase: null pointer argument\n");
    DEBUG_ASSERT(NULL != current_size, "increase: null pointer argument\n");
    
    if(NULL == array) {
        return NULL;
    }

    if(NULL == current_size) {
        free(array);
        return NULL;
    }

    char **result = malloc(*current_size * 2 * sizeof(char *));
    if (NULL == result) {
        free(array);
        return NULL;
    }

    int rv = safe_memcpy(result, array, *current_size * sizeof(char *));
    DEBUG_ASSERT(0 == rv, "memcpy failed\n");
    if(0 != rv) {
        free(result);
        return NULL;
    }

    *current_size *= 2;

    free(array);
    return result;
}
