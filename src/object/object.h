#ifndef __OBJECT

#include "../geometry/geometry.h"
#include "../vector/vector3.h"
#include "../rotation/quaternion.h"

typedef struct Object {
    Vector3 *pos;
    Quaternion *rot;
    Geometry *geometry;
} Object;

Object *NewCubeObject(void);

#define __OBJECT
#endif
