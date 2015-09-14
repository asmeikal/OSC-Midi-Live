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

#include "Drumpad_read.h"

/**
 * Pre: set [pin] to INPUT mode, with pinMode([pin], INPUT)
 */
int digitalReadOSC(const unsigned char pin)
{
  if(0 == digitalRead(pin)) {
    return READMIN;
  }
  else {
    return READMAX;
  }
}

/**
 * Pre: set [pin] to INPUT mode, with pinMode([pin], INPUT)
 */
int digitalReadOSCINV(const unsigned char pin)
{
  if(0 == digitalRead(pin)) {
    return READMAX;
  }
  else {
    return READMIN;
  }
}

int analogReadINV(const unsigned char pin) 
{
  return READMAX - analogRead(pin);
}

