#ifndef __SERIAL_PROTOCOL_H
#define __SERIAL_PROTOCOL_H

/**************************
* Defines & macros
**************************/

#define READMAX   1023U
#define READMIN   0
#define SENDMAX   255
#define SENDMIN   0

#define SCALE(x)  map((x), READMIN, READMAX, SENDMIN, SENDMAX)

#define MIDBITS   0x00
#define ENDBITS   0xFF

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

void buildMessage(unsigned char message[], struct pin_info pins[], unsigned int pin_num);

#endif

