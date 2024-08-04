#include "point.h"
#include "screen.h"
#include <stdlib.h>

void deleteSquaredBayerDither(uint16_t **buffer, uint16_t rank) {
    uint16_t side = 1 << rank;
    for (int i = 0; i < side; i++) {
        free(buffer[i]);
    }
    free(buffer);
}

uint16_t **makeSquaredBayerDither(uint16_t rank) {
    uint16_t side = 1 << rank;
    uint16_t **buffer = (uint16_t **)malloc(sizeof(uint16_t *) * side);
    for (int i = 0; i < side; i++) {
        buffer[i] = (uint16_t *)malloc(sizeof(uint16_t) * side);
    }

    if (rank == 0) {
        buffer[0][0] = 0;
        return buffer;
    }

    uint16_t **prev = makeSquaredBayerDither(rank - 1);

    for (uint16_t px = 0; px < side; px += side / 2) {
        for (uint16_t py = 0; py < side; py += side / 2) {
            int c = 0;
            if (px && py) {
                c = 1;
            }
            if (px && !py) {
                c = 2;
            }
            if (!px && py) {
                c = 3;
            }

            for (uint16_t x = 0; x < side / 2; x++) {
                for (uint16_t y = 0; y < side / 2; y++) {
                    buffer[px + x][py + y] = 4 * prev[x][y] + c;
                }
            }
        }
    }

    deleteSquaredBayerDither(prev, rank - 1);

    return buffer;
}

void setSquaredBayerDither(Screen *s, uint16_t rank) {
    uint16_t **buffer = makeSquaredBayerDither(rank);
    uint16_t side = 1 << rank;
    s->ditherMaxBrightness = 1 << (rank * 2);
    s->ditherMap =
        (uint16_t *)malloc(sizeof(uint16_t) * s->ditherMaxBrightness);
    for (short y = 0; y < side; y++) {
        for (short x = 0; x < side; x++) {
            s->ditherMap[y * side + x] = buffer[y][x];
        }
    }
    s->ditherMapSize = side;
    deleteSquaredBayerDither(buffer, rank);
}

void SetDitheredPixel(Screen *s, short x, short y, uint16_t brighness) {
    if (brighness >
        s->ditherMap[(y * s->ditherMapSize + x) % s->ditherMaxBrightness]) {
        SetPixel(s, x, y, 1);
    } else {
        SetPixel(s, x, y, 0);
    }
}

points *getTrianglePoints(int x1, int y1, int x2, int y2, int x3, int y3);

void DrawDitheredTriangle(Screen *s, int x1, int y1, int x2, int y2, int x3,
                          int y3, uint16_t brighness) {
    points *ps = getTrianglePoints(x1, y1, x2, y2, x3, y3);
    int len = ps->len;
    for (int i = 0; i < len; i++) {
        SetDitheredPixel(s, ps->points[i].x, ps->points[i].y, brighness);
    }
    free(ps->points);
    free(ps);
}
