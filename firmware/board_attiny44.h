#ifndef ATTINY44PINS_H_
#define ATTINY44PINS_H_

//Wiring setup for ATTiny44A uC
/*
                 ----------
            VCC | 1     14 | GND
 LCD_CLK <- PB0 | 2     13 | PA0 <- BATT
 LCD_DIN <- PB1 | 3     12 | PA1 <- TEMP
      N/C x PB3 | 4     11 | PA2 <- GEAR
  LCD_DC <- PB2 | 5     10 | PA3 x N/C 
 LCD_SCE <- PA7 | 6      9 | PA4 x N/C
 LCD_RST <- PA6 | 7      8 | PA5 x N/C
                 ----------
*/

#define DDRA_BOARD 0b11100000;
#define DDRB_BOARD 0b00000111;


#define PORT_LCD_SCE PORTA
#define PIN_LCD_SCE PORTA7
#define PORT_LCD_RST PORTA
#define PIN_LCD_RST PORTA6
#define PORT_LCD_DC PORTB
#define PIN_LCD_DC PORTB2
#define PORT_LCD_DIN PORTB
#define PIN_LCD_DIN PORTB1
#define PORT_LCD_CLK PORTB
#define PIN_LCD_CLK PORTB0


#define PIN_BATT PINA0
#define PIN_TEMP PINA1
#define PIN_GEAR PINA2

#define BATT_ADC_CH 0b00000000	// use VCC for AREF, use ADC1 for input (PA0), MUX 5:0 bits 00000
#define TEMP_ADC_CH 0b00000001	// use VCC for AREF, use ADC1 for input (PA1), MUX 5:0 bits 00001
#define GEAR_ADC_CH 0b00000010	// use VCC for AREF, use ADC1 for input (PA2), MUX 5:0 bits 00010

#endif /* ATTINY44PINS_H_ */