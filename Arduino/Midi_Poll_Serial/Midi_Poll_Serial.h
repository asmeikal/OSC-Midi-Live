#ifndef __MIDI_POLL_SERIAL_H
#define __MIDI_POLL_SERIAL_H


// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define RESETPIN  2

#define READMAX   1023U
#define READMIN   0
#define SENDMAX   255
#define SENDMIN   0

#define MIDBITS   0x00
#define ENDBITS   0xFF

#define BUFFSIZE  20

#define MAKEBUFF(x)   int buffer ## x [BUFFSIZE] = {0};
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

#define READVALUEFUNC(x)  \
unsigned int readValue ## x (void) { \
  unsigned int i, ret = 0; \
  for(i = 0; i+1 < BUFFSIZE; ++i) { \
    BUFF(x)[i] = BUFF(x)[i+1]; \
    ret += BUFF(x)[i] * reduction[BUFFSIZE-i-1][0] / reduction[BUFFSIZE-i-1][1]; \
  } \
  BUFF(x)[BUFFSIZE-1] = analogRead(x); \
  return min(ret + BUFF(x)[BUFFSIZE-1], READMAX); \
}

/**
 * Usage: for each analog pin, use MAKEBUFF(x) and READVALUEFUNC(x), then retrieve values with GETVALUE(x)
 */

#endif

