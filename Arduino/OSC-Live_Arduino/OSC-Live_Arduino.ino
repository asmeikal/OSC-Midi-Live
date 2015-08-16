#include "Drumpad_read.h"
#include "Fast_analogRead.h"
#include "Serial_protocol.h"

/**************************
* User defines
**************************/

#define BAUD_RATE 57600
#define PIN_NUM   2
#define LEDPIN    13

SETUP_DRUM_PAD(A0)
SETUP_DRUM_PAD(A1)

struct pin_info pins[PIN_NUM] = 
      {{A0, GETVALUE(A0)},
       {A1, GETVALUE(A1)}};

unsigned char message[(PIN_NUM + 1) * 2] = {0};
unsigned int message_s = (PIN_NUM + 1) * 2;

/**************************
* Setup & main loop
**************************/

void setup() {
  /* light led up until setup is done */
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
  
  /* faster analogRead */
  SET_PRESCALE();

  Serial.begin(BAUD_RATE);
  while(!Serial);

  /* setup complete! */
  digitalWrite(LEDPIN, LOW);
}

void loop() {
  buildMessage(message, pins, PIN_NUM);

  Serial.write(message, message_s);
}

