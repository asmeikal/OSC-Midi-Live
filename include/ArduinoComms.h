#ifndef __ARDUINO_COMMS_H
#define __ARDUINO_COMMS_H

#include <Select.h>

int connectArduino(char *path, int speed);
void readFromArduino(struct fd_info *fdi);

#endif
