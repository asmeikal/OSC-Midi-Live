#include <PatternMatcher.h>

#include <Serial_protocol.h>
#include <Debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int var_n = 2;
    char msgs[] = "\x00\x0F\xFE\xFE\x01\x00\x01\x00\xEF\xEF\xFE\xFE\x00\x00\xEF\xEF\x67\xFE\x01\x00\x02\x00\xEF\xEF\xFE\xFE\xFE\x06\x00\x08\x00\xEF\xEF\x00";
    int len = 136/4, start, msg_len = MSG_LEN(var_n);

    fprintf(stderr, "Begin. Buffer:\n");
    full_print(msgs, len);
    fprintf(stderr, "\n");

    while(0 <= (start = matchMessage(msgs, len, var_n))) {
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
