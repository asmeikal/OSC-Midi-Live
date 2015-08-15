#ifndef __ARDUINO_COMMS_H
#define __ARDUINO_COMMS_H

#include <Select.h>

#include <termios.h>
#include <unistd.h>

int connectArduino(char *path, speed_t speed);
void readFromArduino(struct fd_info *fdi);

#endif
