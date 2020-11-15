/*
 * fonts.h
 *
 * Library of fonts
 * Large: 8x8, normal: 5x7 and small: 4x7
 */
#ifndef FONTS_H_
#define FONTS_H_

//#include "fonts.h"
#include <avr/pgmspace.h>

 //Font 8x8 - Large
const unsigned char font8[95][8];
// Font 5x7 - normal
const unsigned char font5[95][5];
// Font 4x6 - Small
const unsigned char font4[95][4];

#endif /* FONTS_H_ */