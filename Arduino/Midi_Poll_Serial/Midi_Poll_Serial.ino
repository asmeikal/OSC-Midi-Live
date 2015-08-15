#include "Midi_Poll_Serial.h"

/* user defines */

#define BAUD_RATE 57600
#define VAR_NUM   4
#define LEDPIN    13

int pins[] = {A0, A1, A2, A3};

void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
  
  // set prescale to 16
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);

  Serial.begin(BAUD_RATE);

  while(!Serial) {;}

}

unsigned char message[(VAR_NUM + 1) * 2] = {0};
unsigned int tmp;
int n, i;

void loop() {
  for(i = 0; i < VAR_NUM; ++i) {
    tmp = analogRead(pins[i]);
    message[i*2] = map(tmp, READMIN, READMAX, SENDMIN, SENDMAX);
    message[(i*2)+1] = MIDBITS;
  }
  message[VAR_NUM * 2] = MIDBITS;
  message[(VAR_NUM * 2) + 1] = ENDBITS;

  digitalWrite(LEDPIN, LOW);
  n = Serial.write(message, (VAR_NUM + 1) * 2);
  digitalWrite(LEDPIN, HIGH);
}
