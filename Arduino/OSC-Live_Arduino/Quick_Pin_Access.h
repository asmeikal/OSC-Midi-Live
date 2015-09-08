/**
 * OSC-Midi-Live
 * Copyright (C) 2015 Michele Laurenti
 * email: asmeikal [at] me [dot] com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You can find a copy of the GNU General Public License at 
 * http://www.gnu.org/licenses/gpl.html
 */

#ifndef __QUICK_PIN_ACCESS_H
#define __QUICK_PIN_ACCESS_H

/**
 * Source: http://forum.arduino.cc/index.php?topic=91079.0
 */

#define PINHIGH(x)    PINHIGH ## x
#define PINLOW(x)     PINLOW ## x
#define PININPUT(x)   PININPUT ## x
#define PINOUTPUT(x)  PINOUTPUT ## x

// --- PIN MODE: OUTPUT  --- 

#define PINOUTPUT0    DDRD |= _BV (0)
#define PINOUTPUT1    DDRD |= _BV (1)
#define PINOUTPUT2    DDRD |= _BV (2)
#define PINOUTPUT3    DDRD |= _BV (3)
#define PINOUTPUT4    DDRD |= _BV (4)
#define PINOUTPUT5    DDRD |= _BV (5)
#define PINOUTPUT6    DDRD |= _BV (6)
#define PINOUTPUT7    DDRD |= _BV (7)
#define PINOUTPUT8    DDRB |= _BV (0)
#define PINOUTPUT9    DDRB |= _BV (1)
#define PINOUTPUT10   DDRB |= _BV (2)
#define PINOUTPUT11   DDRB |= _BV (3)
#define PINOUTPUT12   DDRB |= _BV (4)
#define PINOUTPUT13   DDRB |= _BV (5)
#define PINOUTPUTA0   DDRC |= _BV (0)
#define PINOUTPUTA1   DDRC |= _BV (1)
#define PINOUTPUTA2   DDRC |= _BV (2)
#define PINOUTPUTA3   DDRC |= _BV (3)
#define PINOUTPUTA4   DDRC |= _BV (4)
#define PINOUTPUTA5   DDRC |= _BV (5)

// --- PIN MODE: INPUT  --- 

#define PININPUT0     DDRD &= ~_BV (0)
#define PININPUT1     DDRD &= ~_BV (1)
#define PININPUT2     DDRD &= ~_BV (2)
#define PININPUT3     DDRD &= ~_BV (3)
#define PININPUT4     DDRD &= ~_BV (4)
#define PININPUT5     DDRD &= ~_BV (5)
#define PININPUT6     DDRD &= ~_BV (6)
#define PININPUT7     DDRD &= ~_BV (7)
#define PININPUT8     DDRB &= ~_BV (0)
#define PININPUT9     DDRB &= ~_BV (1)
#define PININPUT10    DDRB &= ~_BV (2)
#define PININPUT11    DDRB &= ~_BV (3)
#define PININPUT12    DDRB &= ~_BV (4)
#define PININPUT13    DDRB &= ~_BV (5)
#define PININPUTA0    DDRC &= ~_BV (0)
#define PININPUTA1    DDRC &= ~_BV (1)
#define PININPUTA2    DDRC &= ~_BV (2)
#define PININPUTA3    DDRC &= ~_BV (3)
#define PININPUTA4    DDRC &= ~_BV (4)
#define PININPUTA5    DDRC &= ~_BV (5)

// --- DIGITAL WRITE: HIGH  --- 

#define PINHIGH0      PORTD |= _BV (0)
#define PINHIGH1      PORTD |= _BV (1)
#define PINHIGH2      PORTD |= _BV (2)
#define PINHIGH3      PORTD |= _BV (3)
#define PINHIGH4      PORTD |= _BV (4)
#define PINHIGH5      PORTD |= _BV (5)
#define PINHIGH6      PORTD |= _BV (6)
#define PINHIGH7      PORTD |= _BV (7)
#define PINHIGH8      PORTB |= _BV (0)
#define PINHIGH9      PORTB |= _BV (1)
#define PINHIGH10     PORTB |= _BV (2)
#define PINHIGH11     PORTB |= _BV (3)
#define PINHIGH12     PORTB |= _BV (4)
#define PINHIGH13     PORTB |= _BV (5)
#define PINHIGHA0     PORTC |= _BV (0)
#define PINHIGHA1     PORTC |= _BV (1)
#define PINHIGHA2     PORTC |= _BV (2)
#define PINHIGHA3     PORTC |= _BV (3)
#define PINHIGHA4     PORTC |= _BV (4)
#define PINHIGHA5     PORTC |= _BV (5)

// --- DIGITAL WRITE: LOW  --- 

#define PINLOW0       PORTD &= ~_BV (0)
#define PINLOW1       PORTD &= ~_BV (1)
#define PINLOW2       PORTD &= ~_BV (2)
#define PINLOW3       PORTD &= ~_BV (3)
#define PINLOW4       PORTD &= ~_BV (4)
#define PINLOW5       PORTD &= ~_BV (5)
#define PINLOW6       PORTD &= ~_BV (6)
#define PINLOW7       PORTD &= ~_BV (7)
#define PINLOW8       PORTB &= ~_BV (0)
#define PINLOW9       PORTB &= ~_BV (1)
#define PINLOW10      PORTB &= ~_BV (2)
#define PINLOW11      PORTB &= ~_BV (3)
#define PINLOW12      PORTB &= ~_BV (4)
#define PINLOW13      PORTB &= ~_BV (5)
#define PINLOWA0      PORTC &= ~_BV (0)
#define PINLOWA1      PORTC &= ~_BV (1)
#define PINLOWA2      PORTC &= ~_BV (2)
#define PINLOWA3      PORTC &= ~_BV (3)
#define PINLOWA4      PORTC &= ~_BV (4)
#define PINLOWA5      PORTC &= ~_BV (5)

#endif

