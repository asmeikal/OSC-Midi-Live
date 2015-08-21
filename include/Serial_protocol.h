#ifndef __SERIAL_PROTOCOL_H
#define __SERIAL_PROTOCOL_H

/**
 * A legal serial message contains [1-12] variables. The message has
 * the following format:
 *
 *     -------------------------- --- --------------------------
 *    | \xFE \xFE | BYTE | \x00 | ... | BYTE | \x00 | \xEF \xEF |
 *     -------------------------- --- --------------------------
 *        start     var1   sep.   ...   varN   sep.      end
 * 
 * I.e. there are two bytes for message start, two bytes for message
 * end, and two bytes for each variables. The first byte represents
 * the variable's value, the second byte is a separator byte.
 */

/* Message macros */
#define MSG_START   '\xFE'
#define MSG_END     '\xEF'
#define MSG_SEP     '\x00'

/* Message length */
#define MSG_LEN(vars)   2 + ((vars) * 2) + 2

/* Lookup macros */
#define LU_FALSE    '\x00'
#define LU_TRUE     '\xFF'

/* Each arduino has up to 12 analog pins */
#define MAX_VARS        12
#define MAX_MSG_LEN     MSG_LEN(MAX_VARS)

#endif
