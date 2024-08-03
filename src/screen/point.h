#ifndef _SCREEN_POINT

#include <stdint.h>

typedef struct point {
    uint16_t x;
    uint16_t y;
} point;

typedef struct points {
    point *points;
    uint32_t len;
} points;

#define _SCREEN_POINT
#endif
