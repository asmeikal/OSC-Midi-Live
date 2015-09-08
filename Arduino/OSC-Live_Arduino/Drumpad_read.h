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

#ifndef __DRUMPAD_READ_H
#define __DRUMPAD_READ_H

#include "Quick_Pin_Access.h"

/******************************************
  READ function for drum pads
*******************************************/

#define THRESHOLD     300
#define TIME_WAIT     50L * 1000L   /* 50.000 usec = 50 msec */
#define NOTE_LEN      500L * 1000L  /* 500.000 usec = 500 msec = 0.5 sec */

#define ABS(x)        ((x) >= 0 ? (x) : - (x))

enum ToggleStatus {T_LOW, T_HIGH};
enum SignalStatus {S_DOWN, S_RAISING, S_UP, S_FALLING};

#define MAKE_TIMER(x)      unsigned long timer ## x = 0
#define MAKE_LAST(x)       unsigned int last ## x = 0

#define MAKE_AT_STATUS(x)  ToggleStatus analogToggleStatus ## x = T_LOW
#define MAKE_AB_STATUS(x)  ToggleStatus analogButtonStatus ## x = T_LOW

#define MAKE_DT_STATUS(x)  ToggleStatus digitalToggleStatus ## x = T_LOW
#define MAKE_DB_STATUS(x)  ToggleStatus digitalButtonStatus ## x = T_LOW

#define MAKE_S_STATUS(x)   SignalStatus signalStatus ## x = S_DOWN

#define TIMER(x)           timer ## x
#define LAST(x)            last ## x

#define AT_STATUS(x)       analogToggleStatus ## x
#define AB_STATUS(x)       analogButtonStatus ## x

#define DT_STATUS(x)       digitalToggleStatus ## x
#define DB_STATUS(x)       digitalButtonStatus ## x

#define S_STATUS(x)        signalStatus ## x

/*
   Usage: for each analog pin, use SETUP_DRUM_PAD(x), then read values with GET_DPAD_VALUE(x)
*/

#define SETUP_DPAD(x) \
  MAKE_TIMER(x); \
  MAKE_LAST(x); \
  DEFINE_DPAD_READFUNC(x)

#define GET_DPAD_VALUE(x)   readDrumpadValue ## x

#define DEFINE_DPAD_READFUNC(x)  \
  int readDrumpadValue ## x (const unsigned char pin) { \
    int ret = analogRead(pin); \
    unsigned long t = micros(); \
    if(ABS(t - TIMER(x)) < TIME_WAIT) { \
      ret = max(LAST(x), ret); \
      LAST(x) = ret; \
    } \
    else { \
      if(THRESHOLD < ret) { \
        TIMER(x) = t; \
        LAST(x) = ret; \
      } \
      else { \
        ret = 0; \
      } \
    } \
    return ret; \
  } /* end DEFINE_READFUNC(x) */

/**************************
  Drumpad read w/ long note
**************************/

#define SETUP_DPAD_LONG(x) \
  MAKE_TIMER(x); \
  MAKE_LAST(x); \
  MAKE_S_STATUS(x); \
  DEFINE_DPAD_LONG_READFUNC(x)

#define GET_DPAD_LONG_VALUE(x)   readLongDrumpadValue ## x

#define DEFINE_DPAD_LONG_READFUNC(x) \
  int readLongDrumpadValue ## x (const unsigned char pin) { \
    int ret = analogRead(pin); \
    unsigned long t = micros(); \
    if(THRESHOLD < ret) { \
      S_STATUS(x) = (S_RAISING == S_STATUS(x)) ? S_UP : S_RAISING; \
    } \
    else { \
      S_STATUS(x) = (S_FALLING == S_STATUS(x)) ? S_DOWN : S_FALLING; \
    } \
    if(ABS(t - TIMER(x)) < NOTE_LEN) { \
      if(S_RAISING == S_STATUS(x)) { \
        LAST(x) = ret; \
        TIMER(x) = t; \
        ret = 0; \
      } \
      else { \
        ret = max(LAST(x), ret); \
        LAST(x) = ret; \
      } \
    } \
    else { \
      if(S_RAISING == S_STATUS(x)) { \
        TIMER(x) = t; \
        LAST(x) = ret; \
      } \
      else { \
        ret = 0; \
      } \
    } \
    return ret; \
  }

/**************************
  Drumpad read (toggle)
**************************/

#define SETUP_ATOGGLE(x) \
  MAKE_AT_STATUS(x); \
  MAKE_AB_STATUS(x); \
  SETUP_DPAD(x) \
  DEFINE_ATOGGLE_READFUNC(x)

#define GET_ATOGGLE_VALUE(x) aToggleOSC ## x

#define DEFINE_ATOGGLE_READFUNC(x) \
  int aToggleOSC ## x(const unsigned char pin) { \
    if(THRESHOLD > GET_DPAD_VALUE(x)(pin)) { \
      AB_STATUS(x) = T_LOW; \
    } \
    else { \
      if(T_LOW == AB_STATUS(x)) { \
        AT_STATUS(x) = (T_LOW == AT_STATUS(x)) ? T_HIGH : T_LOW; \
      } \
      AB_STATUS(x) = T_HIGH; \
    } \
    return (T_HIGH == AT_STATUS(x)) ? READMAX : READMIN; \
  }

/**************************
  Digital read (toggle)
**************************/

#define SETUP_DTOGGLE(x) \
  MAKE_DT_STATUS(x); \
  MAKE_DB_STATUS(x); \
  DEFINE_DTOGGLE_READFUNC(x)

#define GET_DTOGGLE_VALUE(x) dToggleOSC ## x

#define DEFINE_DTOGGLE_READFUNC(x) \
  int dToggleOSC ## x(const unsigned char pin) { \
    PININPUT(x); \
    if(0 == digitalRead(pin)) { \
      DB_STATUS(x) = T_LOW; \
    } \
    else { \
      if(T_LOW == DB_STATUS(x)) { \
        DT_STATUS(x) = (T_LOW == DT_STATUS(x)) ? T_HIGH : T_LOW; \
      } \
      DB_STATUS(x) = T_HIGH; \
    } \
    return (T_HIGH == DT_STATUS(x)) ? READMAX : READMIN; \
  }

/**************************
  Functions declaration
**************************/

int digitalReadOSC(const unsigned char pin);
int digitalReadOSCINV(const unsigned char pin);

int analogReadINV(const unsigned char pin);

#endif

