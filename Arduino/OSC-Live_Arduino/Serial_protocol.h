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

#ifndef __SERIAL_PROTOCOL_H
#define __SERIAL_PROTOCOL_H

/**************************
* Defines & macros
**************************/

#define READMAX   1023U
#define READMIN   0
#define SENDMAX   255
#define SENDMIN   0

#define SCALE(x)    map((x), READMIN, READMAX, SENDMIN, SENDMAX)
#define MSGSIZE(x)  2+((x)*2)+2

#define STARTBITS 0xFE
#define MIDBITS   0x00
#define ENDBITS   0xEF

/**************************
* Structs & typedefs
**************************/

typedef int (*readPinValue)(unsigned char);

struct pin_info {
  unsigned char pin;
  readPinValue function;
};

/**************************
* Function definition
**************************/

void buildMessage(unsigned char message[], const struct pin_info pins[], const unsigned int pin_num);

#endif

