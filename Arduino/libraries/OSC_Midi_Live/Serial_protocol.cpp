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

#include "Serial_protocol.h"

void buildMessage(unsigned char message[], const struct pin_info pins[], const unsigned int pin_num)
{
  int i, tmp;
  
  message[0] = message[1] = STARTBITS;

  for(i = 0; i < pin_num; ++i) {
    tmp = pins[i].function(pins[i].pin);
    message[(i*2)+2] = SCALE(tmp);
    message[(i*2)+3] = MIDBITS;
  }
  
  message[MSGSIZE(pin_num)-2] = message[MSGSIZE(pin_num)-1] = ENDBITS;
  
}

