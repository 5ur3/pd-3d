#include "vector3.h"
#include <math.h>
#include <stdlib.h>

Vector3 *NewV3(float x, float y, float z) {
    Vector3 *v = (Vector3 *)malloc(sizeof(Vector3));
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

Vector3 *NewZeroV3() { return NewV3(0, 0, 0); }

Vector3 *NewNegativeV3(Vector3 *v) { return NewV3(-v->x, -v->y, -v->z); }

Vector3 *NewCopyV3(Vector3 *v) { return NewV3(v->x, v->y, v->z); }

Vector3 *NewSumV3(Vector3 *a, Vector3 *b) {
    return NewV3(a->x + b->x, a->y + b->y, a->z + b->z);
}

Vector3 *NewDiffV3(Vector3 *a, Vector3 *b) {
    return NewV3(a->x - b->x, a->y - b->y, a->z - b->z);
}

Vector3 *NewMulV3(Vector3 *v, float s) {
    Vector3 *c = NewCopyV3(v);
    MulV3(c, s);
    return c;
}

float GetV3Dot(Vector3 *a, Vector3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float GetV3SquaredLength(Vector3 *v) { return GetV3Dot(v, v); }

float GetV3SquaredDistance(Vector3 *a, Vector3 *b) {
    Vector3 *diff = NewDiffV3(a, b);
    float squaredLength = GetV3SquaredLength(diff);
    free(diff);
    return squaredLength;
}

void SetV3Values(Vector3 *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void SetV3(Vector3 *target, Vector3 *source) {
    target->x = source->x;
    target->y = source->y;
    target->z = source->z;
}

void MulV3(Vector3 *v, float s) {
    v->x *= s;
    v->y *= s;
    v->z *= s;
}

void NormalizeV3(Vector3 *v) {
    float len = sqrtf(GetV3SquaredLength(v));
    MulV3(v, 1.0f / len);
}

int EqualV3(Vector3 *a, Vector3 *b) {
    return a->x == b->x && a->y == b->y && a->z == b->z;
}

// https://stackoverflow.com/questions/52791641/distance-from-point-to-line-3d-formula
Vector3 *NewClosestToPointOnALine(Vector3 *point, Vector3 *a, Vector3 *b) {
    Vector3 *D = NewDiffV3(b, a);
    NormalizeV3(D);
    Vector3 *AP = NewDiffV3(point, a);
    float d = GetV3Dot(D, AP);
    MulV3(D, d);
    Vector3 *X = NewSumV3(a, D);
    free(D);
    free(AP);
    return X;
}

float GetLineDistance(Vector3 *point, Vector3 *from, Vector3 *lineNorm) {
    Vector3 *AP = NewDiffV3(point, from);
    float d = GetV3Dot(lineNorm, AP);
    free(AP);
    return d;
}