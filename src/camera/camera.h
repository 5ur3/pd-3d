#ifndef __CAMERA
#include "../rotation/quaternion.h"
#include "../vector/vector3.h"

typedef struct CameraVectors {
    Vector3 *forward;
    Vector3 *up;
    Vector3 *right;
    Quaternion *updatedForRot;
} CameraVectors;

typedef struct Camera {
    Vector3 *pos;
    Quaternion *rot;
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