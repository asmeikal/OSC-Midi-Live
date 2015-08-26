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

