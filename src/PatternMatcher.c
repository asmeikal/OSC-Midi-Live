#include <PatternMatcher.h>

#include <Serial_protocol.h>

#include <Debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/************************************************************
* Local functions declaration
************************************************************/

static int matchPattern(const char *string, const unsigned int string_l, const char *pattern, const char *lookup, const unsigned int pattern_l);

/************************************************************
* Function definition
************************************************************/

/**
 * Returns index of first legal message containing [var_n] variables, 
 * or -1 if there are none.
 */
int matchMessage(const char *buffer, const unsigned int buff_len, const unsigned int var_n)
{

    DEBUG_ASSERT(NULL != buffer, "matchMessage: null pointer argument\n");
    if(NULL == buffer) {
        errno = EINVAL;
        return -1;
    }

    DEBUG_ASSERT(MAX_VARS >= var_n, "matchMessage: var_n too big\n");
    if(MAX_VARS < var_n) {
        errno = EINVAL;
        return -1;
    }

    if(0 == buff_len) {
        return -1;
    }

    char pattern[MAX_MSG_LEN] = {0},
         lookup[MAX_MSG_LEN]  = {0};

    int pattern_l = MSG_LEN(var_n), i;

    pattern[0] = pattern[1] = MSG_START;
    lookup[0] = lookup[1] = LU_TRUE;

    for(i = 1; i <= var_n; ++i) {
        lookup[i*2] = LU_FALSE;
        pattern[(i*2)+1] = MSG_SEP;
        lookup[(i*2)+1] = LU_TRUE;
    }

    pattern[pattern_l-2] = pattern[pattern_l-1] = MSG_END;
    lookup[pattern_l-2] = lookup[pattern_l-1] = LU_TRUE;

    return matchPattern(buffer, buff_len, pattern, lookup, pattern_l);
}

struct match {
    int start;
    int len;
};

struct match matches[MAX_MSG_LEN] = {{0,0}};

/**
 * Returns start index of the first occurrence of [pattern] in [string], or
 * -1 if not found.
 * 
 * A pattern may look like "\xFE\xFE\x??\x00\x??\x00\xEF\xEF", where '\x??'
 * means 'any character'. This pattern will be specified as follows:
 * 
 *      pattern[] = "\xFE\xFE\x00\x00\x00\x00\xEF\xEF"
 *      lookup[]  = "\xFF\xFF\x00\xFF\x00\xFF\xFF\xFF"
 *      pattern_l = 8
 *
 * [lookup] masks which bytes are specified in the pattern. A non-zero
 * byte in [lookup] indicates that the byte in [pattern] is part of the 
 * matching sequence.
 *
 * Pre: 'pattern_l' <= MAX_MSG_LEN
 */
static int matchPattern(const char *string, const unsigned int string_l, const char *pattern, const char *lookup, const unsigned int pattern_l)
{

    DEBUG_ASSERT(NULL != pattern, "matchPattern: null pointer argument\n");
    DEBUG_ASSERT(NULL != string, "matchPattern: null pointer argument\n");
    DEBUG_ASSERT(NULL != lookup, "matchPattern: null pointer argument\n");

    if((NULL == pattern) || 
       (NULL == string) || 
       (NULL == lookup)) 
    {
        errno = EINVAL;
        return -1;
    }

    DEBUG_ASSERT(MAX_MSG_LEN >= pattern_l, 
        "matchPattern: pattern too long\n");
    if(MAX_MSG_LEN < pattern_l) {
        errno = EMSGSIZE;
        return -1;
    }

    if(pattern_l > string_l) {
        /* Pattern is longer than string: it'll never match */
        return -1;
    }

    if(0 == pattern_l) {
        return 0;
    }

    int matches_n = 0,
        i, j, k, pos;

    for(i = 0; i < string_l; ++i) {
        /* For each byte in the string... */

        /* ...move forward or discard each match */
        for(j = 0; j < matches_n; ++j) {
            pos = matches[j].len;

            if((0 == lookup[pos]) || 
               ((0 != lookup[pos]) && (string[i] == pattern[pos]))) 
            {
                /* Match matches (sic), keep it */
                ++matches[j].len;

                if(pattern_l == matches[j].len) {
                    return matches[j].start;
                }
            }
            else {
                /* Match doesn't match, remove it */
                for(k = j; k < matches_n; ++k) {
                    matches[k].start = matches[k+1].start;
                    matches[k].len = matches[k+1].len;
                }
                --matches_n;
                --j;
            }
        }

        /* ...possibly add a match starting here */
        if((0 == lookup[0]) || 
           ((0 != lookup[0]) && (string[i] == pattern[0])))
        {
            matches[matches_n].start = i;
            matches[matches_n].len = 1;
            ++matches_n;

            if(1 == pattern_l) {
                return matches[j].start;
            }
        }
    }

    return -1;
}
