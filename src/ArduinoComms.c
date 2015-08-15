#include <ArduinoComms.h>

#include <MessageBuffer.h>
#include <Debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


/************************************************************
* Defines and macros
************************************************************/

#define MSG_END     "\000\000\377"

/************************************************************
* Local functions declaration
************************************************************/

static int parseBuffer(struct fd_info *fdi);
static int getValues(unsigned char *dst, unsigned char *src, unsigned int var_n, int end);

static speed_t getBaudRateFromInt(int baud);
static int getIntFromBaudRate(speed_t baud);

/************************************************************
* Functions definition
************************************************************/

/**
 * Opens the arduino file at 'path'.
 * Non-critical function: not allowed to fail.
 * Return <0 on failure.
 */
int connectArduino(char *path, int speed)
{
    int fd, rv;

    fd = open(path, O_RDWR | O_NOCTTY);
    DEBUG_ASSERT(0 <= fd, "open: %s\n", strerror(errno));
    if(0 > fd) {
        return -1;
    }

    struct termios tty;
    memset (&tty, 0, sizeof tty);

    rv = tcgetattr(fd, &tty);
    DEBUG_ASSERT(0 == rv, "tcgetattr: %s\n", strerror(errno));    
    if(0 != rv) {
        return -1;
    }

    speed_t s = getBaudRateFromInt(speed);
    DEBUG_ASSERT(-1 != s, "baud rate not valid\n");
    if(-1 == s) {
        return -1;
    }

    DEBUG_PRINT("%s: old IN speed:  %d\n", path, getIntFromBaudRate(cfgetispeed(&tty)));
    DEBUG_PRINT("%s: old OUT speed: %d\n", path, getIntFromBaudRate(cfgetospeed(&tty)));

    rv = cfsetispeed(&tty, s); /* input baud rate */
    DEBUG_ASSERT(0 == rv, "cfsetispeed: %s\n", strerror(errno));

    rv = cfsetospeed(&tty, s); /* output baud rate */
    DEBUG_ASSERT(0 == rv, "cfsetospeed: %s\n", strerror(errno));

    DEBUG_PRINT("%s: new IN speed:  %d\n", path, getIntFromBaudRate(cfgetispeed(&tty)));
    DEBUG_PRINT("%s: new OUT speed: %d\n", path, getIntFromBaudRate(cfgetospeed(&tty)));

    tty.c_cflag &= ~PARENB; /* no parity */
    tty.c_cflag &= ~CSTOPB; /* 1 stop bit */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; /* 8 bits */

#ifdef CRTSCTS
    tty.c_cflag &=  ~CRTSCTS; /* no flow control */
#endif

    tty.c_cflag |= CREAD | CLOCAL; /* turn on read & ignore ctrl */

    rv = tcflush(fd, TCIFLUSH);
    DEBUG_ASSERT(0 == rv, "tcflush: %s\n", strerror(errno));    
    if(0 != rv) {
        return -1;
    }

    rv = tcsetattr(fd, TCSANOW, &tty); /* apply the settings */
    DEBUG_ASSERT(0 == rv, "tcsetattr: %s\n", strerror(errno));    
    if(0 != rv) {
        return -1;
    }

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

/************************************************************
* Local functions
************************************************************/

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

    /* if there is not enough space for a full message, free enough space */
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

static speed_t getBaudRateFromInt(int baud)
{
    switch(baud) {
        case 0:
            return B0;
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 134:
            return B134;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        default:
            return -1;
    }

}

static int getIntFromBaudRate(speed_t baud)
{
    switch(baud) {
        case B0:
            return 0;
        case B50:
            return 50;
        case B75:
            return 75;
        case B110:
            return 110;
        case B134:
            return 134;
        case B150:
            return 150;
        case B200:
            return 200;
        case B300:
            return 300;
        case B600:
            return 600;
        case B1200:
            return 1200;
        case B1800:
            return 1800;
        case B2400:
            return 2400;
        case B4800:
            return 4800;
        case B9600:
            return 9600;
        case B19200:
            return 19200;
        case B38400:
            return 38400;
        case B57600:
            return 57600;
        case B115200:
            return 115200;
        case B230400:
            return 230400;
        default:
            return -1;
    }
}
