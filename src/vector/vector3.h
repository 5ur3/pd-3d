#ifndef __VECTOR3

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

Vector3 *NewV3(float x, float y, float z);
Vector3 *NewZeroV3(void);
Vector3 *NewNegativeV3(Vector3 *vector);
Vector3 *NewSumV3(Vector3 *a, Vector3 *b);
Vector3 *NewDiffV3(Vector3 *a, Vector3 *b);
Vector3 *NewMulV3(Vector3 *v, float s);
Vector3 *NewCopyV3(Vector3 *source);
Vector3 *NewNormalizedV3(Vector3 *vector);
Vector3 *NewCrossProductV3(Vector3 *a, Vector3 *b);
float GetV3Dot(Vector3 *a, Vector3 *b);
float GetV3SquaredLength(Vector3 *vector);
float GetV3SquaredDistance(Vector3 *a, Vector3 *b);
void SetV3Values(Vector3 *vector, float x, float y, float z);
void SetV3(Vector3 *target, Vector3 *source);
void MulV3(Vector3 *v, float s);
void AddV3(Vector3 *v, Vector3 *add);
void NormalizeV3(Vector3 *v);
float GetLineDistance(Vector3 *point, Vector3 *from, Vector3 *lineNorm);
int EqualV3(Vector3 *a, Vector3 *b);
void MoveAndDeleteV3(Vector3 *target, Vector3 *source);

#define __VECTOR3
#endif
