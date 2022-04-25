/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/********************************************************************************
 * @file     dwin_lcd.cpp
 * @author   
 * @date     2019/07/18
 * @version  2.0.1
 * @brief    DWIN screen control functions
 ********************************************************************************/

#include "../../inc/MarlinConfig.h"

#if HAS_DWIN_LCD
#include "dwin_ui/dwin.h"
#include "dwin_lcd.h"
#include <string.h> // for memset

DWINLCD dwinLCD;

uint8_t DWIN_SendBuf[DWIN_SENDBUF_SIZE] = { 0xAA };
uint8_t DWIN_BufTail[4] = { 0xCC, 0x33, 0xC3, 0x3C };
uint8_t databuf[26] = { 0 };
uint8_t receivedType;
short recnum = 0;

FORCE_INLINE static void DWIN_Byte(size_t &i, const uint16_t bval) {
	DWIN_SendBuf[++i] = bval;
}

FORCE_INLINE static void DWIN_Word(size_t &i, const uint16_t wval) {
	DWIN_SendBuf[++i] = wval >> 8;
	DWIN_SendBuf[++i] = wval & 0xFF;
}

FORCE_INLINE static void DWIN_Long(size_t &i, const uint32_t lval) {
	DWIN_SendBuf[++i] = (lval >> 24) & 0xFF;
	DWIN_SendBuf[++i] = (lval >> 16) & 0xFF;
	DWIN_SendBuf[++i] = (lval >>	8) & 0xFF;
	DWIN_SendBuf[++i] = lval & 0xFF;
}

FORCE_INLINE static void DWIN_String(size_t &i, char * const string) {
	const size_t len = _MIN(sizeof(DWIN_SendBuf) - i, strlen(string));
	memcpy(&DWIN_SendBuf[i+1], string, len);
	i += len;
}

FORCE_INLINE static void DWIN_String(size_t &i, const __FlashStringHelper * string) {
	if (!string) return;
	const size_t len = strlen_P((PGM_P)string); // cast it to PGM_P, which is basically const char *, and measure it using the _P version of strlen.
	if (len == 0) return;
	memcpy(&DWIN_SendBuf[i+1], string, len);
	i += len;
}

// Send the data in the buffer and the packet end
FORCE_INLINE static void DWIN_Send(size_t &i) {
	++i;
	LOOP_L_N(n, i) { LCD_SERIAL.write(DWIN_SendBuf[n]); delayMicroseconds(1); }
	LOOP_L_N(n, 4) { LCD_SERIAL.write(DWIN_BufTail[n]); delayMicroseconds(1); }
}

/*-------------------------------------- System variable function --------------------------------------*/

// Handshake (1: Success, 0: Fail)
// Tx: AA 00 CC 33 C3 3C
// Rx: AA 00 4F 4B CC 33 C3 3C
bool DWINLCD::Handshake(void) {
  size_t i = 0;
  DWIN_Byte(i, 0x00);
  DWIN_Send(i);

  while (LCD_SERIAL.available() > 0 && recnum < (signed)sizeof(databuf)) {
    databuf[recnum] = LCD_SERIAL.read();
    // ignore the invalid data
    if (databuf[0] != FHONE) { // prevent the program from running.
      if (recnum > 0) {
        recnum = 0;
        ZERO(databuf);
      }
      continue;
    }
    delay(10);
    recnum++;
  }

  return ( recnum >= 3
        && databuf[0] == FHONE
        && databuf[1] == '\0'
        && databuf[2] == 'O'
        && databuf[3] == 'K' );
}

// Set the backlight luminance
//  luminance: (0x00-0xFF)
void DWINLCD::Set_Backlight_Luminance(const uint8_t luminance) {
  size_t i = 0;
  DWIN_Byte(i, 0x30);
  DWIN_Byte(i, _MAX(luminance, 0x1F));
  DWIN_Send(i);
}

// Set screen display direction
//  dir: 0=0°, 1=90°, 2=180°, 3=270°
void DWINLCD::Frame_SetDir(uint8_t dir) {
  size_t i = 0;
  DWIN_Byte(i, 0x34);
  DWIN_Byte(i, 0x5A);
  DWIN_Byte(i, 0xA5);
  DWIN_Byte(i, dir);
  DWIN_Send(i);
}

// Update display
void DWINLCD::UpdateLCD(void) {
  size_t i = 0;
  DWIN_Byte(i, 0x3D);
  DWIN_Send(i);
}

/*---------------------------------------- Drawing functions ----------------------------------------*/

// Clear screen
//  color: Clear screen color
void DWINLCD::Frame_Clear(const uint16_t color) {
  size_t i = 0;
  DWIN_Byte(i, 0x01);
  DWIN_Word(i, color);
  DWIN_Send(i);
}

// Draw a point
//  width: point width   0x01-0x0F
//  height: point height 0x01-0x0F
//  x,y: upper left point
void DWINLCD::Draw_Point(uint8_t width, uint8_t height, uint16_t x, uint16_t y) {
  size_t i = 0;
  DWIN_Byte(i, 0x02);
  DWIN_Byte(i, width);
  DWIN_Byte(i, height);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}

// Draw a line
//  color: Line segment color
//  xStart/yStart: Start point
//  xEnd/yEnd: End point
void DWINLCD::Draw_Line(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x03);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

// Draw a rectangle
//  mode: 0=frame, 1=fill, 2=XOR fill
//  color: Rectangle color
//  xStart/yStart: upper left point
//  xEnd/yEnd: lower right point
void DWINLCD::Draw_Rectangle(uint8_t mode, uint16_t color,
                         uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x05);
  DWIN_Byte(i, mode);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

// Move a screen area
//  mode: 0, circle shift; 1, translation
//  dir: 0=left, 1=right, 2=up, 3=down
//  dis: Distance
//  color: Fill color
//  xStart/yStart: upper left point
//  xEnd/yEnd: bottom right point
void DWINLCD::Frame_AreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
                         uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x09);
  DWIN_Byte(i, (mode << 7) | dir);
  DWIN_Word(i, dis);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

/*---------------------------------------- Text related functions ----------------------------------------*/

// Draw a string
//  widthAdjust: true=self-adjust character width; false=no adjustment
//  bShow: true=display background color; false=don't display background color
//  size: Font size
//  color: Character color
//  bColor: Background color
//  x/y: Upper-left coordinate of the string
//  *string: The string
void DWINLCD::Draw_String(bool widthAdjust, bool bShow, uint8_t size,
                      uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, char *string) {
  size_t i = 0;
  DWIN_Byte(i, 0x11);
  // Bit 7: widthAdjust
  // Bit 6: bShow
  // Bit 5-4: Unused (0)
  // Bit 3-0: size
  DWIN_Byte(i, (widthAdjust * 0x80) | (bShow * 0x40) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_String(i, string);
  DWIN_Send(i);
}
void DWINLCD::Draw_String(bool widthAdjust, bool bShow, uint8_t size,
                      uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, PGM_P string) {
  size_t i = 0;	
  DWIN_Byte(i, 0x11);
  // Bit 7: widthAdjust
  // Bit 6: bShow
  // Bit 5-4: Unused (0)
  // Bit 3-0: size
  DWIN_Byte(i, (widthAdjust * 0x80) | (bShow * 0x40) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
	char str_buf = 0;
	for(uint8_t j = DWIN_SENDBUF_SIZE-12; j>0; j--) { 
		str_buf = pgm_read_byte(string);
		string++;
		DWIN_Byte(i, str_buf);
		if(str_buf == 0) break;
	}		 
  DWIN_Send(i);
}
					  
// Draw a positive integer
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  color: Character color
//  bColor: Background color
//  iNum: Number of digits
//  x/y: Upper-left coordinate
//  value: Integer value
void DWINLCD::Draw_IntValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, uint32_t value) {
  size_t i = 0;
  DWIN_Byte(i, 0x14);
  // Bit 7: bshow
  // Bit 6: 1 = signed; 0 = unsigned number;
  // Bit 5: zeroFill
  // Bit 4: zeroMode
  // Bit 3-0: size
  DWIN_Byte(i, (bShow * 0x80) | (zeroFill * 0x20) | (zeroMode * 0x10) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, 0); // fNum
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  #if 0
    for (char count = 0; count < 8; count++) {
      DWIN_Byte(i, value);
      value >>= 8;
      if (!(value & 0xFF)) break;
    }
  #else
    // Write a big-endian 64 bit integer
    const size_t p = i + 1;
    for (char count = 8; count--;) { // 7..0
      ++i;
      DWIN_SendBuf[p + count] = value;
      value >>= 8;
    }
  #endif

  DWIN_Send(i);
}

// Draw a floating point number
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  color: Character color
//  bColor: Background color
//  iNum: Number of whole digits
//  fNum: Number of decimal digits
//  x/y: Upper-left point
//  value: Float value
void DWINLCD::Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value) {
  size_t i = 0;
  DWIN_Byte(i, 0x14);
  DWIN_Byte(i, (bShow * 0x80) | (zeroFill * 0x20) | (zeroMode * 0x10) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, fNum);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Long(i, value);
  DWIN_Send(i);
}

void DWINLCD::Draw_SignedFloatValue(uint8_t size, uint16_t Color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value) {			
	uint8_t w = 6;
	if(size <= 5) 
		w = 6 + size*2;
	else if(size <= 9) 
		w = 6 + size*4;
	if (value < 0) {
		Draw_String(true, true, size, Color, bColor, x, y, PSTR("-"));
		Draw_FloatValue(true, true, 0, size, Color, bColor, iNum, fNum, x+w, y, -value);
	}
	else {
		Draw_String(true, true, size, Color, bColor, x, y, PSTR(" "));
		Draw_FloatValue(true, true, 0, size, Color, bColor, iNum, fNum, x+w, y, value);
	}
}
/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw JPG and cached in #0 virtual display area
// id: Picture ID
void DWINLCD::JPG_ShowAndCache(const uint8_t id) {
  size_t i = 0;
  DWIN_Word(i, 0x2200);
  DWIN_Byte(i, id);
  DWIN_Send(i);
}

//Draw a QR code
//x/y: Upper-left point
//pixcel: Pixel size occupied by each point of QR code
//data: Display data, up to 154 bytes.
void DWINLCD::Draw_QRCode(const uint16_t x, const uint16_t y, const uint8_t pixcel, char *string) {
	size_t i = 0;	
  DWIN_Byte(i, 0x21);
  DWIN_Word(i, x);
  DWIN_Word(i, y); 
	DWIN_Byte(i, pixcel);
	DWIN_String(i, string);
  DWIN_Send(i);
}

void DWINLCD::Draw_QRCode(const uint16_t x, const uint16_t y, const uint8_t pixcel, PGM_P string) {
	size_t i = 0;	
  DWIN_Byte(i, 0x21);
  DWIN_Word(i, x);
  DWIN_Word(i, y); 
	DWIN_Byte(i, pixcel);
	char str_buf = 0;
	for(uint8_t j = DWIN_SENDBUF_SIZE-7; j>0; j--) {
		str_buf = pgm_read_byte(string);
		DWIN_Byte(i, str_buf);
		if(str_buf == 0) break;
		string++;
	}
  DWIN_Send(i);
}


// Draw an Icon
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWINLCD::ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  NOMORE(x, (uint16_t)(DWIN_WIDTH - 1));
  NOMORE(y, (uint16_t)(DWIN_HEIGHT - 1)); // -- ozy -- srl
  size_t i = 0;
  DWIN_Byte(i, 0x23);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Byte(i, 0x80 | libID);
  DWIN_Byte(i, picID);
  DWIN_Send(i);
}

// Unzip the JPG picture to a virtual display area
//  n: Cache index
//  id: Picture ID
void DWINLCD::JPG_CacheToN(uint8_t n, uint8_t id) {
  size_t i = 0;
  DWIN_Byte(i, 0x25);
  DWIN_Byte(i, n);
  DWIN_Byte(i, id);
  DWIN_Send(i);
}

// Copy area from virtual display area to current screen
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWINLCD::Frame_AreaCopy(uint8_t cacheID, uint16_t xStart, uint16_t yStart,
                         uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y) {
  size_t i = 0;
  DWIN_Byte(i, 0x26);
  DWIN_Byte(i, 0x80 | cacheID);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}

//  Copy area from virtual display area to current screen
//  PixtureID: pixture index number
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
//void Frame_AreaCopy_Index(uint8_t PixtureID, uint8_t cacheID, uint16_t xStart, uint16_t yStart,
                         //uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y) {
void DWINLCD::Frame_AreaCopy_Index(uint8_t cacheID, uint8_t PixtureID,  uint8_t ItemID, const uint16_t *array,uint16_t x,uint16_t y) {
   size_t i = 0;
  DWIN_Byte(i, 0x27);
  DWIN_Byte(i, 0x80 | cacheID);
  DWIN_Word(i, *array++);
  DWIN_Word(i, *array++);
  DWIN_Word(i, *array++);
  DWIN_Word(i, *array);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}

// Animate a series of icons
//  animID: Animation ID; 0x00-0x0F
//  animate: true on; false off;
//  libID: Icon library ID
//  picIDs: Icon starting ID
//  picIDe: Icon ending ID
//  x/y: Upper-left point
//  interval: Display time interval, unit 10mS
void DWINLCD::ICON_Animation(uint8_t animID, bool animate, uint8_t libID, uint8_t picIDs, uint8_t picIDe, uint16_t x, uint16_t y, uint16_t interval) {
  NOMORE(x, (uint16_t)(DWIN_WIDTH - 1));
  NOMORE(y, (uint16_t)(DWIN_HEIGHT - 1)); // -- ozy -- srl
  size_t i = 0;
  DWIN_Byte(i, 0x28);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  // Bit 7: animation on or off
  // Bit 6: start from begin or end
  // Bit 5-4: unused (0)
  // Bit 3-0: animID
  DWIN_Byte(i, (animate * 0x80) | 0x40 | animID);
  DWIN_Byte(i, libID);
  DWIN_Byte(i, picIDs);
  DWIN_Byte(i, picIDe);
  DWIN_Byte(i, interval);
  DWIN_Send(i);
}

// Animation Control
//  state: 16 bits, each bit is the state of an animation id
void DWINLCD::ICON_AnimationControl(uint16_t state) {
  size_t i = 0;
  DWIN_Byte(i, 0x28);
  DWIN_Word(i, state);
  DWIN_Send(i);
}

/*---------------------------------------- Memory functions ----------------------------------------*/
// The LCD has an additional 32KB SRAM and 16KB Flash

// Data can be written to the sram and save to one of the jpeg page files

// Write Data Memory
//  command 0x31
//  Type: Write memory selection; 0x5A=SRAM; 0xA5=Flash
//  Address: Write data memory address; 0x000-0x7FFF for SRAM; 0x000-0x3FFF for Flash
//  Data: data
//
//  Flash writing returns 0xA5 0x4F 0x4B

// Read Data Memory
//  command 0x32
//  Type: Read memory selection; 0x5A=SRAM; 0xA5=Flash
//  Address: Read data memory address; 0x000-0x7FFF for SRAM; 0x000-0x3FFF for Flash
//  Length: leangth of data to read; 0x01-0xF0
//
//  Response:
//    Type, Address, Length, Data

// Write Picture Memory
//  Write the contents of the 32KB SRAM data memory into the designated image memory space
//  Issued: 0x5A, 0xA5, PIC_ID
//  Response: 0xA5 0x4F 0x4B
//
//  command 0x33
//  0x5A, 0xA5
//  PicId: Picture Memory location, 0x00-0x0F
//
//  Flash writing returns 0xA5 0x4F 0x4B

#endif // HAS_DWIN_LCD
