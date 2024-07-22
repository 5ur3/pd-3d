#include "object.h"
#include "../geometry/geometry.h"
#include <stdlib.h>

Object *NewCubeObject() {
    Object *o = (Object *)malloc(sizeof(Object));
    o->geometry = NewCubeGeometry();
    o->pos = NewZeroV3();
    o->rot = NewZeroV3();
    return o;
}