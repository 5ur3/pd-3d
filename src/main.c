#include <stdio.h>
#include <stdlib.h>
#define TARGET_EXTENSION 1
#include "pd_api.h"

#include "camera/camera.h"
#include "geometry/geometry.h"
#include "object/object.h"
#include "renderer/renderer.h"
#include "scene/scene.h"
#include "screen/screen.h"
#include "util/consts.h"

static int update(void *userdata);

PlaydateAPI *pd;

Screen *screen;
Scene *scene;
Camera *camera;
Object *object;
float camX = 0;
float camZ = 0;
Quaternion *q;

int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg) {
    if (event == kEventInit) {
        object = NewEmptyObject();
        Geometry *g = LoadObj(playdate, "models/cat_lpoly.obj");
        object->geometry = g;
        object->scale = 0.01f;
        SetQuaternionValues(object->rot, 0, 1, 0, PI);
        screen = NewScreen(playdate);
        scene = NewEmptyScene();
        SetV3Values(object->pos, 0, -0.5f, 2);
        AddObjectToScene(scene, object);
        camera = NewDefaultCamera();

        Vector3 qv = {1, 2, 3};
        q = NewQuaternion(&qv, PI / 50);

        playdate->system->setUpdateCallback(update, NULL);
        pd = playdate;
    }

    return 0;
}

#define SPEED 0.2f
#define R 3.0f
// float scale = 1;
static int update(void *userdata) {
    PDButtons current;
    pd->system->getButtonState(&current, NULL, NULL);
    if (current & kButtonLeft) {
        Quaternion *left = NewZeroQuaternion();
        SetQuaternionValues(left, 0, -1, 0, R / 180.f * R);
        MulQuaternion(camera->rot, left);
        free(left);
    }
    if (current & kButtonRight) {
        Quaternion *right = NewZeroQuaternion();
        SetQuaternionValues(right, 0, 1, 0, R / 180.f * R);
        MulQuaternion(camera->rot, right);
        free(right);
    }
    if (current & kButtonUp) {
        camZ += SPEED * GetCameraForward(camera)->z;
        camX += SPEED * GetCameraForward(camera)->x;
    }
    if (current & kButtonDown) {
        camZ -= SPEED * GetCameraForward(camera)->z;
        camX -= SPEED * GetCameraForward(camera)->x;
    }

    camera->pos->x = camX;
    camera->pos->z = camZ;

    camera->pos->y += pd->system->getCrankChange() / 180;

    FillScreen(screen, 0);
    Render(scene, camera, screen);

    DrawScreen(screen);

    pd->system->drawFPS(0, 0);
    return 1;
}
