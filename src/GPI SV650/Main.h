#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 1000000UL         /* 8MHz crystal oscillator / div8 */

#include <stdint.h>       // needed for uint8_t
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>


#define BATT_ADC_CH 0   // use VCC for AREF, use ADC0 for input (PA0), MUX bit 0:0:0:0:0
#define GEAR_ADC_CH 1	// use VCC for AREF, use ADC1 for input (PA1), MUX bit 0:0:0:0:0

// defines the number of times to take a sampling of voltages from the GPS switch higher numbers increase accurancy at the expense of speed
// both values must be powers of two
#define SAMPLE_SETS 16 //avg this many samples together


#define DISP_A_PORT		PORTB
#define DISP_A_PIN		_BV(PB2)
#define DISP_B_PORT		PORTA
#define DISP_B_PIN		_BV(PA2)
#define DISP_C_PORT		PORTA
#define DISP_C_PIN		_BV(PA3)
#define DISP_D_PORT		PORTA
#define DISP_D_PIN		_BV(PA4)
#define DISP_E_PORT		PORTA
#define DISP_E_PIN		_BV(PA5)
#define DISP_F_PORT		PORTA
#define DISP_F_PIN		_BV(PA6)
#define DISP_G_PORT		PORTA
#define DISP_G_PIN		_BV(PA7)
#define DISP_ALERT_PORT		PORTB
#define DISP_ALERT_PIN		_BV(PB1)

#define DELAY_MS 25
#define GEAR_DEBOUNCE_MS 150UL

/*
voltage readings between first and second gear are very close, it would be easy to mistake one for the other thereby traveling directly
between these gears. This is physically impossible as you have to shift past neutral. Luckily neutral has a high voltage (close to 5v) so
we can definitely tell this from background noise. We ensure that we always latch through each gear in succession.

	i.e. 1-N-2-3-4-5-6

 The Suzuki manual states the following voltages as nominal for each gear position
	1st: 1.36V
	N:   5.00V
	2nd: 1.77V
	3rd: 2.49V
	4th: 3.23V
	5th: 4.10V
	6th: 4.55V
	
	but our AD converter returns a 10bit value for voltages in the range 0v-5v as 0-1023
	therefore each trigger level maps to an int value. We allow for variances in the range of 
	delta V above and below the factory listed value in order to trigger shifts.

	FIRST_HI_V | SECOND_LO_V | SECOND_HI_V | THIRD_LO_V | THIRD_HI_V | FOURTH_LO_V | FOURTH_HI_V | FIFTH_LO_V | FIFTH_HI_V | SIXTH_LO_V | NEUTRAL_LO_V
*/
//1.36v =69.4
// Note: SV650 gear shows 0V on 1st gear, when bike engine is off, but switch is on. Only when engaged to N for first time, then the 1st gear reading becomes normal
#define	FIRST_LO_V 54U 
#define	FIRST_HI_V 79U //ok to butt this against second ?? we have neutral between...

//1.77v = 90.2
#define	SECOND_LO_V 80U 
#define	SECOND_HI_V 100U

//2.49v = 127
#define	THIRD_LO_V 117U
#define	THIRD_HI_V 137U

//3.23v = 164.7
#define	FOURTH_LO_V 155U
#define	FOURTH_HI_V 175U

//4.1v = 209.1
#define	FIFTH_LO_V 199U
#define	FIFTH_HI_V 219U

//4.55v = 232.1
#define	SIXTH_LO_V 223U
#define	SIXTH_HI_V 240U

#define	NEUTRAL_LO_V 242U /*4.76v*/
#define	NEUTRAL_HI_V 255U

// define the available gears 
#define	NEUTRAL 0U
#define FIRST 1U
#define SECOND 2U
#define THIRD 3U
#define FOURTH 4U
#define FIFTH 5U
#define SIXTH 6U
#define	INDETERMINATE 255U

//this is the maximum instantaneous gear level change we'll accept before throwing out our average it equates to ~0.39v
#define MAX_GEAR_LEVEL_TRANSIENT 20U
// this is the minimum voltage signal required to make our software consider it a viable reading ~1.0v
#define MIN_GEAR_LEVEL_THRESHOLD 51U

#endif /* MAIN_H_ */

