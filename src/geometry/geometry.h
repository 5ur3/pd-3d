#ifndef __GEOMETRY

#include "../vector/vector3.h"
#define TARGET_EXTENSION 1
#include "pd_api.h"
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

Geometry *LoadObj(PlaydateAPI *pd, const char *path);
void InitDefaultGeometry(void);

#define __GEOMETRY
#endif
