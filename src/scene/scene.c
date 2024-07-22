#include "scene.h"
#include <stdlib.h>
#include <string.h>

Scene *NewEmptyScene() {
    Scene *s = (Scene *)malloc(sizeof(Scene));
    s->objectsLen = 0;
    s->objectsCap = 0;
    s->objects = NULL;
    return s;
}

void AddObjectToScene(Scene *s, Object *o) {
    if (s->objectsLen == s->objectsCap) {
        uint16_t newCap = s->objectsCap + 10;
        Object **newObjects = (Object **)malloc(sizeof(Object *) * newCap);
        if (s->objects != NULL) {
            memcpy(newObjects, s->objects, sizeof(Object *) * s->objectsLen);
            free(s->objects);
        }
        s->objects = newObjects;
        s->objects[s->objectsLen++] = o;
        s->objectsCap = newCap;
    } else {
        s->objects[s->objectsLen++] = o;
    }
}
