#include "camera.h"
#include "../util/consts.h"
#include "stdlib.h"

void invalidateVectors(Camera *c) {
    if (c->cVectors.forward == NULL ||
        !EqualQuaternion(c->rot, c->cVectors.updatedForRot)) {
        free(c->cVectors.updatedForRot);
        if (c->cVectors.forward == NULL) {
            c->cVectors.forward = NewZeroV3();
            c->cVectors.right = NewZeroV3();
            c->cVectors.up = NewZeroV3();
        }
        c->cVectors.updatedForRot = NewCopyQuaternion(c->rot);

        Vector3 f = {0, 0, 1};
        Vector3 r = {1, 0, 0};
        Vector3 u = {0, 1, 0};
        MoveAndDeleteV3(c->cVectors.forward, NewRotatedV3(&f, c->rot));
        MoveAndDeleteV3(c->cVectors.right, NewRotatedV3(&r, c->rot));
        MoveAndDeleteV3(c->cVectors.up, NewRotatedV3(&u, c->rot));
    }
}

Camera *NewCamera(float xFov, float depth) {
    Camera *c = (Camera *)malloc(sizeof(Camera));
    c->pos = NewZeroV3();
    c->rot = NewZeroQuaternion();
    c->xFov = xFov;
    c->depth = depth;
    c->cVectors.forward = NULL;
    c->cVectors.up = NULL;
    c->cVectors.right = NULL;
    c->cVectors.updatedForRot = NewZeroQuaternion();
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
