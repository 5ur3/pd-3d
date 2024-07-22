#ifndef __SCENE
#include "../object/object.h"

typedef struct Scene {
    uint16_t objectsLen;
    uint16_t objectsCap;
    Object **objects;
} Scene;

Scene *NewEmptyScene(void);
void AddObjectToScene(Scene *s, Object *o);

#define __SCENE
#endif