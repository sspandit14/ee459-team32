#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#define F_WIDTH 5
#define F_HEIGHT 7

const uint8_t* get_glyph(char c);

#endif