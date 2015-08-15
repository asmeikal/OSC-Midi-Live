#include <ArduinoComms.h>

#include <MessageBuffer.h>
#include <Debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

/************************************************************
* Defines and macros
************************************************************/

#define MSG_END     "\000\000\377"

/************************************************************
* Local functions declaration
************************************************************/

static int parseBuffer(struct fd_info *fdi);
static int getValues(unsigned char *dst, unsigned char *src, unsigned int var_n, int end);

/************************************************************
* Functions definition
************************************************************/

/**
 * Opens the arduino file at 'path'.
 * Non-critical function: not allowed to fail.
 * Return <0 on failure.
 */
int connectArduino(char *path, speed_t speed)
{
    int fd = open(path, O_RDWR);
    DEBUG_ASSERT(0 <= fd, "open: %s\n", strerror(errno));
    if(0 > fd) {
        return -1;
    }

    struct termios settings;
    tcgetattr(fd, &settings);

    cfsetispeed(&settings, speed); /* input baud rate */
    cfsetospeed(&settings, speed); /* output baud rate */
    settings.c_cflag &= ~PARENB; /* no parity */
    settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
    // settings.c_cflag &= ~CSIZE;
    settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
    // settings.c_lflag = ICANON; /* canonical mode */
    // settings.c_oflag &= ~OPOST; /* raw output */

    tcsetattr(fd, TCSANOW, &settings); /* apply the settings */
    tcflush(fd, TCIOFLUSH);

    return fd;
  
}

/**
 * Callback function for arduino file descriptor.
 * Reads as much as possible from the fd, stores in the buffer, 
 * then parses the buffer contents.
 */
void readFromArduino(struct fd_info *fdi)
{
    DEBUG_ASSERT(NULL != fdi, "NULL pointer argument\n");
    if(NULL == fdi) {
        return;
    }

    int n;

    n = read(fdi->fd, fdi->buffer + fdi->buff_full, fdi->buff_size - fdi->buff_full);

    if(0 > n) {
        /* error */
        return;
    }
    else {
        fdi->buff_full += n;
    }

    (void) parseBuffer(fdi);

    (void) flushMessageBuffer((MessageBuffer) fdi->extra_info);

}


// TODO: if buffer is full of useless data?
/**
 * Reads through the buffer, discards illegal messages, and sends 
 * legal ones.
 */
static int parseBuffer(struct fd_info *fdi)
{
    DEBUG_ASSERT(NULL != fdi, "NULL pointer argument\n");
    if(NULL == fdi) {
        return -1;
    }

    MessageBuffer mb = (MessageBuffer) fdi->extra_info;
    unsigned int var_n = getVarNumber(mb), msg_size, free_space;
    int i, rv;

    msg_size = (var_n + 1) * 2;

    unsigned char *values = malloc(var_n);

    DEBUG_ASSERT(NULL != values, "malloc failed\n");
    if(NULL == values) {
        return -1;
    }

    int done = 0;

    while((3 < fdi->buff_full) && (!done)) {
        
        /* All messages end with the sequence 0x00 0x00 0xFF */

        /* Find the first occurrence of said sequence */
        for(i = 0; (i+3) <= fdi->buff_full; ++i) {
            if(0 == memcmp(fdi->buffer+i, MSG_END, 3)) {
                /* Get values from said sequence */
                rv = getValues(values, (unsigned char *) fdi->buffer, var_n, i);

                /* If sequence is legal, send values */
                if(0 == rv) {
                    (void) addToMessageBuffer(mb, values, var_n);
                }

                /* Shift buffer backwards */
                memmove(fdi->buffer, fdi->buffer + (i+3), fdi->buff_full - (i+3));
                fdi->buff_full -= (i+3);

                break;
            }
            else if((i+3) == fdi->buff_full) {
                /* We reached the end of buffer without results */
                done = 1;
            }
        }
    }

    free_space = fdi->buff_size - fdi->buff_full;
    if(msg_size > free_space) {
        memmove(fdi->buffer, fdi->buffer + (msg_size - free_space), msg_size - free_space);
    }

    free(values);

    return 0;
}

static int getValues(unsigned char *dst, unsigned char *src, unsigned int var_n, int end)
{
    DEBUG_ASSERT(NULL != dst, "NULL pointer argument\n");
    DEBUG_ASSERT(NULL != src, "NULL pointer argument\n");

    if((NULL == dst) || (NULL == src) || (end != ((var_n * 2) - 1))) {
        memset(dst, 0, var_n);
        return -1;
    }

    int i, k;

    for(i = 0, k = 0; i < end; ++i) {
        if(i % 2) { // pari, == 0
            if(0 != src[i]) {
                memset(dst, 0, var_n);
                return -1;
            }
        }
        else { // dispari, valore
            dst[k] = src[i];
            ++k;
        }
    }

    return 0;
}

