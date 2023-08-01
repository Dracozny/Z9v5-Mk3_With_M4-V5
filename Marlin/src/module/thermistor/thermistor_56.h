/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once
// QWG 104F B3950 thermistor 4.7 kOhm pull-up
const temp_entry_t temptable_56[] PROGMEM = {
#if (MOTHERBOARD == BOARD_ZONESTAR_ZM3E4)
  { OV(  17), 330 },
  { OV(  19), 315 },
  { OV(  20), 310 },
  { OV(  21), 300 },
  { OV(  23), 295 },
  { OV(  25), 290 },
  { OV(  27), 285 },
  { OV(  28), 280 },
  { OV(  31), 275 },
  { OV(  33), 270 },
  { OV(  35), 265 },
  { OV(  38), 260 },
  { OV(  41), 255 },
  { OV(  44), 250 },
  { OV(  48), 245 },  
  { OV(  50), 240 },  
  { OV(  52), 235 },
  { OV(  54), 230 },
  { OV(  56), 225 },
  { OV(  59), 220 },
  { OV(  63), 215 },
  { OV(  68), 210 },  
  { OV(  78), 205 },
  { OV(  92), 200 },
  { OV( 100), 195 },
  { OV( 110), 190 },
  { OV( 120), 185 },
  { OV( 140), 180 },
  { OV( 148), 175 },
  { OV( 156), 170 },
  { OV( 171), 165 },
  { OV( 187), 160 },
  { OV( 205), 155 },
  { OV( 224), 150 },
  { OV( 268), 145 },
  { OV( 293), 140 },
  { OV( 320), 135 },
  { OV( 348), 130 },
  { OV( 379), 125 },
  { OV( 428), 120 },
  { OV( 476), 115 },
  { OV( 516), 110 },
  { OV( 553), 105 },
  { OV( 591), 100 },
  { OV( 628),  95 },
  { OV( 665),  90 },
  { OV( 702),  85 },
  { OV( 737),  80 },
  { OV( 770),  75 },
  { OV( 801),  70 },
  { OV( 830),  65 },
  { OV( 857),  60 },
  { OV( 881),  55 },
  { OV( 903),  50 },
  { OV( 922),  45 },
  { OV( 939),  40 },
  { OV( 954),  35 },
  { OV( 966),  30 },
  { OV( 977),  25 },
  { OV( 993),  20 },
  { OV(1010),  15 },
  { OV(1015),  10 },
  { OV(1017),   5 },
  { OV(1018),   0 },
  { OV(1019),  -5 },
  { OV(1020),  -10},
  { OV(1022),  -15}
#elif (MOTHERBOARD == BOARD_ZONESTAR_ZM3E4V2) 
	// correct in 2023-07-11
  {  OV(  17), 315},
 	{  OV(  19), 310},
  {  OV(  21), 305},
  {  OV(  23), 300},
  {  OV(  25), 295},
	{  OV(  27), 290},
	{  OV(  29), 285},
	{  OV(  31), 280},
	{  OV(  33), 275},
	{  OV(  36), 270},
	{  OV(  39), 265},
	{  OV(  42), 260},
	{  OV(  45), 255},
	{  OV(  49), 250},
	{  OV(  53), 245},
	{  OV(  57), 240},
	{  OV(  62), 235},
	{  OV(  68), 230},
	{  OV(  74), 225},
	{  OV(  80), 220},
	{  OV(  87), 215},
	{  OV(  95), 210},
	{  OV( 104), 205},
	{  OV( 114), 200},
	{  OV( 124), 195},
	{  OV( 136), 190},
	{  OV( 149), 185},
	{  OV( 163), 180},
	{  OV( 179), 175},
	{  OV( 196), 170},
	{  OV( 214), 165},
	{  OV( 234), 160},
	{  OV( 256), 155},
	{  OV( 280), 150},
	{  OV( 306), 145},
	{  OV( 334), 140},
	{  OV( 363), 135},
	{  OV( 395), 130},
	{  OV( 428), 125},
	{  OV( 462), 120},
	{  OV( 498), 115},
	{  OV( 535), 110},
	{  OV( 573), 105},
	{  OV( 611), 100},
	{  OV( 649),  95},
	{  OV( 686),  90},
	{  OV( 723),  85},
	{  OV( 758),  80},
	{  OV( 791),  75},
	{  OV( 822),  70},
	{  OV( 852),  65},
	{  OV( 878),  60},
	{  OV( 903),  55},
	{  OV( 924),  50},
	{  OV( 944),  45},
	{  OV( 960),  40},
	{  OV( 975),  35},
	{  OV( 988),  30},
	{  OV( 998),  25},
	{  OV(1002),  20},
	{  OV(1007),  15},
	{  OV(1016),  10},
	{  OV(1018),   5},
	{  OV(1019),   0},
	{  OV(1020),  -5},
	{  OV(1021), -10},
	{  OV(1022), -15}
#else
	// correct in 2023-07-11
 	{ OV(  17), 315 },
  { OV(  18), 310 },
  { OV(  20), 305 },
  { OV(  23), 300 },
  { OV(  25), 295 },
  { OV(  27), 290 },
  { OV(  28), 285 },
  { OV(  31), 280 },
  { OV(  33), 275 },
  { OV(  35), 270 },
  { OV(  38), 265 },
  { OV(  41), 260 },
  { OV(  44), 255 },
  { OV(  48), 250 },
  { OV(  52), 245 },
  { OV(  56), 240 },
  { OV(  61), 235 },
  { OV(  66), 230 },
  { OV(  71), 225 },
  { OV(  78), 220 },
  { OV(  84), 215 },
  { OV(  92), 210 },
  { OV( 100), 205 },
  { OV( 109), 200 },
  { OV( 120), 195 },
  { OV( 131), 190 },
  { OV( 143), 185 },
  { OV( 156), 180 },
  { OV( 171), 175 },
  { OV( 187), 170 },
  { OV( 205), 165 },
  { OV( 224), 160 },
  { OV( 245), 155 },
  { OV( 268), 150 },
  { OV( 293), 145 },
  { OV( 320), 140 },
  { OV( 348), 135 },
  { OV( 379), 130 },
  { OV( 411), 125 },
  { OV( 445), 120 },
  { OV( 480), 115 },
  { OV( 516), 110 },
  { OV( 553), 105 },
  { OV( 591), 100 },
  { OV( 628),  95 },
  { OV( 665),  90 },
  { OV( 702),  85 },
  { OV( 737),  80 },
  { OV( 770),  75 },
  { OV( 801),  70 },
  { OV( 830),  65 },
  { OV( 857),  60 },
  { OV( 881),  55 },
  { OV( 903),  50 },
  { OV( 922),  45 },
  { OV( 939),  40 },
  { OV( 954),  35 },
  { OV( 966),  30 },
  { OV( 977),  25 },
  { OV( 985),  20 },
  { OV( 993),  15 },
  { OV( 999),  10 },
  { OV(1004),   5 },
  { OV(1008),   0 },
  { OV(1013),  -5 },
  { OV(1016), -10 },
  { OV(1019), -15 }
#endif
};
