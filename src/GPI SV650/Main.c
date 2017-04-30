#include "main.h"

//default the position to neutral on boot
volatile uint8_t _currentGear = INDETERMINATE;

volatile uint8_t samplesReadCounter = 0;
volatile uint8_t channel = 0;

volatile uint8_t _gearSamples[SAMPLE_SETS] = {0};
volatile uint8_t _gearPosIdx = 0;
volatile uint8_t _gearPosCnt = 0;
volatile uint16_t _gearPosSum = 0;

volatile uint8_t _battSamples[SAMPLE_SETS] = {0};
volatile uint8_t _battIdx = 0;
volatile uint8_t _battCnt = 0;
volatile uint16_t _battSum = 0;

uint8_t _displayMask = 0b00000000;
volatile uint8_t _pinIdx = 0;

uint8_t AverageBattVoltage()
{
	if (_battCnt < SAMPLE_SETS)
	{
		 return 0;
	}
	return _battSum / _battCnt;
}

uint8_t AverageGearVoltage()
{
	if (_gearPosCnt < SAMPLE_SETS) 
	{
		return 0;
	}
	return _gearPosSum / _gearPosCnt;
}

//interrupt service routine for ADC
// here we read the value as 8 bit precision and then copy that value to a memory location
ISR(ADC_vect)
{
	samplesReadCounter++;

	//only want the high val ... don't need 10bit precision here
	uint8_t newestVal = ADCH;
	uint8_t readChannel = channel;
	
	/*
	When we've gathered the correct number of ADC samplings from the current channel 
	we will reset and move to the next channel	
	*/
	if(samplesReadCounter == SAMPLE_SETS)
	{
		samplesReadCounter = 0;
		//select channel
		if(channel == GEAR_ADC_CH)
		{
			channel = BATT_ADC_CH;
		}
		else
		{
			channel = GEAR_ADC_CH;
		}
		ADMUX = channel; //pick the channel to mux
	}	
	
	/*
	Set the ADC Start Conversion bit on the ADC Control and Status Register A
	*/
	ADCSRA |= (1<<ADSC); 

	if(readChannel == BATT_ADC_CH)
	{
		_battSum -= _battSamples[_battIdx];
		_battSamples[_battIdx] = newestVal;
		_battSum += _battSamples[_battIdx];
		
		_battIdx = (_battIdx + 1) % SAMPLE_SETS;
		if (_battCnt < SAMPLE_SETS)
		{ 
			_battCnt++;
		}
	}
	else
	{
		// for some reason the bike doesn't maintain the gear voltage when the engine is not running
		// in this case we get a voltage level for one second followed by ground.. to work around this we throw away any readings that
		// are below a threshold
		if(newestVal > MIN_GEAR_LEVEL_THRESHOLD)
		{
			//if levels between gears has changed radically then retake the average from scratch, this should keep us from
			// gradually floating through the intervening space between gears in our averages
			// this will have the effect of making the level changes more integer like
			uint8_t currAvg = AverageGearVoltage();
			if ((currAvg > newestVal && (currAvg-newestVal) > MAX_GEAR_LEVEL_TRANSIENT) ||
				(newestVal > currAvg && (newestVal-currAvg) > MAX_GEAR_LEVEL_TRANSIENT))
			{
				_gearPosCnt = 0;
				_gearPosIdx = 0;
				_gearPosSum = 0;
				for (int i=0; i< SAMPLE_SETS; i++)
				{
					_gearSamples[i] = 0;
				}
			}
			
			_gearPosSum -= _gearSamples[_gearPosIdx];
			_gearSamples[_gearPosIdx] = newestVal;
			_gearPosSum += _gearSamples[_gearPosIdx];
			_gearPosIdx = (_gearPosIdx + 1) % SAMPLE_SETS;
			if (_gearPosCnt < SAMPLE_SETS)
			{
				 _gearPosCnt++;
			}
		}
	}
}


/*
	Takes a character argument as input, and sets the display mask to draw that 
	character on the seven segment display, the actual drawing is handled by the ISR on overflow Timer0
*/
void SetDisplay(uint8_t gear)
{
	switch(gear)
	{
		case INDETERMINATE:
			_displayMask = 0b10000000;		// Display '-'
			break;
		case NEUTRAL:
			_displayMask = 0b10101000;		// Display 'n'
			break;
		case FIRST:
			_displayMask = 0b00001100;		// Display Digit '1'
			break;
		case SECOND:
			_displayMask = 0b10110110;		// Display Digit '2'
			break;
		case THIRD:
			_displayMask = 0b10011110;		// Display Digit '3'
			break;
		case FOURTH:
			_displayMask = 0b11001100;		// Display Number 4
			break;
		case FIFTH:
			_displayMask = 0b11011010;		// Display Number 5
			break;
		case SIXTH:
			_displayMask = 0b11111010;		// Display Number 6
			break;
	}
}

/*
* turn on each led in the segment in turn, this keeps the total current draw to that of nearly one segment 
*/
ISR(TIM0_OVF_vect)
{
	//determine which pins turn it is in this clock cycle
	if(_pinIdx > 7)
		_pinIdx=0;
	
	if(_pinIdx == 0 && (_displayMask & 0b00000010) == 0b00000010)
		DISP_A_PORT &= ~DISP_A_PIN; //Write the ch1 pin high
	else
		DISP_A_PORT |= DISP_A_PIN; //Write the ch1 pin low
	
	if(_pinIdx == 1 && (_displayMask & 0b00000100) == 0b00000100)
		DISP_B_PORT &= ~DISP_B_PIN;
	else
		DISP_B_PORT |= DISP_B_PIN;
	
	if(_pinIdx == 2 && (_displayMask & 0b00001000) == 0b00001000)
		DISP_C_PORT &= ~DISP_C_PIN;
	else
		DISP_C_PORT |= DISP_C_PIN;
	
	if(_pinIdx == 3 && (_displayMask & 0b00010000) == 0b00010000)
		DISP_D_PORT &= ~DISP_D_PIN;
	else
		DISP_D_PORT |= DISP_D_PIN;
	
	if(_pinIdx == 4 && (_displayMask & 0b00100000) == 0b00100000)
		DISP_E_PORT &= ~DISP_E_PIN;
	else
		DISP_E_PORT |= DISP_E_PIN;
	
	if(_pinIdx == 5 && (_displayMask & 0b01000000) == 0b01000000)
		DISP_F_PORT &= ~DISP_F_PIN;
	else
		DISP_F_PORT |= DISP_F_PIN;
	
	if(_pinIdx == 6 && (_displayMask & 0b10000000) == 0b10000000)
		DISP_G_PORT &= ~DISP_G_PIN;
	else
		DISP_G_PORT |= DISP_G_PIN;	

	_pinIdx++;
}

// Input a voltage level from 0-255 and get a gear back
//
uint8_t DetermineGearPos(uint8_t avgVal)
{
	if (avgVal >= SIXTH_LO_V && avgVal <= SIXTH_HI_V) 
		return SIXTH;
	if (avgVal >= FIFTH_LO_V && avgVal <= FIFTH_HI_V) 
		return FIFTH;
	if (avgVal >= FOURTH_LO_V && avgVal <= FOURTH_HI_V) 
		return FOURTH;
	if (avgVal >= THIRD_LO_V && avgVal <= THIRD_HI_V) 
		return THIRD;
	if (avgVal >= SECOND_LO_V && avgVal <= SECOND_HI_V) 
		return SECOND; 
	if (avgVal >= FIRST_LO_V && avgVal <= FIRST_HI_V) 
		return FIRST;
	if (avgVal >= NEUTRAL_LO_V && avgVal <= NEUTRAL_HI_V)
		return NEUTRAL;		
	return INDETERMINATE;
}

/*
	This function initializes the ADC
	We start the Multiplexer by defining the particular channel to sample
	The ADC is set to operate in One Shot mode as we will be triggering the next
	sample each time the current one is processed.
	
	The pre-scaler is set to 1/64 clock cycle, this should give us 15,625Hz for operating cycles.
*/
void initADC()
{
		ADMUX = BATT_ADC_CH;
		
		ADCSRA = 
			(1 << ADEN)  |     // Enable ADC
			(1 << ADSC)  |     // start ADC
			(0 << ADATE) |     // disable auto triggering of ADC
			(0 << ADIF)  |     // clear ADC interrupt flag
			(1 << ADIE)  |     // Enable ADC interrupt
			(1 << ADPS2) |     // set prescaler to /64 1:1:0
			(1 << ADPS1) |     // set prescaler to /64 1:1:0
			(0 << ADPS0);      // set prescaler to /64 1:1:0    
			
		ADCSRB = 
			(1 << ADLAR);     // left shift result, only interested in 8 bit precision
}

void init()
{
	// set port direction registers high is OUT
	DDRA |= (1 << DDA2) | (1 << DDA3) | (1 << DDA4) | (1 << DDA5) | (1 << DDA6) | (1 << DDA7); 
	DDRB |= (1 << DDB2) | (1 << DDB1);
	
	PORTA |= DISP_B_PIN | DISP_C_PIN | DISP_D_PIN | DISP_E_PIN | DISP_F_PIN | DISP_G_PIN;//start high
	PORTB |= DISP_A_PIN | DISP_ALERT_PIN;

	initADC();

	//this timer drives our display
	TCCR0B |= (1 << CS00);// disable timer pre-scale (=clock rate)
	TIMSK0 |= (1 << TOIE0); // enable timer overflow interrupt specifically
	
	//turn off unused parts
	power_usi_disable();
	power_timer1_disable();
}

int main(void)
{
	//initialize the chip registers
	init();

	// enable interrupt service routines, we need these for the display and ADC 
	sei(); 

	// run the display through its paces
	_displayMask = 0b00000010;
	_delay_ms(50);
	_displayMask = 0b00000100;
	_delay_ms(50);
	_displayMask = 0b00001000;
	_delay_ms(50);
	_displayMask = 0b00010000;
	_delay_ms(50);
	_displayMask = 0b00100000;
	_delay_ms(50);
	_displayMask = 0b01000000;
	_delay_ms(50);
	_displayMask = 0b10000000;

	while(1)
	{
		// Only accept a gear as changed if it maintains a stability for a defined interval of time
		uint8_t tempGear1 = DetermineGearPos(AverageGearVoltage());
		if (tempGear1 != _currentGear)
		{
			// make sure that we're serious about this new state, 
			_delay_ms(GEAR_DEBOUNCE_MS);
			uint8_t tempGear2 = DetermineGearPos(AverageGearVoltage());
			if(tempGear2 == tempGear1)
			{
				//now make the change
				_currentGear = tempGear2;
				SetDisplay(_currentGear);
			}
		}
		
		// 14.5 Zener shifts all voltage -10, so we're really testing for 13.2 min and 14.5 max
		uint8_t battVoltage = AverageBattVoltage();
		if((battVoltage > 225) || (battVoltage < 160)) 
			DISP_ALERT_PORT &= ~DISP_ALERT_PIN;
		else
			DISP_ALERT_PORT |= DISP_ALERT_PIN;

		//delay to slow looping
		_delay_ms(DELAY_MS);
	}
	return 0;
}
