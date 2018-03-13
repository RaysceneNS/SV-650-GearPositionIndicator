#ifndef BITMAP_H_
#define BITMAP_H_

/*
All bitmaps are RLE encoded 48x42

due to encoding scheme no solid area can be more than 255 bytes in length
*/

static const unsigned char BMP_CLEAR[] PROGMEM = {
	'\x00','\x00','\xf0',
};

static const unsigned char BMP_FIRST[] PROGMEM = {
	'\x00','\x00','\x1d','\xf0','\xf8','\xfc','\xfe','\xff', // 0x0008
	'\xff','\x04','\x00','\x00','\x20','\xff','\xff','\x08', // 0x0010
	'\x00','\x00','\x20','\x1f','\x3f','\x7f','\xff','\xff', // 0x0018
	'\x05','\x00','\x00','\x20','\xf0','\xf8','\xfc','\xfe', // 0x0020
	'\xff','\xff','\x04','\x00','\x00','\x20','\xff','\xff', // 0x0028
	'\x08','\x00','\x00','\x20','\x0f','\x1f','\x3f','\x7f', // 0x0030
	'\xff','\xff','\x04','\x00','\x00','\x03'
};

static const unsigned char BMP_SECOND[] PROGMEM = {
	'\x00','\x00','\x05','\x0f','\x1f','\x3f','\x7f','\xff', // 0x0008
	'\xff','\x18','\xfe','\xfc','\xf8','\xf0','\x00','\x00', // 0x0010
	'\x20','\xff','\xff','\x08','\x00','\x00','\x09','\x80', // 0x0018
	'\xc0','\xe0','\xf0','\xf0','\x14','\xff','\xff','\x08', // 0x0020
	'\x00','\x00','\x08','\xff','\xff','\x08','\x0f','\x0f', // 0x0028
	'\x14','\x07','\x03','\x01','\x00','\x00','\x09','\xff', // 0x0030
	'\xff','\x08','\x00','\x00','\x20','\x0f','\x1f','\x3f', // 0x0038
	'\x7f','\xff','\xff','\x18','\xfe','\xfc','\xf8','\xf0', // 0x0040
	'\x00','\x00','\x03'
};

static const unsigned char BMP_THIRD[] PROGMEM = {
	'\x00','\x00','\x05','\x0f','\x1f','\x3f','\x7f','\xff', // 0x0008
	'\xff','\x18','\xfe','\xfc','\xf8','\xf0','\x00','\x00', // 0x0010
	'\x20','\xff','\xff','\x08','\x00','\x00','\x09','\x80', // 0x0018
	'\xc0','\xe0','\xf0','\xf0','\x14','\xff','\xff','\x08', // 0x0020
	'\x00','\x00','\x09','\x01','\x03','\x07','\x0f','\x0f', // 0x0028
	'\x14','\xff','\xff','\x08','\x00','\x00','\x20','\xff', // 0x0030
	'\xff','\x08','\x00','\x00','\x08','\xf0','\xf8','\xfc', // 0x0038
	'\xfe','\xff','\xff','\x18','\x7f','\x3f','\x1f','\x0f', // 0x0040
	'\x00','\x00','\x03'
};

static const unsigned char BMP_FOURTH[] PROGMEM = {
	'\x00','\x00','\x05','\xff','\xff','\x04','\xfe','\xfc', // 0x0008
	'\xf8','\xf0','\x00','\x00','\x10','\xf0','\xf8','\xfc', // 0x0010
	'\xfe','\xff','\xff','\x04','\x00','\x00','\x08','\xff', // 0x0018
	'\xff','\x08','\x00','\x00','\x10','\xff','\xff','\x08', // 0x0020
	'\x00','\x00','\x08','\x7f','\xff','\xff','\x07','\xf0', // 0x0028
	'\xf0','\x10','\xff','\xff','\x08','\x00','\x00','\x09', // 0x0030
	'\x01','\x03','\x07','\x0f','\x0f','\x14','\xff','\xff', // 0x0038
	'\x08','\x00','\x00','\x20','\xff','\xff','\x08','\x00', // 0x0040
	'\x00','\x20','\x0f','\x1f','\x3f','\x7f','\xff','\xff', // 0x0048
	'\x04','\x00','\x00','\x03',
};

static const unsigned char BMP_FIFTH[] PROGMEM = {
	'\x00','\x00','\x05','\xf0','\xf8','\xfc','\xfe','\xff', // 0x0008
	'\xff','\x18','\x7f','\x3f','\x1f','\x0f','\x00','\x00', // 0x0010
	'\x08','\xff','\xff','\x08','\x00','\x00','\x20','\xff', // 0x0018
	'\xff','\x08','\xf0','\xf0','\x14','\xe0','\xc0','\x80', // 0x0020
	'\x00','\x00','\x0a','\x01','\x03','\x07','\x0f','\x0f', // 0x0028
	'\x14','\xff','\xff','\x08','\x00','\x00','\x20','\xff', // 0x0030
	'\xff','\x08','\x00','\x00','\x08','\xf0','\xf8','\xfc', // 0x0038
	'\xfe','\xff','\xff','\x18','\x7f','\x3f','\x1f','\x0f', // 0x0040
	'\x00','\x00','\x03',
};

static const unsigned char BMP_SIXTH[] PROGMEM = {
	'\x00','\x00','\x05','\xf0','\xf8','\xfc','\xfe','\xff', // 0x0008
	'\xff','\x18','\x7f','\x3f','\x1f','\x0f','\x00','\x00', // 0x0010
	'\x08','\xff','\xff','\x08','\x00','\x00','\x20','\xff', // 0x0018
	'\xff','\x08','\xf0','\xf0','\x14','\xe0','\xc0','\x80', // 0x0020
	'\x00','\x00','\x09','\xff','\xff','\x08','\x0f','\x0f', // 0x0028
	'\x10','\xff','\xff','\x08','\x00','\x00','\x08','\xff', // 0x0030
	'\xff','\x08','\x00','\x00','\x10','\xff','\xff','\x08', // 0x0038
	'\x00','\x00','\x08','\x0f','\x1f','\x3f','\x7f','\xff', // 0x0040
	'\xff','\x18','\x7f','\x3f','\x1f','\x0f','\x00','\x00', // 0x0048
	'\x03',
};

static const unsigned char BMP_NEUTRAL[] PROGMEM = {
	'\x00','\x00','\x05','\xe0','\xe0','\x04','\xc0','\x80', // 0x0008
	'\x00','\x00','\x12','\xe0','\xe0','\x08','\x00','\x00', // 0x0010
	'\x08','\xff','\xff','\x07','\xfe','\xfc','\xf8','\xf0', // 0x0018
	'\xe0','\xc0','\x80','\x00','\x00','\x0a','\xff','\xff', // 0x0020
	'\x08','\x00','\x00','\x08','\xff','\xff','\x08','\x7f', // 0x0028
	'\xff','\xff','\x06','\xfe','\xfc','\xf8','\xf0','\xe0', // 0x0030
	'\xc0','\x80','\x00','\x00','\x02','\xff','\xff','\x08', // 0x0038
	'\x00','\x00','\x08','\xff','\xff','\x08','\x00','\x00', // 0x0040
	'\x02','\x01','\x03','\x07','\x0f','\x1f','\x3f','\x7f', // 0x0048
	'\xff','\xff','\x06','\xfe','\xff','\xff','\x08','\x00', // 0x0050
	'\x00','\x08','\xff','\xff','\x08','\x00','\x00','\x0a', // 0x0058
	'\x01','\x03','\x07','\x0f','\x1f','\x3f','\x7f','\xff', // 0x0060
	'\xff','\x07','\x00','\x00','\x08','\x07','\x07','\x08', // 0x0068
	'\x00','\x00','\x12','\x01','\x03','\x07','\x07','\x04', // 0x0070
	'\x00','\x00','\x03',
};

// gliffs are 8x5
static const uint8_t degree_symbol[] PROGMEM = {0x38, 0x44, 0x44, 0x44, 0x38};  // o
static const uint8_t volt_symbol[] PROGMEM =   {0x1c, 0x20, 0x40, 0x20, 0x1c};  // v
static const uint8_t decimal[] PROGMEM =       {0x00, 0x00, 0x60, 0x60, 0x00};  // .
static const uint8_t hyphen[] PROGMEM =        {0x06, 0x06, 0x06, 0x06, 0x00};  // -


// medium numbers are 16 high x 12 wide and have padding left and right
static const uint8_t MEDIUMNUMBERS[][24] PROGMEM =
{
	{0x00, 0xfc, 0x7a, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbc, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xbc, 0x7e, 0x00},   // 0
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x00},   // 1
	{0x00, 0x00, 0x02, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x81, 0x00, 0x00},   // 2
	{0x00, 0x00, 0x02, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00},   // 3
	{0x00, 0xfc, 0x78, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x78, 0xfc, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3d, 0x7e, 0x00},   // 4
	{0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x02, 0x00, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00},   // 5
	{0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x02, 0x00, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00},   // 6
	{0x00, 0x00, 0x02, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x00},   // 7
	{0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00},   // 8
	{0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00},   // 9
};

#endif /* BITMAP_H_ */