#include "geometry.h"
#include "string.h"
#include <stdio.h>

#ifndef MAX_OBJ_FILE_SIZE
#define MAX_OBJ_FILE_SIZE 1024 * 1024
#endif

Geometry *NewEmptyGeometry(void);

Geometry *LoadObj(PlaydateAPI *pd, const char *path) {
    SDFile *file = pd->file->open(path, kFileRead);
    if (file == NULL) {
        return NULL;
    }

    char *buf = (char *)malloc(sizeof(char) * MAX_OBJ_FILE_SIZE);
    int size = pd->file->read(file, buf, MAX_OBJ_FILE_SIZE);
    if (size == MAX_OBJ_FILE_SIZE) {
        return NULL;
    }

    Geometry *o = NewEmptyGeometry();

    for (int i = 1; i < size - 1; i++) {
        if (buf[i - 1] == '\n' && buf[i + 1] == ' ') {
            if (buf[i] == 'v') {
                o->vCount++;
            } else if (buf[i] == 'f') {
                o->fCount++;
            }
        }
    }

    o->v = (Vector3 *)malloc(sizeof(Vector3) * o->vCount);
    o->f = (GeometryFace *)malloc(sizeof(GeometryFace) * o->fCount);

    int v = 0;
    int f = 0;
    char *trash = (char *)malloc(sizeof(char) * 100);
    for (int i = 1; i < size - 1; i++) {
        if (buf[i - 1] == '\n' && buf[i + 1] == ' ') {
            if (buf[i] == 'v') {
                sscanf(&buf[i], "v %f %f %f", &o->v[v].x, &o->v[v].y,
                       &o->v[v].z);
                v++;
            } else if (buf[i] == 'f') {
                sscanf(&buf[i], "f %hu%s %hu%s %hu", &o->f[f].a, trash,
                       &o->f[f].b, trash, &o->f[f].c);
                o->f[f].a--;
                o->f[f].b--;
                o->f[f].c--;
                f++;
            }
        }
    }
    free(trash);
    free(buf);

    return o;
}