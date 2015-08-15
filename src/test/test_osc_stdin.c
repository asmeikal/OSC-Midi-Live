#include <Select.h>
#include <Sockets.h>
#include <MessageBuffer.h>
#include <BundleBuffer.h>
#include <Bundle.h>
#include <Debug.h>
#include <MemoryManipulation.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void test_callback(struct fd_info *fdi)
{
    int n, long_val, i;
    unsigned char val;
    char line[256] = {0};

    MessageBuffer mb = (MessageBuffer) fdi->extra_info;

    n = read(fdi->fd, (fdi->buffer)+(fdi->buff_full), (fdi->buff_size) - (fdi->buff_full));
    if(0 >= n) {
        DEBUG_PRINT("read %d bytes\n", n);
        return;
    }

    fdi->buff_full += n;

    for(i = 0; i < fdi->buff_full; ++i) {
        line[i] = (fdi->buffer)[i];
        if(line[i] == '\n') {
            line[i] = '\0';
            break;
        }
    }

    memmove(fdi->buffer, fdi->buffer+i+1, fdi->buff_full - i);

    fdi->buff_full -= i+1;

    sscanf(line, "%d", &long_val);

    val = (unsigned char) long_val;

    addToMessageBuffer(mb, &val, 1);

    flushMessageBuffer(mb);

}

int main(int argc, char *argv[])
{
    char osc_address[256];
    int rv, rvselect;

    if(argc > 1) {
        rv = safe_memcpy(osc_address, argv[1], strlen(argv[1]));
    }
    else {
        rv = safe_memcpy(osc_address, "127.0.0.1", strlen("127.0.0.1"));
    }
    DEBUG_ASSERT_CRITICAL(0 == rv, "copy failed\n");

    // start bundle buffer
    (void) initBundleBuffer();

    // create and register read FD (stdin)
    // create message buffer
    MessageBuffer mb = createMessageBuffer(1, "stdin");
    registerReadFD(0, test_callback, 256, (void *) mb);

    // create and register write FD (socket to localhost:8000)
    int socket = createSocket(osc_address, 8000);
    DEBUG_ASSERT_CRITICAL(0 <= socket, "Problem starting socket: %s\n", strerror(errno));

    registerWriteFD(socket, sendAllBufferedBundles, NULL);

    while(1) {
        buildReadFDSet();
        buildWriteFDSet();

        rvselect = selectFDSet();
 
        if(0 == rvselect) {
            /* timeout */
            continue;
        }
        else if(0 < rvselect) {
            fireCallBacks();
        }
        else {
            DEBUG_PRINT("select: %s\n", strerror(errno));
            continue;
        }
        pokeBundle();
    }

    return 0;
}
