#include "Serial_protocol.h"

void buildMessage(unsigned char message[], struct pin_info pins[], unsigned int pin_num)
{
  int i, tmp;
  
  for(i = 0; i < pin_num; ++i) {
    tmp = pins[i].function(pins[i].pin);
    message[i*2] = SCALE(tmp);
    message[(i*2)+1] = MIDBITS;
  }
  message[pin_num * 2] = MIDBITS;
  message[(pin_num * 2) + 1] = ENDBITS;
  
}

