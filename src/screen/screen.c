#define TARGET_EXTENSION 1
#include "screen.h"
#include "pd_api.h"

Screen *NewScreen(PlaydateAPI *pd) {
    LCDBitmap *lcdbm = pd->graphics->newBitmap(SCREEN_WIDTH, SCREEN_HEIGTH, 0);
    uint8_t *bm = NULL;
    pd->graphics->getBitmapData(lcdbm, NULL, NULL, NULL, NULL, &bm);
    Screen *screen = (Screen *)malloc(sizeof(Screen));
    screen->lcdbm = lcdbm;
    screen->bm = bm;
    screen->pd = pd;
    return screen;
}

void DrawScreen(Screen *s) {
    s->pd->graphics->drawBitmap(s->lcdbm, 0, 0, kBitmapUnflipped);
}

void FillScreen(Screen *s, int enabled) {
    int val = enabled ? 0xFF : 0;
    memset(s->bm, val, SCREEN_HEIGTH * SCREEN_ROWS);
}

void SetSquare(Screen *s, int x, int y, int size, int enabled) {
    for (int dy = -size; dy <= size; dy++) {
        for (int dx = -size; dx <= size; dx++) {
            int X = x + dx;
            int Y = y + dy;
            if (X < 0 || X >= SCREEN_WIDTH || Y < 0 || Y >= SCREEN_HEIGTH) {
                continue;
            }

            SetPixel(s, X, Y, enabled);
        }
    }
}

void DrawLine(Screen *s, float x1, float y1, float x2, float y2, int enabled) {
    float xd = x2 - x1;
    float yd = y2 - y1;
    if (yd == 0 && xd == 0) {
        return;
    }

    float len = sqrtf(xd * xd + yd * yd);
    xd /= len;
    yd /= len;
    float xc = x1;
    float yc = y1;

    float xmin = x1 < x2 ? x1 : x2;
    float ymin = y1 < y2 ? y1 : y2;
    float xmax = x1 < x2 ? x2 : x1;
    float ymax = y1 < y2 ? y2 : y1;

    int inScreen = 0;
    for (int i = 0; i < 1000; i++) {
        if (xmin > xc || xmax < xc || ymin > yc || ymax < yc) {
            break;
        }

        int px = roundf(xc);
        int py = roundf(yc);

        if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGTH) {
            SetPixel(s, px, py, enabled);
            inScreen = 1;
        } else {
            if (inScreen) {
                break;
            }
        }

        xc += xd;
        yc += yd;
    }
}