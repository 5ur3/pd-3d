#ifndef __CAMERA
#include "../vector/vector3.h"

typedef struct CameraVectors {
    Vector3 *forward;
    Vector3 *up;
    Vector3 *right;
    Vector3 *updatedForRot;
} CameraVectors;

typedef struct Camera {
    Vector3 *pos;
    Vector3 *rot;
    float xFov;
    float depth;
    CameraVectors cVectors;
} Camera;

Camera *NewCamera(float xFov, float depth);
Camera *NewDefaultCamera(void);
Vector3 *GetCameraForward(Camera *c);
Vector3 *GetCameraUp(Camera *c);
Vector3 *GetCameraRight(Camera *c);

#define __CAMERA
#endif