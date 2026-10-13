#ifndef PONG_FONT_H
#define PONG_FONT_H

#include <stdint.h>

#define PONG_FONT_FIRST_CHAR 32
#define PONG_FONT_LAST_CHAR 126
#define PONG_FONT_CHAR_WIDTH 6
#define PONG_FONT_CHAR_HEIGHT 10
#define PONG_FONT_BYTES_PER_COLUMN 2

extern const uint8_t font[];

#endif
