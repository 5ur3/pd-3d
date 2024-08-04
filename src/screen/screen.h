#ifndef __SCREEN

#include "pd_api.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGTH 240
#define SCREEN_ROWS 52

typedef struct Screen {
    LCDBitmap *lcdbm;
    uint8_t *bm;
    PlaydateAPI *pd;
    uint16_t *ditherMap;
    uint16_t ditherMapSize;
    uint16_t ditherMaxBrightness;
} Screen;

Screen *NewScreen(PlaydateAPI *pd);
void DrawScreen(Screen *s);
void FillScreen(Screen *s, int enabled);
#define bmindex(x, y) (y * SCREEN_ROWS + (x >> 3))
#define bmmask(x) (1 << (7 - (x & 7)))
#define SetPixel(s, x, y, enabled)                                             \
    (enabled ? (s->bm[bmindex(x, y)] |= bmmask(x))                             \
             : (s->bm[bmindex(x, y)] &= ~bmmask(x)))
void SetSquare(Screen *s, int x, int y, int size, int enabled);
void DrawLine(Screen *s, float x1, float y1, float x2, float y2, int enabled);
void SetDitheredPixel(Screen *s, short x, short y, uint16_t brighness);
void DrawTriangle(Screen *s, int x1, int y1, int x2, int y2, int x3, int y3,
                  int enabled);
void DrawDitheredTriangle(Screen *s, int x1, int y1, int x2, int y2, int x3,
                          int y3, uint16_t brighness);

#define __SCREEN
#endif
