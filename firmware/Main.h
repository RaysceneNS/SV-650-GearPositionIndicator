#ifndef MAIN_H_
#define MAIN_H_

/* 
8MHz crystal oscillator / div8 
*/
#define F_CPU 1000000UL

#include <stdint.h>       // needed for uint8_t
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "board_attiny44.h"
#include "transmission.h"
#include "pcd8544.h"
#include "bitmap.h"

// defines the number of times to take a sampling of voltages from the ADC, higher numbers increase accuracy at the expense of speed
// both values must be powers of two
#define SAMPLE_SETS 8 //avg this many samples together

#define DELAY_MS 25 // this delay is used to set a max frame rate of the LCD panel 1000/DELAY_MS = FRAMES

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))

#endif /* MAIN_H_ */

