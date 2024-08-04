#define TARGET_EXTENSION 1
#include "screen.h"
#include "pd_api.h"
#include "point.h"

void setSquaredBayerDither(Screen *s, uint16_t rank);

Screen *NewScreen(PlaydateAPI *pd) {
    LCDBitmap *lcdbm = pd->graphics->newBitmap(SCREEN_WIDTH, SCREEN_HEIGTH, 0);
    uint8_t *bm = NULL;
    pd->graphics->getBitmapData(lcdbm, NULL, NULL, NULL, NULL, &bm);
    Screen *screen = (Screen *)malloc(sizeof(Screen));
    screen->lcdbm = lcdbm;
    screen->bm = bm;
    screen->pd = pd;
    setSquaredBayerDither(screen, 3);
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

points *getTrianglePoints(int x1, int y1, int x2, int y2, int x3, int y3);

void DrawTriangle(Screen *s, int x1, int y1, int x2, int y2, int x3, int y3,
                  int enabled) {
    points *ps = getTrianglePoints(x1, y1, x2, y2, x3, y3);
    int len = ps->len;
    for (int i = 0; i < len; i++) {
        SetPixel(s, ps->points[i].x, ps->points[i].y, enabled);
    }
    free(ps->points);
    free(ps);
}
