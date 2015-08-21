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

