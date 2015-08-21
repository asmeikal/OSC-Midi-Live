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
  
  message[MSG_SIZE(pin_num)-2] = message[MSG_SIZE(pin_num)-1] = ENDBITS;
  
}

