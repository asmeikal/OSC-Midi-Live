#ifndef __FAST_ANALOG_READ_H
#define __FAST_ANALOG_READ_H

/**
 * Source: http://forum.arduino.cc/index.php?topic=6549.0
 */

/* defines for setting and clearing register bits */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define SET_PRESCALE()  \
  sbi(ADCSRA, ADPS2); \
  cbi(ADCSRA, ADPS1); \
  cbi(ADCSRA, ADPS0)

#endif

