#ifndef __GEOMETRY

#include "../vector/vector3.h"
#include <stdint.h>

typedef struct GeometryFace {
    uint16_t a, b, c;
} GeometryFace;

void SetGeometryFace(GeometryFace *face, uint16_t a, uint16_t b, uint16_t c);

typedef struct Geometry {
    uint16_t vCount;
    Vector3 *v;
    uint16_t fCount;
    GeometryFace *f;
} Geometry;

Geometry *NewCubeGeometry(void);

#define __GEOMETRY
#endif
