#include "object.h"
#include "../geometry/geometry.h"
#include "../vector/vector3.h"
#include <stdlib.h>

Object *NewEmptyObject() {
    Object *o = (Object *)malloc(sizeof(Object));
    o->geometry = &EmptyGeometry;
    o->pos = NewZeroV3();
    o->rot = NewZeroQuaternion();
    o->scale = 1;
    o->cache.verticies = NULL;
    return o;
}

Object *NewCubeObject() {
    InitDefaultGeometry();
    Object *o = NewEmptyObject();
    o->geometry = &CubeGeometry;
    return o;
}

uint16_t asUint16(float f) {
    union {
        uint16_t i;
        float f;
    } u;
    u.f = f;
    return u.i;
}

uint16_t rotateUint16(uint16_t n, int rotate) {
    return (n << rotate) | (n >> (16 - rotate));
}

uint16_t getObjectHash(Object *o) {
    return asUint16(o->rot->x) ^ rotateUint16(asUint16(o->rot->y), 3) ^
           rotateUint16(asUint16(o->rot->z), 6) ^
           rotateUint16(asUint16(o->rot->w), 9) ^
           rotateUint16(asUint16(o->scale), 12);
}

short shouldRecalculateVerticies(Object *o) {
    return o->cache.verticies == NULL || getObjectHash(o) != o->cache.hash;
}

void recalculateCachedVerticies(Object *o) {
    if (o->cache.verticies == NULL) {
        o->cache.verticies =
            (Vector3 *)malloc(sizeof(Vector3) * o->geometry->vCount);
    }

    for (int i = 0; i < o->geometry->vCount; i++) {
        Vector3 *rotated = NewRotatedV3(&o->geometry->v[i], o->rot);
        MulV3(rotated, o->scale);
        MoveAndDeleteV3(&o->cache.verticies[i], rotated);
    }

    o->cache.hash = getObjectHash(o);
}

Vector3 *GetObjectVerticies(Object *o) {
    if (shouldRecalculateVerticies(o)) {
        recalculateCachedVerticies(o);
    }
    return o->cache.verticies;
}

uint16_t GetObjectVerticiesCount(Object *o) { return o->geometry->vCount; }
