#ifndef __DRUMPAD_READ_H
#define __DRUMPAD_READ_H

#include "Quick_Pin_Access.h"

/******************************************
* READ function for drum pads
*******************************************/

#define THRESHOLD     600
#define TIME_WAIT     50L * 1000L   /* 50.000 usec = 50 msec */

/*
 * Usage: for each analog pin, use SETUP(x), then read values with GETVALUE(x)
 */
 
#define SETUP_DRUM_PAD(x) \
    MAKE_TIMER(x); \
    MAKE_LAST(x); \
    DEFINE_READFUNC(x)

#define MAKE_TIMER(x) unsigned long timer ## x = 0
#define MAKE_LAST(x)  unsigned int last ## x = 0
#define TIMER(x)      timer ## x
#define LAST(x)       last ## x
#define GETVALUE(x)   readValue ## x

#define DEFINE_READFUNC(x)  \
int readValue ## x (unsigned char pin) { \
  int ret = analogRead(pin); \
  unsigned long t = micros(); \
  if(t < TIMER(x) + TIME_WAIT) { \
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



#endif

