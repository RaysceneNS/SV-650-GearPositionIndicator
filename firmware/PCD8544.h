#ifndef PCD8544_H_
#define PCD8544_H_

#include <avr/pgmspace.h>
#include <avr/io.h>
#include "board_attiny44.h"
#include "bitmap.h"

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define LCD_X     84
#define LCD_Y     48
#define LCD_CMD   0
#define LCD_DATA  1


// PCD8544 Command set
// ------------------
// General commands
#define PCD8544_POWERDOWN			0x04
#define PCD8544_ENTRYMODE			0x02
#define PCD8544_EXTENDEDINSTRUCTION	0x01
#define PCD8544_DISPLAYBLANK		0x00
#define PCD8544_DISPLAYNORMAL		0x04
#define PCD8544_DISPLAYALLON		0x01
#define PCD8544_DISPLAYINVERTED		0x05
// Normal instruction set
#define PCD8544_FUNCTIONSET			0x20
#define PCD8544_DISPLAYCONTROL		0x08
#define PCD8544_SETYADDR			0x40
#define PCD8544_SETXADDR			0x80
// Extended instruction set
#define PCD8544_SETTEMP				0x04
#define PCD8544_SETBIAS				0x10
#define PCD8544_SETVOP				0x80
// Display presets
#define LCD_BIAS					0x03	// Range: 0-7 (0x00-0x07)
#define LCD_TEMP					0x02	// Range: 0-3 (0x00-0x03)
#define LCD_CONTRAST				0x46	// Range: 0-127 (0x00-0x7F)


void LcdBitmap(uint8_t x, uint8_t y, const uint8_t *BMP);
void LcdDrawVLine(uint8_t x, uint8_t y, uint8_t height);
void LcdDrawHLine(uint8_t x, uint8_t y, uint8_t width);
void LcdGotoXY(uint8_t x, uint8_t y);
void LcdWrite(uint8_t dc, uint8_t data);
void LcdInitialise(void);
void LcdClear(void);
void LcdGliff(uint8_t x, uint8_t y, const uint8_t *BMP);
void LcdMediumDigit(uint8_t x, uint8_t y, uint8_t c);

#endif /* PCD8544_H_ */