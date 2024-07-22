#include <stdio.h>
#include <stdlib.h>
#define TARGET_EXTENSION 1
#include "pd_api.h"

#include "camera/camera.h"
#include "object/object.h"
#include "renderer/renderer.h"
#include "scene/scene.h"
#include "screen/screen.h"

static int update(void *userdata);

PlaydateAPI *pd;

Screen *screen;
Scene *scene;
Camera *camera;
Object *cube;
float camX = 0;
float camZ = 0;

int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg) {
    if (event == kEventInit) {
        screen = NewScreen(playdate);
        scene = NewEmptyScene();
        cube = NewCubeObject();
        SetV3Values(cube->pos, 0, 0, 5);
        AddObjectToScene(scene, cube);
        camera = NewDefaultCamera();

        playdate->system->setUpdateCallback(update, NULL);
        pd = playdate;
    }

    return 0;
}

#define SPEED 0.05f
#define R 2
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
    FillScreen(screen, 0);
    RenderVerticies(scene, camera, screen);
    RenderMesh(scene, camera, screen);
    DrawScreen(screen);

    pd->system->drawFPS(0, 0);
    return 1;
}
