#include <stdio.h>
#include <stdlib.h>
#define TARGET_EXTENSION 1
#include "pd_api.h"

#include "camera/camera.h"
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
Object *cube;
float camX = 0;
float camZ = 0;
Quaternion *q;

int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg) {
    if (event == kEventInit) {
        screen = NewScreen(playdate);
        scene = NewEmptyScene();
        cube = NewCubeObject();
        SetV3Values(cube->pos, 0, 0, 5);
        AddObjectToScene(scene, cube);
        camera = NewDefaultCamera();

        Vector3 qv = {1, 2, 3};
        q = NewQuaternion(&qv, PI / 50);

        playdate->system->setUpdateCallback(update, NULL);
        pd = playdate;
    }

    return 0;
}

#define SPEED 0.05f
#define R 2
// float scale = 1;
static int update(void *userdata) {
    PDButtons current;
    pd->system->getButtonState(&current, NULL, NULL);
    if (current & kButtonLeft) {
        camX -= SPEED;
    }
    if (current & kButtonRight) {
        camX += SPEED;
    }
    if (current & kButtonUp) {
        camZ += SPEED;
    }
    if (current & kButtonDown) {
        camZ -= SPEED;
    }

    camera->pos->x = camX;
    camera->pos->z = camZ;
    SetQuaternionValues(camera->rot, 1, 0, 0,
                        pd->system->getCrankAngle() / 180.0f * PI);

    MulQuaternion(cube->rot, q);
    FillScreen(screen, 0);
    Render(scene, camera, screen);

    // #define N 8
    //     float triangles[N][6] = {{-10, -10, 0, 0, 0, -10}, {-10, 0, 0, 0,
    //     -10, 10},
    //                              {0, 0, 0, 10, 10, 10},    {0, 0, 10, 0, 10,
    //                              -10},
    //                              {-5, -5, 0, 0, 0, -5},    {-5, 0, 0, 0, -5,
    //                              5}, {0, 0, 0, 5, 5, 5},       {0, 0, 5, 0,
    //                              5, -5}};
    //     scale += SPEED;
    //     for (int i = 0; i < N; i++) {
    //         DrawDitheredTriangle(screen, triangles[i][0] * scale +
    //         SCREEN_WIDTH / 2,
    //                              triangles[i][1] * scale + SCREEN_HEIGTH / 2,
    //                              triangles[i][2] * scale + SCREEN_WIDTH / 2,
    //                              triangles[i][3] * scale + SCREEN_HEIGTH / 2,
    //                              triangles[i][4] * scale + SCREEN_WIDTH / 2,
    //                              triangles[i][5] * scale + SCREEN_HEIGTH / 2,
    //                              screen->ditherMaxBrightness * 1.0 / N * (i +
    //                              1));
    //     }
    DrawScreen(screen);

    pd->system->drawFPS(0, 0);
    return 1;
}
