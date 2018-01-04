#include "transmission.h"

/*
// Input a voltage level from 0-255 and get a gear back
*/
uint8_t DetermineGearPos(uint8_t avgVal)
{
	if (avgVal >= SIXTH_LO_V && avgVal <= SIXTH_HI_V)
	{
		return SIXTH;
	}
	if (avgVal >= FIFTH_LO_V && avgVal <= FIFTH_HI_V)
	{
		return FIFTH;
	}
	if (avgVal >= FOURTH_LO_V && avgVal <= FOURTH_HI_V)
	{
		return FOURTH;
	}
	if (avgVal >= THIRD_LO_V && avgVal <= THIRD_HI_V)
	{
		return THIRD;
	}
	if (avgVal >= SECOND_LO_V && avgVal <= SECOND_HI_V)
	{
		return SECOND;
	}
	if (avgVal >= FIRST_LO_V && avgVal <= FIRST_HI_V)
	{
		return FIRST;
	}
	if (avgVal >= NEUTRAL_LO_V && avgVal <= NEUTRAL_HI_V)
	{
		return NEUTRAL;
	}
	return INDETERMINATE;
}
