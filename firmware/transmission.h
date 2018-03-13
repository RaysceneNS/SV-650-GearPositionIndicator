/*
 * transmission.h
 *
 * Defines the values used to interface with the Gear Position Indicator pawl of the transmission on a Suzuki SV650
 */ 

#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_

#include <stdint.h>       // needed for uint8_t

#define GEAR_DEBOUNCE_MS 100UL

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
	
	but our AD converter returns a 8bit value for voltages in the range 0v-5v as 0-255
	therefore each trigger level maps to an int value. We allow for variances in the range of 
	delta V above and below the factory listed value in order to trigger shifts.

	FIRST_LO_V   <->  FIRST_HI_V
	SECOND_LO_V  <->  SECOND_HI_V
	THIRD_LO_V   <->  THIRD_HI_V
	FOURTH_LO_V  <->  FOURTH_HI_V
	FIFTH_LO_V   <->  FIFTH_HI_V
	SIXTH_LO_V   <->  SIXTH_HI_V
	NEUTRAL_LO_V <->  NEUTRAL_HI_V
	
	Note due to a power supply requirement of 3.3v we need to level shift the Gear input from 5v through a voltage divider.
	the divider is 10k/10k which shifts inputs by half. Thus 5.0v in = 2.5v in = 193ADC
*/

//1.36v =52.5
// Note: SV650 gear shows 0V on 1st gear, when bike engine is off, but switch is on. Only when engaged to N for first time, then the 1st gear reading becomes normal
#define	FIRST_LO_V 41U 
#define	FIRST_HI_V 60U //is it OK to butt this against second ?? we have neutral between...

//1.77v = 68.2
#define	SECOND_LO_V 61U 
#define	SECOND_HI_V 76U

//2.49v = 96
#define	THIRD_LO_V 89U
#define	THIRD_HI_V 104U

//3.23v = 124.7
#define	FOURTH_LO_V 117U
#define	FOURTH_HI_V 132U

//4.1v = 158.3
#define	FIFTH_LO_V 151U
#define	FIFTH_HI_V 166U

//4.55v = 175.7
#define	SIXTH_LO_V 169U
#define	SIXTH_HI_V 182U

#define	NEUTRAL_LO_V 183U /*4.76v*/
#define	NEUTRAL_HI_V 193U


//this is the maximum instantaneous gear level change we'll accept before throwing out our average it equates to ~0.39v
#define MAX_GEAR_LEVEL_TRANSIENT 15U
// this is the minimum voltage signal required to make our software consider it a viable reading ~1.0v
#define MIN_GEAR_LEVEL_THRESHOLD 39U

// define the available gears 
#define	NEUTRAL 0U
#define FIRST 1U
#define SECOND 2U
#define THIRD 3U
#define FOURTH 4U
#define FIFTH 5U
#define SIXTH 6U
#define	INDETERMINATE 255U

uint8_t DetermineGearPos(uint8_t avgVal);

#endif /* TRANSMISSION_H_ */
