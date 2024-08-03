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

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

int min3(int a, int b, int c) { return min(min(a, b), c); }

int max3(int a, int b, int c) { return max(max(a, b), c); }

// https://stackoverflow.com/questions/8862136/is-there-a-built-in-swap-function-in-c
#define swap(a, b)                                                             \
    do {                                                                       \
        int tmp = a;                                                           \
        a = b;                                                                 \
        b = tmp;                                                               \
    } while (0)

void addPointsOfHorizontalLine(points *ps, int x1, int x2, int y) {
    if (x1 > x2) {
        swap(x1, x2);
    }

    if (x1 < 0) {
        x1 = 0;
    }
    if (x2 >= SCREEN_WIDTH) {
        x2 = SCREEN_WIDTH - 1;
    }

    for (int i = x1; i <= x2; i++) {
        ps->points[ps->len].x = i;
        ps->points[ps->len++].y = y;
    }
}

void addPointsOfBottomFlatTriangle(points *ps, float x1, float y1, float x2,
                                   float y2, float x3, float y3) {
    float invslope1 = (x2 - x1) / (y2 - y1);
    float invslope2 = (x3 - x1) / (y3 - y1);

    float curx1 = x1;
    float curx2 = x1;

    for (int scanlineY = y1; scanlineY <= y2; scanlineY++) {
        if (scanlineY >= 0 && scanlineY < SCREEN_HEIGTH) {
            addPointsOfHorizontalLine(ps, curx1, curx2, scanlineY);
        }
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void addPointsOfTopFlatTriangle(points *ps, float x1, float y1, float x2,
                                float y2, float x3, float y3) {
    float invslope1 = (x3 - x1) / (y3 - y1);
    float invslope2 = (x3 - x2) / (y3 - y2);

    float curx1 = x3;
    float curx2 = x3;

    for (int scanlineY = y3; scanlineY >= y1; scanlineY--) {
        if (scanlineY >= 0 && scanlineY < SCREEN_HEIGTH) {
            addPointsOfHorizontalLine(ps, curx1, curx2, scanlineY);
        }
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

points *getTrianglePoints(int x1, int y1, int x2, int y2, int x3, int y3) {
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
    points *ps = (points *)malloc(sizeof(points));
    uint32_t boundingRectArea = (max3(x1, x2, x3) - min3(x1, x2, x3) + 1) *
                                (max3(y1, y2, y3) - min3(y1, y2, y3) + 1);
    ps->points = (point *)malloc(sizeof(point) * boundingRectArea);
    ps->len = 0;

    if (y1 > y2) {
        swap(y1, y2);
        swap(x1, x2);
    }
    if (y1 > y3) {
        swap(y1, y3);
        swap(x1, x3);
    }
    if (y3 < y2) {
        swap(y2, y3);
        swap(x2, x3);
    }

    if (y2 == y3) {
        addPointsOfBottomFlatTriangle(ps, x1, y1, x2, y2, x3, y3);
    } else if (y1 == y2) {
        addPointsOfTopFlatTriangle(ps, x1, y1, x2, y2, x3, y3);
    } else {
        int y4 = y2;
        // (int)(vt1.x + ((float)(vt2.y - vt1.y) / (float)(vt3.y - vt1.y)) *
        // (vt3.x - vt1.x))
        int x4 = (int)(x1 + ((float)(y2 - y1) / (float)(y3 - y1)) * (x3 - x1));

        addPointsOfBottomFlatTriangle(ps, x1, y1, x2, y2, x4, y4);
        addPointsOfTopFlatTriangle(ps, x2, y2, x4, y4, x3, y3);
    }

    return ps;
}

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
