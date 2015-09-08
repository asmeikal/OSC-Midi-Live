/**
 * OSC-Midi-Live
 * Copyright (C) 2015 Michele Laurenti
 * email: asmeikal [at] me [dot] com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You can find a copy of the GNU General Public License at 
 * http://www.gnu.org/licenses/gpl.html
 */

#include <ArduinoComms.h>
#include <Debug.h>
#include <CopyrightNotice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) 
{

    copyright_notice("OSC-Midi-Live", 2015, "Michele Laurenti");

    DEBUG_ASSERT_CRITICAL(argc == 3, "usage: test_termios [dev_name] [baudrate]\n");

    int fd;
    fd = connectArduino(argv[1], atoi(argv[2]));

    DEBUG_ASSERT_CRITICAL(0 <= fd, "unable to open device: %s\n", strerror(errno));

    char buffer[256] = {0};
    int n;

    while(1) {
        n = read(fd, buffer, 256);
        full_print(buffer, n);
    }

    return 0;
}
