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

#include <PatternMatcher.h>

#include <Serial_protocol.h>
#include <Debug.h>
#include <CopyrightNotice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{

    copyright_notice("OSC-Midi-Live", 2015, "Michele Laurenti");

    int var_n = 2;
    char msgs[] = "\x00\x0F\xFE\xFE\x01\x00\x01\x00\xEF\xEF\xFE\xFE\x00\x00\xEF\xEF\x67\xFE\x01\x00\x02\x00\xEF\xEF\xFE\xFE\xFE\x06\x00\x08\x00\xEF\xEF\x00";
    int len = 136/4, start, msg_len = MSG_LEN(var_n);

    fprintf(stderr, "Begin. Buffer:\n");
    full_print(msgs, len);
    fprintf(stderr, "\n");

    while(0 <= (start = fastMatchMessage(msgs, len, var_n))) {
        fprintf(stderr, "start of pattern: %d\n", start);
        fprintf(stderr, "Matched:\n");
        full_print(msgs+start, msg_len);
        fprintf(stderr, "\n");

        memmove(msgs, msgs+(start + msg_len), len - (start + msg_len));
        len -= (start + msg_len);

        fprintf(stderr, "Remaining buffer:\n");
        full_print(msgs, len);
        fprintf(stderr, "\n");

    }

    fprintf(stderr, "All done.\n");


    return 0;
}
