#ifndef __OBJECT

#include "../geometry/geometry.h"
#include "../rotation/quaternion.h"
#include "../vector/vector3.h"

typedef struct objectCache {
    Vector3 *verticies;
    uint16_t hash;
} objectCache;

typedef struct Object {
    Vector3 *pos;
    Quaternion *rot;
    Geometry *geometry;
    objectCache cache;
    float scale;
} Object;

Object *NewEmptyObject(void);
Object *NewCubeObject(void);
Vector3 *GetObjectVerticies(Object *o);
uint16_t GetObjectVerticiesCount(Object *o);

#define __OBJECT
#endif
