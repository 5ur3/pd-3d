#include "geometry.h"
#include <stdlib.h>

// Verticies:
// 0: 0 0 0
// 1: 1 0 0
// 2: 0 1 0
// 3: 1 1 0
// 4: 0 0 1
// 5: 1 0 1
// 6: 0 1 1
// 7: 1 1 1
// Faces:
// 0, 1, 2, 3 :: 0, 1, 2 + 1, 2, 3
// 4, 5, 6, 7 :: 4, 5, 6 + 5, 6, 7
// 0, 1, 4, 5 :: 0, 1, 4 + 1, 4, 5
// 2, 3, 6, 7 :: 2, 3, 6 + 3, 6, 7
// 0, 2, 4, 6 :: 0, 2, 4 + 2, 4, 6
// 1, 3, 5, 7 :: 1, 3, 5 + 3, 5, 7
Geometry *NewCubeGeometry() {
    Geometry *g = (Geometry *)malloc(sizeof(Geometry));
    g->vCount = 8;
    g->v = (Vector3 *)malloc(sizeof(Vector3) * 8);
    for (int i = 0; i < 8; i++) {
        g->v[i].x = i & 1;
        g->v[i].y = (i & 2) >> 1;
        g->v[i].z = (i & 4) >> 2;

        g->v[i].x -= 0.5f;
        g->v[i].y -= 0.5f;
        g->v[i].z -= 0.5f;
    }

    g->fCount = 12;
    g->f = (GeometryFace *)malloc(sizeof(GeometryFace) * 12);
    SetGeometryFace(&g->f[0], 0, 1, 2);
    SetGeometryFace(&g->f[1], 1, 2, 3);
    SetGeometryFace(&g->f[2], 4, 5, 6);
    SetGeometryFace(&g->f[3], 5, 6, 7);
    SetGeometryFace(&g->f[4], 0, 1, 4);
    SetGeometryFace(&g->f[5], 1, 4, 5);
    SetGeometryFace(&g->f[6], 2, 3, 6);
    SetGeometryFace(&g->f[7], 3, 6, 7);
    SetGeometryFace(&g->f[8], 0, 2, 4);
    SetGeometryFace(&g->f[9], 2, 4, 6);
    SetGeometryFace(&g->f[10], 1, 3, 5);
    SetGeometryFace(&g->f[11], 3, 5, 7);

    return g;
}

void SetGeometryFace(GeometryFace *face, uint16_t a, uint16_t b, uint16_t c) {
    face->a = a;
    face->b = b;
    face->c = c;
}