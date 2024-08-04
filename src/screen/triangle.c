#include "point.h"
#include "screen.h"

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

    if (y1 < 0) {
        curx1 += invslope1 * -y1;
        curx2 += invslope2 * -y1;
        y1 = 0;
    }

    for (int scanlineY = y1; scanlineY <= y2; scanlineY++) {
        if (scanlineY >= SCREEN_HEIGTH) {
            break;
        }
        addPointsOfHorizontalLine(ps, curx1, curx2, scanlineY);
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

    if (y3 >= SCREEN_HEIGTH) {
        curx1 -= invslope1 * (y3 - SCREEN_HEIGTH + 1);
        curx2 -= invslope2 * (y3 - SCREEN_HEIGTH + 1);
        y3 = SCREEN_HEIGTH - 1;
    }

    for (int scanlineY = y3; scanlineY >= y1; scanlineY--) {
        if (scanlineY < 0) {
            break;
        }
        addPointsOfHorizontalLine(ps, curx1, curx2, scanlineY);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

points *getTrianglePoints(int x1, int y1, int x2, int y2, int x3, int y3) {
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
    points *ps = (points *)malloc(sizeof(points));
    int w = max3(x1, x2, x3) - min3(x1, x2, x3);
    if (w > SCREEN_WIDTH) {
        w = SCREEN_WIDTH;
    }
    int h = max3(y1, y2, y3) - min3(y1, y2, y3);
    if (h > SCREEN_HEIGTH) {
        h = SCREEN_HEIGTH;
    }
    uint32_t boundingRectArea = (w + 1) * (h + 1);
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
