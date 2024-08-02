#include "../vector/vector3.h"

#ifndef __QUATERNION

typedef struct Quaternion {
    float x;
    float y;
    float z;
    float w;
} Quaternion;

Quaternion* NewZeroQuaternion(void);
Quaternion *NewQuaternion(Vector3 *v, float angle);
Quaternion *NewCopyQuaternion(Quaternion *q);
Quaternion *NewInvertedQuaternion(Quaternion *q);
Quaternion *NewMulV3Quaternion(Quaternion *q, Vector3* v);
Quaternion *NewMulQuaternion(Quaternion *a, Quaternion* b);
Vector3* NewRotatedV3(Vector3* v, Quaternion*q);
void InvertQuaternion(Quaternion *q);
void RotateV3(Vector3* v, Quaternion* q);
void MulV3Quaternion(Quaternion *q, Vector3* v);
void MulQuaternion(Quaternion *a, Quaternion* b);

#define __QUATERNION
#endif