#ifndef __OBJECT

#include "../geometry/geometry.h"
#include "../vector/vector3.h"

typedef struct Object {
    Vector3 *pos;
    Vector3 *rot;
    Geometry *geometry;
} Object;

Object *NewCubeObject(void);

#define __OBJECT
#endif
