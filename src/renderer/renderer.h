#ifndef __RENDERER
#include "../camera/camera.h"
#include "../scene/scene.h"
#include "../screen/screen.h"

void RenderVerticies(Scene *s, Camera *c, Screen *target);
void RenderMesh(Scene *s, Camera *c, Screen *target);
void Render(Scene *s, Camera *c, Screen *target);

#define __RENDERER
#endif