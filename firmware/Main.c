#include "main.h"

//default the position to neutral on boot
volatile uint8_t _currentGear = INDETERMINATE;
volatile uint8_t _volts = 0;
volatile uint8_t _temp = 0;

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
ISR(ADC_vect)
{
	//only want the high val ... don't need 10bit precision here
	uint8_t newestVal = ADCH;
	static uint8_t isr_idx = 0;
	
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

	//turn off unused parts to conserve power
	power_usi_disable();
	power_timer1_disable();
}

void CalculateVoltage()
{
	// thus 2.5v on ADC is 5.7x2.5 = 14.25v input = 194 ADC
	// note: maximum reading on the divider is 18.81 v
	// the factor is determined as VCC / 256 ADC steps * 5.7
	//   3.3v / 256 * vdiv(5.7)=0.0734765625 v per ADC step
	uint8_t volts = AverageBattVoltage() * (0.734765625);
		
	if(volts < 100)	{
		volts = 100;
	}
	if(volts >= 200) {
		volts = 199;
	}
	_volts = volts;
}

void LcdDrawVoltage()
{
	//print the 1 in the hundreds places our range is 10 - 18.8v
	LcdGliff(44, 4, oneup);
	LcdGliff(44, 5, onedown);

	//print the ones place
	uint8_t volts = _volts;
	LcdMediumDigit(67, 4, (volts % 10));

	//print the decimal point in the voltage
	LcdGliff(62, 5, decimal);
	
	//print the tens place
	volts = (volts-(volts % 10))/10;
	LcdMediumDigit(50, 4, (volts % 10));

	//print the v to denote volts
	LcdGliff(79, 5, volt_symbol);
}

void CalculateGear()
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
		}
	}
}

/*
Prints the current gear on the LCD
*/
void LcdDrawGear()
{
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

void CalculateTemp()
{
	// the factor is 1.2890625=3.3/256*100  vcc/adcsteps*degreesC
	// the 0.5volt offset is baked into the MCP9700 to allow the part to read below zero degrees Celsius
	// this call converts the ADC reading into a degrees value
	int16_t degreesC = ((AverageTempValue() * 1.2890625) - 50);
	
	//cap the range
	if(degreesC < -40){
		degreesC = -40;
	}
	if(degreesC > 60){
		degreesC = 60;
	}
	_temp = degreesC;
}

void LcdDrawTemp()
{
	int8_t degreesC = _temp;
	//print negation symbols
	if(degreesC < 0) {
		LcdGliff(44, 1, hyphen);
		degreesC = -degreesC;
	}
	else {
		LcdGliff(44, 1, space);
	}
	
	//print the ones places
	LcdMediumDigit(64, 0, (degreesC % 10));
	
	//print the tens place 
	degreesC=(degreesC-(degreesC % 10))/10;
	if((degreesC % 10))	{
		LcdMediumDigit(52, 0, (degreesC % 10));
	}
	else {
		LcdClearBlock(52, 0, 12, 2);
	}
	//print the degree symbol
	LcdGliff(79, 0, degree_symbol);
}

void LcdBootAnimation()
{
	// animate a startup sequence, filling the display from the bottom to the top of the display with horizontal bars
	// The reason is to allow the ADC to stabilize the internal values after the boot
	for(int8_t y = 5; y >= 0; y--)
	{		
		LcdGotoXY(21, y);
		for(uint8_t x=0; x < 42; x++) // fill full block
		{
			LcdWrite(LCD_DATA, 0xff);
		}
		_delay_ms(100);
	}
}

int main(void)
{
	//initialize the chip registers
	init();
	LcdInitialise();
	
	// enable interrupt service routines, we need these for the ADC
	sei();

	// animate a startup sequence, filling the display from the bottom to the top of the display with horizontal bars
	// The reason is to allow the ADC to stabilize the internal values after the boot 
	LcdBootAnimation();
	
	// enable watch dog, careful that timeout is longer than loop sleep time
	wdt_enable(WDTO_250MS);	
	
	uint8_t i = 0xff;
	while(1)
	{
		CalculateTemp();
		CalculateGear();
		CalculateVoltage();
		
		if(i++ == 0)
		{
			// every 255 loops interject a full screen clear
			LcdClear();
		}
		
		LcdDrawGear();
		LcdDrawTemp();
		LcdDrawVoltage();
		
		// draw the divider grid to separate the display into three components
		//      | TEMP
		// GEAR |-----
		// POS  | BATT
		LcdDrawHLine(42,3,42);
		LcdDrawVLine(42,0,6);
		
		//delay to slow looping approx 40fps
		_delay_ms(DELAY_MS);
		wdt_reset();
	}
	
	return 0;
}
