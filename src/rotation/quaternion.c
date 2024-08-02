#include "quaternion.h"
#include <math.h>
#include <stdlib.h>

Quaternion *NewZeroQuaternion() {
    Quaternion* q = (Quaternion *)malloc(sizeof(Quaternion));
    q->x = 0;
    q->y = 0;
    q->z = 0;
    q->w = 1;
    return q;
}

Quaternion *NewQuaternion(Vector3 *v, float angle) {
    Quaternion *q = NewZeroQuaternion();
    Vector3 *nv = NewNormalizedV3(v);
    angle /= 2.0f;
    q->w = cosf(angle);
    float s = sinf(angle);
    q->x = nv->x * s;
    q->y = nv->y * s;
    q->z = nv->z * s;
    free(nv);
    return q;
}

Quaternion *NewCopyQuaternion(Quaternion *q) {
    Quaternion *cq = NewZeroQuaternion();
    cq->w = q->w;
    cq->x = q->x;
    cq->y = q->y;
    cq->z = q->z;
    return cq;
}

Quaternion *NewInvertedQuaternion(Quaternion *q) {
    Quaternion *inverted = NewCopyQuaternion(q);
    InvertQuaternion(inverted);
    return inverted;
}

Quaternion *NewMulV3Quaternion(Quaternion *q, Vector3 *v) {
    Quaternion* res = NewCopyQuaternion(q);
    MulV3Quaternion(res, v);
    return res;
}

Quaternion *NewMulQuaternion(Quaternion *a, Quaternion *b) {
    Quaternion* res = NewCopyQuaternion(a);
    MulQuaternion(res, b);
    return res;
}

Vector3* NewRotatedV3(Vector3* v, Quaternion*q) {
    Vector3* res = NewCopyV3(v);
    RotateV3(res, q);
    return res;
}

void RotateV3(Vector3 *v, Quaternion *q) {
    Quaternion *t = NewMulV3Quaternion(q, v);
    Quaternion *qInverted = NewInvertedQuaternion(q);
    Quaternion *t1 = NewMulQuaternion(t, qInverted);
    v->x = t1->x;
    v->y = t1->y;
    v->z = t1->z;
    free(t);
    free(qInverted);
    free(t1);
}

void InvertQuaternion(Quaternion *q) {
    q->x = -q->x;
    q->y = -q->y;
    q->z = -q->z;
}

void MulV3Quaternion(Quaternion *q, Vector3 *v) {
    Quaternion res;
    // res.w = -a.x * b.x - a.y * b.y - a.z * b.z
    // res.x = a.w * b.x + a.y * b.z - a.z * b.y
    // res.y = a.w * b.y - a.x * b.z + a.z * b.x
    // res.z = a.w * b.z + a.x * b.y - a.y * b.x
    res.w = -q->x * v->x - q->y * v->y - q->z * v->z;
    res.x = q->w * v->x + q->y * v->z - q->z * v->y;
    res.y = q->w * v->y - q->x * v->z + q->z * v->x;
    res.z = q->w * v->z + q->x * v->y - q->y * v->x;
    q->w = res.w;
    q->x = res.x;
    q->y = res.y;
    q->z = res.z;
}

void MulQuaternion(Quaternion *q, Quaternion *by) {
    Quaternion res;
    res.w = q->w * by->w - q->x * by->x - q->y * by->y - q->z * by->z;
    res.x = q->w * by->x + q->x * by->w + q->y * by->z - q->z * by->y;
    res.y = q->w * by->y - q->x * by->z + q->y * by->w + q->z * by->x;
    res.z = q->w * by->z + q->x * by->y - q->y * by->x + q->z * by->w;
    q->w = res.w;
    q->x = res.x;
    q->y = res.y;
    q->z = res.z;
}
