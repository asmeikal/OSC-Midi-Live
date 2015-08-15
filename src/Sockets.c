#include <Sockets.h>

#include <Debug.h>
#include <BundleBuffer.h>
#include <MemoryManipulation.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/************************************************************
* Defines
************************************************************/

#define MAX_BUNDLE_SIZE     256

/************************************************************
* Local variables
************************************************************/

char bundle_to_send[MAX_BUNDLE_SIZE] = {0};
size_t bundle_size = 0;
size_t bundle_sent = 0;

/************************************************************
* Function definition
************************************************************/

/**
 * Returns >= 0 on success, sets errno on failure.
 */
int createSocket(char *address, unsigned short port)
{
    int rv;
    errno = 0;

    DEBUG_ASSERT(NULL != address, "NULL pointer argument\n");
    if(NULL == address) {
        errno = EINVAL;
        return -1;
    }

    int fd;
    struct sockaddr_in addr = {0};

    fd = socket(PF_INET, SOCK_STREAM, 0);
    DEBUG_ASSERT(0 <= fd, "socket: %s\n", strerror(errno));
    if(0 > fd) {
        return fd;
    }

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    rv = inet_pton(addr.sin_family, address, &(addr.sin_addr));
    DEBUG_ASSERT(0 < rv, "invalid address '%s'\n", address);
    if(0 >= rv) {
        errno = EINVAL;
        return -1;
    }

    rv = connect(fd, (struct sockaddr *) &addr, sizeof (addr));
    DEBUG_ASSERT(0 <= rv, "connect: %s\n", strerror(errno));
    if (0 > rv) {
        return -1;
    }

    return fd;

}

// callback function
void sendAllBufferedBundles(struct fd_info *fdi)
{
    int n, rv;

    while((0 < (signed) (bundle_size - bundle_sent)) || 
          (!isEmptyBundleBuffer())) {

        if(0 == (bundle_size - bundle_sent)) {
            bundle_size = MAX_BUNDLE_SIZE;
            rv = getTopOfBundleBuffer(bundle_to_send, &bundle_size);
            bundle_sent = 0;
            if(rv < 0) {
                bundle_size = 0;
                return;
            }
        }

        n = send(fdi->fd, bundle_to_send+bundle_sent, bundle_size - bundle_sent, 0);

        if(0 >= n) {
            /* Error */
            DEBUG_PRINT("send: %s\n", strerror(errno));
            // TODO: should do something to avoid deadlock in case of error
            return;
        }

        bundle_sent += n;

    }
}
