
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define RESETPIN  2
#define LEDPIN    6

#define READMAX   1023U
#define READMIN   0
#define SENDMAX   255
#define SENDMIN   0

#define MIDBITS   0x00
#define ENDBITS   0xFF

#define BUFFSIZE  20

#define MAKEBUFF(x)   int buffer ## x [BUFFSIZE] = {0}
#define BUFF(x)       buffer ## x
#define GETVALUE(x)   readValue ## x ()

unsigned int reduction[][2] =
{{1,   1},
 {621, 625},
 {851, 873},
 {118, 125},
 {451, 500},
 {843, 992},
 {493, 625},
 {505, 701},
 {545, 843},
 {450, 791},
 {306, 625},
 {172, 419},
 {333, 998},
 {172, 659},
 {125, 643},
 {8,   59},
 {53,  617},
 {46,  985},
 {11,  579},
 {1,   294}};

#define READVALUE(x)  \
unsigned int readValue ## x (void) { \
  unsigned int i, ret = 0; \
  for(i = 0; i+1 < BUFFSIZE; ++i) { \
    BUFF(x)[i] = BUFF(x)[i+1]; \
    ret += BUFF(x)[i] * reduction[BUFFSIZE-i-1][0] / reduction[BUFFSIZE-i-1][1]; \
  } \
  BUFF(x)[BUFFSIZE-1] = analogRead(x); \
  return min(ret + BUFF(x)[BUFFSIZE-1], READMAX); \
}

MAKEBUFF(A0);
MAKEBUFF(A1);
READVALUE(A0)
READVALUE(A1)

void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
  
  // set prescale to 16
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);

  Serial.begin(115200);
//  digitalWrite(LEDPIN, LOW);

}

unsigned char message[6] = {0};

unsigned int tmp;

void loop() {
  tmp = GETVALUE(A0);
//  Serial.println(tmp);
//  return;
  message[0] = map(tmp, READMIN, READMAX, SENDMIN, SENDMAX);
  tmp = GETVALUE(A1);
  message[2] = map(tmp, READMIN, READMAX, SENDMIN, SENDMAX);
  message[1] = message[3] = MIDBITS;
  message[4] = message[5] = ENDBITS;
  digitalWrite(LEDPIN, LOW);
  Serial.write(message, 6);
  digitalWrite(LEDPIN, HIGH);
}
