#ifndef __GEOMETRY

#include "../vector/vector3.h"
#include <stdint.h>

typedef struct GeometryFace {
    uint16_t a, b, c;
} GeometryFace;

typedef struct Geometry {
    uint16_t vCount;
    Vector3 *v;
    uint16_t fCount;
    GeometryFace *f;
} Geometry;

extern Geometry CubeGeometry;
extern Geometry EmptyGeometry;

void InitDefaultGeometry(void);

#define __GEOMETRY
#endif
