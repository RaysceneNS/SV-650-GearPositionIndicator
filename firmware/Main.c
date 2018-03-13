#include "main.h"

//default the position to neutral on boot
volatile uint8_t _currentGear = INDETERMINATE;

//create rolling average variables for the ADC 
volatile uint8_t _gearSamples[SAMPLE_SETS] = {0};
volatile uint8_t _gearPosIdx = 0;
volatile uint8_t _gearPosCnt = 0;
volatile uint16_t _gearPosSum = 0;

volatile uint8_t _battSamples[SAMPLE_SETS] = {0};
volatile uint8_t _battPosIdx = 0;
volatile uint8_t _battPosCnt = 0;
volatile uint16_t _battPosSum = 0;

volatile uint8_t _tempSamples[SAMPLE_SETS] = {0};
volatile uint8_t _tempPosIdx = 0;
volatile uint8_t _tempPosCnt = 0;
volatile uint16_t _tempPosSum = 0;


uint8_t AverageGearValue()
{
	if (_gearPosCnt < SAMPLE_SETS)
	{
		return 0;
	}
	return _gearPosSum / SAMPLE_SETS;
}

uint8_t AverageBattVoltage()
{
	if (_battPosCnt < SAMPLE_SETS)
	{
		return 0;
	}
	return _battPosSum / SAMPLE_SETS;
}

uint8_t AverageTempValue()
{
	if (_tempPosCnt < SAMPLE_SETS)
	{
		return 0;
	}
	return _tempPosSum / SAMPLE_SETS;
}

/*
 interrupt service routine for ADC
 here we read the value as 8 bit precision and then copy that value to a memory location
*/
volatile uint8_t isr_idx = 0;
ISR(ADC_vect)
{
	//only want the high val ... don't need 10bit precision here
	uint8_t newestVal = ADCH;
	
	/*
	Set the ADC Start Conversion bit on the ADC Control and Status Register A
	*/
	ADCSRA |= (1<<ADSC);
	
	isr_idx++;
	
	if(isr_idx <= 2)
	{
		//throw away the first reading(s) after each admux switch as the ADC hasn't stabilized at this time
		return;
	}
	
	if(ADMUX == GEAR_ADC_CH)
	{
		// for some reason the bike doesn't maintain the gear voltage when the engine is not running
		// in this case we get a voltage level for one second followed by ground.. to work around this we throw away any readings that
		// are below a threshold
		if(newestVal > MIN_GEAR_LEVEL_THRESHOLD)
		{
			//if levels between gears has changed radically then retake the average from scratch, this should keep us from
			// gradually floating through the intervening space between gears in our averages
			// this will have the effect of making the level changes more integer like
			uint8_t currAvg = AverageGearValue();
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
	else if(ADMUX == BATT_ADC_CH)
	{
		_battPosSum -= _battSamples[_battPosIdx];
		_battSamples[_battPosIdx] = newestVal;
		_battPosSum += _battSamples[_battPosIdx];
		_battPosIdx = (_battPosIdx + 1) % SAMPLE_SETS;
		if (_battPosCnt < SAMPLE_SETS)
		{
			_battPosCnt++;
		}		
	}
	else if(ADMUX == TEMP_ADC_CH)
	{
		_tempPosSum -= _tempSamples[_tempPosIdx];
		_tempSamples[_tempPosIdx] = newestVal;
		_tempPosSum += _tempSamples[_tempPosIdx];
		_tempPosIdx = (_tempPosIdx + 1) % SAMPLE_SETS;
		if (_tempPosCnt < SAMPLE_SETS)
		{
			_tempPosCnt++;
		}		
	}
		
	//move the admux to the next channel
	if(isr_idx > 10)
	{
		if(ADMUX == GEAR_ADC_CH)
		{
			ADMUX = BATT_ADC_CH;
		}
		else if(ADMUX == BATT_ADC_CH)
		{
			ADMUX = TEMP_ADC_CH;			
		}
		else if(ADMUX == TEMP_ADC_CH)
		{
			ADMUX = GEAR_ADC_CH;			
		}
		
		isr_idx = 0;
	}
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
	ADMUX = GEAR_ADC_CH;
	
	ADCSRA =
		(1 << ADEN)  |     // Enable ADC
		(1 << ADSC)  |     // start ADC
		(0 << ADATE) |     // disable auto triggering of ADC
		(0 << ADIF)  |     // clear ADC interrupt flag
		(1 << ADIE)  |     // Enable ADC interrupt
		(1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);    // set prescaler to /64 1:1:0
	
	ADCSRB = (1 << ADLAR);     // left shift result, only interested in 8 bit precision
}

void init()
{
	// set port direction registers high is OUT
	DDRA = DDRA_BOARD;
	DDRB = DDRB_BOARD;	
	
	initADC();

	LcdInitialise();

	//turn off unused parts to conserve power
	power_usi_disable();
	power_timer1_disable();
}

void LcdDrawVoltage()
{
	// the voltage divider is built using a 10k & 47k divider network = 5.7x divider on input V
	// thus 2.5v on ADC is 5.7x2.5 = 14.25v input = 194 ADC 
	// note: maximum reading on the divider is 18.81 v
	// the factor is determined as VCC / 256 ADC steps * 5.7
	//   3.3v / 256 * vdiv(5.7)=0.0734765625 v per ADC step
	uint8_t volts = AverageBattVoltage() * (0.0734765625*10);
	
	//print the 1 in the tens places
	for(uint8_t i = 2; i >= 0; i--)
	{
		LcdMediumDigit(4, 42 + (i*12), (volts % 10));
		volts = (volts-(volts % 10))/10;
	}
	
	//print the decimal point in the voltage
	LcdGliff(5, 64, decimal);
	
	//print the v to denote volts
	LcdGliff(5, 78, volt_symbol);
}

/*
Prints the current gear on the LCD
*/
void LcdDrawGear()
{
	// Only accept a gear as changed if it maintains a stability for a defined interval of time
	uint8_t tempGear1 = DetermineGearPos(AverageGearValue());
	if (tempGear1 != _currentGear)
	{
		// make sure that we're serious about this new state,
		_delay_ms(GEAR_DEBOUNCE_MS);
		uint8_t tempGear2 = DetermineGearPos(AverageGearValue());
		if(tempGear2 == tempGear1)
		{
			//now make the change
			_currentGear = tempGear2;
			switch(_currentGear)
			{
				case INDETERMINATE:
				LcdBitmap(0, 0, BMP_CLEAR);
				break;
				case NEUTRAL:
				LcdBitmap(0, 0, BMP_NEUTRAL);
				break;
				case FIRST:
				LcdBitmap(0, 0, BMP_FIRST);
				break;
				case SECOND:
				LcdBitmap(0, 0, BMP_SECOND);
				break;
				case THIRD:
				LcdBitmap(0, 0, BMP_THIRD);
				break;
				case FOURTH:
				LcdBitmap(0, 0, BMP_FOURTH);
				break;
				case FIFTH:
				LcdBitmap(0, 0, BMP_FIFTH);
				break;
				case SIXTH:
				LcdBitmap(0, 0, BMP_SIXTH);
				break;
			}
		}
	}
}

void LcdDrawTemp()
{
	// the factor is 1.2890625=3.3/256*100  vcc/adcsteps*degreesC
	// the 0.5volt offset is baked into the MCP9700 to allow the part to read below zero degrees Celsius
	// this call converts the ADC reading into a degrees value
	int8_t degreesC = ((AverageTempValue() * 1.2890625) - 50);
	
	if(degreesC < 0)
	{
		//print negation symbols
		LcdGliff(0, 48, hyphen);
	}
	
	//print the 1 in the tens places
	for(uint8_t i = 1; i >= 0; i--)
	{
		LcdMediumDigit(4, 52 + (i*12), (degreesC % 10));
		degreesC=(degreesC-(degreesC % 10))/10;
	}
		
	//print the degree symbol
	LcdGliff(0, 78, degree_symbol);
}

int main(void)
{
	//initialize the chip registers
	init();
	
	// enable interrupt service routines, we need these for the ADC
	sei();

	// animate a startup sequence, filling the display from the bottom to the top of the display with horizontal bars
	// The reason is to allow the ADC to stabilize the internal values after the boot 
	for(uint8_t y = 5; y >= 0; y--)
	{	
		for(uint8_t x=21; x<63; x++) // top
		{
			LcdGotoXY(x, y);
			LcdWrite(LCD_DATA, 0xff);
		}
		_delay_ms(250);
	}
	
	// enable watch dog, careful that timeout is longer than loop sleep time
	wdt_enable(WDTO_250MS);	
	
	uint8_t i = 0xff;
	while(1)
	{
		if(i++ == 0)
		{
			// using a clear 
			LcdClear();
		}
		
		LcdDrawGear();
		LcdDrawTemp();
		LcdDrawVoltage();
		
		// draw the divider grid to separate the display into three components
		//      | TEMP
		// GEAR |-----
		// POS  | BATT
		LcdDrawVLine(42,0,5);
		LcdDrawHLine(42,3,42);
		
		//delay to slow looping
		_delay_ms(DELAY_MS);
		wdt_reset();
	}
	
	return 0;
}
