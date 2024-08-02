#include "../camera/camera.h"
#include "../scene/scene.h"
#include "../screen/screen.h"
#include "../util/consts.h"
#include "../rotation/quaternion.h"

typedef struct ScreenPosition {
    float x;
    float y;
    int valid;
} ScreenPosition;

ScreenPosition *newEmptyScreenPosition(void) {
    return malloc(sizeof(ScreenPosition));
}

ScreenPosition *newInvalidScreenPosition(void) {
    ScreenPosition *p = newEmptyScreenPosition();
    p->valid = 0;
    return p;
}

ScreenPosition *newScreenPosition(float x, float y) {
    ScreenPosition *p = newEmptyScreenPosition();
    p->valid = 1;
    p->x = x;
    p->y = y;
    return p;
}

ScreenPosition *newCameraProjection(Vector3 *pos, Camera *cam) {
    float projectionDistance =
        GetLineDistance(pos, cam->pos, GetCameraForward(cam));

    if (projectionDistance < 0) {
        return newInvalidScreenPosition();
    }

    Vector3 *projectionCenterV =
        NewMulV3(GetCameraForward(cam), projectionDistance);
    Vector3 *projectionCenter = NewSumV3(cam->pos, projectionCenterV);
    Vector3 *projectionCenterOffset = NewDiffV3(pos, projectionCenter);

    float projectionCenterDistance =
        sqrtf(GetV3SquaredLength(projectionCenterOffset));
    float rightCos = GetV3Dot(projectionCenterOffset, GetCameraRight(cam)) /
                     sqrtf(GetV3SquaredLength(projectionCenterOffset));
    int isDown = GetV3Dot(projectionCenterOffset, GetCameraUp(cam)) < 0;
    float angle = acosf(rightCos);
    float halfScreen = tanf(cam->xFov / 2) * projectionDistance;

    float projectionCenterDistancePixels =
        projectionCenterDistance / halfScreen * (float)SCREEN_WIDTH / 2.0f;
    float xf = projectionCenterDistancePixels * rightCos;
    float yf = projectionCenterDistancePixels * sinf(angle);
    if (isDown) {
        yf = -yf;
    }

    free(projectionCenterOffset);
    free(projectionCenter);
    free(projectionCenterV);

    return newScreenPosition(xf + SCREEN_WIDTH / 2, yf + SCREEN_HEIGTH / 2);
}

void RenderVerticies(Scene *s, Camera *c, Screen *target) {
    for (int i = 0; i < s->objectsLen; i++) {
        Object *o = s->objects[i];

        uint16_t vCount = GetObjectVerticiesCount(o);
        Vector3* verticies = GetObjectVerticies(o);
        for (int j = 0; j < vCount; j++) {
            Vector3 *vpos = NewSumV3(o->pos, &verticies[j]);
            ScreenPosition *projection = newCameraProjection(vpos, c);
            if (projection->valid) {
                SetSquare(target, projection->x, projection->y, 2, 1);
            }

            free(vpos);
            free(projection);
        }
    }
}

void RenderMesh(Scene *s, Camera *c, Screen *target) {
    for (int i = 0; i < s->objectsLen; i++) {
        Object *o = s->objects[i];

        Vector3* verticies = GetObjectVerticies(o);
        for (int j = 0; j < o->geometry->fCount; j++) {
            Vector3 *va = &verticies[o->geometry->f[j].a];
            Vector3 *vposa = NewSumV3(o->pos, va);
            ScreenPosition *projectiona = newCameraProjection(vposa, c);

            Vector3 *vb = &verticies[o->geometry->f[j].b];
            Vector3 *vposb = NewSumV3(o->pos, vb);
            ScreenPosition *projectionb = newCameraProjection(vposb, c);

            Vector3 *vc = &verticies[o->geometry->f[j].c];
            Vector3 *vposc = NewSumV3(o->pos, vc);
            ScreenPosition *projectionc = newCameraProjection(vposc, c);

            if (projectiona->valid && projectionb->valid) {
                DrawLine(target, projectiona->x, projectiona->y, projectionb->x,
                         projectionb->y, 1);
            }
            if (projectionc->valid && projectionb->valid) {
                DrawLine(target, projectionc->x, projectionc->y, projectionb->x,
                         projectionb->y, 1);
            }
            if (projectiona->valid && projectionc->valid) {
                DrawLine(target, projectiona->x, projectiona->y, projectionc->x,
                         projectionc->y, 1);
            }

            free(vposa);
            free(projectiona);
            free(vposb);
            free(projectionb);
            free(vposc);
            free(projectionc);
        }
    }
}