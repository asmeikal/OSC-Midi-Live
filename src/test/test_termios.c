#include <ArduinoComms.h>
#include <Debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) 
{
    DEBUG_ASSERT_CRITICAL(argc == 3, "usage: test_termios [dev_name] [baudrate]\n");

    int fd;
    fd = connectArduino(argv[1], (speed_t) atoi(argv[2]));

    DEBUG_ASSERT_CRITICAL(0 <= fd, "unable to open device: %s\n", strerror(errno));

    char buffer[256] = {0};
    int n;

    while(1) {
        n = read(fd, buffer, 256);
        full_print(buffer, n);
    }

    return 0;
}
