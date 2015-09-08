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
#include "Fast_analogRead.h"
#include "Serial_protocol.h"

#define BAUD_RATE 57600

/**************************
* User defines
**************************/

#ifndef __AVR_ATmega328P__
  #define __LEONARDO__
#endif

#if defined(__LEONARDO__)
  #define PIN_NUM   12
#else
  #define PIN_NUM   9
#endif

SETUP_DPAD(A0)
SETUP_DPAD(A1)
SETUP_DPAD(A2)
SETUP_DPAD(A3)
SETUP_DPAD(A4)
SETUP_DPAD(A5)

#if defined(__LEONARDO__)
  SETUP_DPAD(A6)
  SETUP_DPAD(A7)
  SETUP_DPAD(A8)
  SETUP_DPAD(A9)
  SETUP_DPAD(A10)
  SETUP_DPAD(A11)
#endif

const struct pin_info pins[PIN_NUM] = 
#if defined(__LEONARDO__)
      {{A0, GET_DPAD_VALUE(A0)},
       {A1, GET_DPAD_VALUE(A1)},
       {A2, GET_DPAD_VALUE(A2)},
       {A3, GET_DPAD_VALUE(A3)},
       {A4, GET_DPAD_VALUE(A4)},
       {A5, GET_DPAD_VALUE(A5)},
       {A6, GET_DPAD_VALUE(A6)},
       {A7, GET_DPAD_VALUE(A7)},
       {A8, GET_DPAD_VALUE(A8)},
       {A9, GET_DPAD_VALUE(A9)},
       {A10, GET_DPAD_VALUE(A10)},
       {A11, GET_DPAD_VALUE(A11)}};
#else
      {{A0, GET_DPAD_VALUE(A0)},
       {A1, GET_DPAD_VALUE(A1)},
       {A2, GET_DPAD_VALUE(A2)},
       {A3, GET_DPAD_VALUE(A3)},
       {A4, GET_DPAD_VALUE(A4)},
       {A5, GET_DPAD_VALUE(A5)},
       {4, digitalReadOSCINV},
       {6, digitalReadOSC},
       {8, digitalReadOSC}};
#endif

/**************************
* Setup & main loop
**************************/

unsigned char message[MSGSIZE(PIN_NUM)] = {0};

void setup() {
  /* faster analogRead */
  SET_PRESCALE();

#ifndef __LEONARDO__
  pinMode(4, INPUT);
  pinMode(6, INPUT);
  pinMode(8, INPUT);
#endif

  Serial.begin(BAUD_RATE);
  while(!Serial);

  /* setup complete! */
}

void loop() {
  buildMessage(message, pins, PIN_NUM);

  Serial.write(message, MSGSIZE(PIN_NUM));
}


