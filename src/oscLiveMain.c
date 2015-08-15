#include <Select.h>
#include <Sockets.h>
#include <ArduinoComms.h>
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

int main(int argc, char *argv[])
{
    char osc_address[256], name[16], *dev_name, *dev_vars;
    int rv, rvselect, dev_n = 3, fd, socket;
    MessageBuffer mb;

    DEBUG_ASSERT_CRITICAL(argc > 3, "usage: oscLiveMain [address] [baudrate] [devname:varn] ...\n");

    /* start the bundle buffer */
    (void) initBundleBuffer();

    /* start TCP connection */
    socket = createSocket(argv[1], 8000);
    DEBUG_ASSERT_CRITICAL(0 <= socket, "problem starting socket: %s\n", strerror(errno));
    rv = registerWriteFD(socket, sendAllBufferedBundles, NULL);
    DEBUG_ASSERT_CRITICAL(0 == rv, "can't register socket\n");

    /**
     * For all arduinos, run:
     *     fd = connectArduino("/dev/arduino_i_path");
     *     MessageBuffer mb_i = createMessageBuffer(N, "arduino_i");
     *     registerReadFD(fd, getMessage, 256, (void *) mb);
     */
    while(dev_n < argc) {
        dev_name = strtok(argv[dev_n], ":");
        DEBUG_ASSERT_CRITICAL(NULL != dev_name, "invalid device name\n");
        dev_vars = strtok(NULL, ":");
        DEBUG_ASSERT_CRITICAL(NULL != dev_vars, "invalid device name\n");
        fd = connectArduino(dev_name, (speed_t) atoi(argv[2]));
        DEBUG_ASSERT_CRITICAL(0 <= fd, "can't open serial %s: %s\n", argv[dev_n], strerror(errno));
        (void) snprintf(name, 16, "arduino%d", (dev_n - 2));
        mb = createMessageBuffer(atoi(dev_vars), name);
        rv = registerReadFD(fd, readFromArduino, 256, (void *) mb);
        DEBUG_ASSERT_CRITICAL(0 == rv, "can't register arduino #%d\n", (dev_n - 2));
        ++dev_n;
    }

    DEBUG_PRINT("started\n");

    while(1) {
        (void) buildReadFDSet();
        (void) buildWriteFDSet();

        rvselect = selectFDSet();
 
        if(0 == rvselect) {
            /* timeout */
            continue;
        }
        else if(0 < rvselect) {
            (void) fireCallBacks();
        }
        else {
            DEBUG_PRINT("select: %s\n", strerror(errno));
            continue;
        }
        (void) pokeBundle();
    }

    return 0;
}
