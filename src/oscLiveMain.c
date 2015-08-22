#include <Select.h>
#include <Sockets.h>
#include <ArduinoComms.h>
#include <MessageBuffer.h>
#include <BundleBuffer.h>
#include <Bundle.h>
#include <Debug.h>
#include <MemoryManipulation.h>
#include <StringSplit.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(const int argc, const char *argv[])
{
    char osc_address[256], name[16], *dev_name, *dev_vars,
         *param_ip, *param_baudrate, **param_arduino;
    int rv, rvselect, dev_n = 0, arg_n, fd, socket, baud;
    MessageBuffer mb;

    DEBUG_ASSERT_CRITICAL((1 == argc) || (3 < argc), "usage: oscLiveMain [address] [baudrate] [devname:varn] ...\n");

    if(1 == argc) {
        char *tmp_arduino = malloc(256);
        param_ip = malloc(256);
        param_baudrate = malloc(256);
        DEBUG_ASSERT_CRITICAL(NULL != tmp_arduino, "malloc failed\n");
        DEBUG_ASSERT_CRITICAL(NULL != param_ip, "malloc failed\n");
        DEBUG_ASSERT_CRITICAL(NULL != param_baudrate, "malloc failed\n");

        fprintf(stdout, "[indirizzo ip]: ");
        rv = fscanf(stdin, " %256[^\n]", param_ip);
        DEBUG_ASSERT_CRITICAL(1 == rv, "fscanf failed\n");

        fprintf(stdout, "[baud rate]:    ");
        rv = fscanf(stdin, " %256[^\n]", param_baudrate);
        DEBUG_ASSERT_CRITICAL(1 == rv, "fscanf failed\n");

        fprintf(stdout, "[arduino]:      ");
        rv = fscanf(stdin, " %256[^\n]", tmp_arduino);
        DEBUG_ASSERT_CRITICAL(1 == rv, "fscanf failed\n");

        param_arduino = stringSplit(tmp_arduino);
        DEBUG_ASSERT_CRITICAL(NULL != param_arduino, "stringSplit failed\n");

        for(arg_n = 0; NULL != param_arduino[arg_n]; ++arg_n);
        free(tmp_arduino);
    }
    else if (argc >= 3) {
        param_ip = (char *) argv[1];
        param_baudrate = (char *) argv[2];
        param_arduino = (char **) argv+3;
        arg_n = argc - 3;
    }
    else {
        return -1;
    }

    /* start the bundle buffer */
    (void) initBundleBuffer();

    /* start TCP connection */
    socket = createSocket(param_ip, 8000);
    DEBUG_ASSERT_CRITICAL(0 <= socket, "problem starting socket: %s\n", strerror(errno));
    rv = registerWriteFD(socket, "TCP socket", sendAllBufferedBundles, NULL);
    DEBUG_ASSERT_CRITICAL(0 == rv, "can't register socket\n");

    /**
     * For all arduinos, run:
     *     fd = connectArduino("/dev/arduino_i_path", baud_rate);
     *     MessageBuffer mb_i = createMessageBuffer(N, "arduino_i");
     *     registerReadFD(fd, getMessage, 256, (void *) mb);
     */
    baud = atoi(param_baudrate);
    while(dev_n < arg_n) {
        dev_name = strtok(param_arduino[dev_n], ":");
        DEBUG_ASSERT_CRITICAL(NULL != dev_name, "invalid device name\n");
        dev_vars = strtok(NULL, ":");
        DEBUG_ASSERT_CRITICAL(NULL != dev_vars, "invalid device name\n");
        fd = connectArduino(dev_name, baud);
        DEBUG_ASSERT_CRITICAL(0 <= fd, "can't open serial %s: %s\n", param_arduino[dev_n], strerror(errno));
        (void) snprintf(name, 16, "arduino%d", dev_n);
        mb = createMessageBuffer(atoi(dev_vars), name);
        rv = registerReadFD(fd, dev_name, readFromArduino, 256, (void *) mb);
        DEBUG_ASSERT_CRITICAL(0 == rv, "can't register arduino #%d\n", dev_n);
        ++dev_n;
    }

    if(1 == argc) {
        free(param_ip);
        free(param_baudrate);
        int i = 0;
        while(NULL != param_arduino[i]) {
            free(param_arduino[i]);
            ++i;
        }
        free(param_arduino);
    }

    DEBUG_PRINT("started\n");

    // setup screen

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
        // refresh screen
    }

    return 0;
}
