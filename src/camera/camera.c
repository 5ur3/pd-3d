#include "camera.h"
#include "../util/consts.h"
#include "stdlib.h"

void invalidateVectors(Camera *c) {
    if (c->cVectors.forward == NULL ||
        !EqualV3(c->rot, c->cVectors.updatedForRot)) {
        free(c->cVectors.updatedForRot);
        if (c->cVectors.forward != NULL) {
            free(c->cVectors.forward);
            free(c->cVectors.right);
            free(c->cVectors.up);
        }
        // TODO: propertly recalculate
        c->cVectors.updatedForRot = NewCopyV3(c->rot);
        c->cVectors.forward = NewV3(0, 0, 1);
        c->cVectors.right = NewV3(1, 0, 0);
        c->cVectors.up = NewV3(0, 1, 0);
    }
}

Camera *NewCamera(float xFov, float depth) {
    Camera *c = (Camera *)malloc(sizeof(Camera));
    c->pos = NewZeroV3();
    c->rot = NewZeroV3();
    c->xFov = xFov;
    c->depth = depth;
    c->cVectors.forward = NULL;
    c->cVectors.up = NULL;
    c->cVectors.right = NULL;
    c->cVectors.updatedForRot = NewZeroV3();
    invalidateVectors(c);
    return c;
}

Camera *NewDefaultCamera() { return NewCamera(60.0f / 180.0f * PI, 100.0f); }

Vector3 *GetCameraForward(Camera *c) {
    invalidateVectors(c);
    return c->cVectors.forward;
}
Vector3 *GetCameraUp(Camera *c) {
    invalidateVectors(c);
    return c->cVectors.up;
}
Vector3 *GetCameraRight(Camera *c) {
    invalidateVectors(c);
    return c->cVectors.right;
}
