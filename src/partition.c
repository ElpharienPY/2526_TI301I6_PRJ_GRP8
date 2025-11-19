#include <stdlib.h>
#include "partition.h"

Partition partitionCreate (int vertexCount) {
    Partition p;
    p.classes = NULL;
    p.count = 0;
    p.v2c = NULL;

    if (vertexCount <= 0) {
        return p;
    }

    p.v2c = (int *)malloc((vertexCount + 1) *sizeof(int));
    if (p.v2c == NULL) {
        return p;
    }

    for (int i = 0; i <= vertexCount; i++) {
        p.v2c[i] = -1;
    }

    return p;
}

int partitionAddClass(Partition *p, const int *vertices, int count) {
    if (p == NULL || vertices == NULL || count <= 0) {
        return 1; /* Invalid parameters */
    }

    int newCount = p->count + 1;

    /* Grow the classes array */
    Class *newArray = (Class *)realloc(p->classes, newCount * sizeof(Class));
    if (newArray == NULL) {
        return 2; /* Allocation failed for classes array */
    }

    p->classes = newArray;

    /* Allocate and copy the vertices of the new class */
    Class *newClass = &p->classes[newCount-1];

    newClass->vertices = (int *)malloc(count * sizeof(int));
    if (newClass->vertices == NULL) {
        return 3; /* Allocation failed for vertices array */
    }

    newClass->size = count;

    for (int i = 0; i < count; i++) {
        int v = vertices[i];
        newClass->vertices[i] = v;

        if (p->v2c != NULL && v >= 0) {
            p->v2c[v] = newCount - 1;
        }
    }

    p->count = newCount;

    return 0; /* Success */
}

void partitionFree(Partition *p) {
    if (p == NULL) {
        return;
    }

    /* Free each class's vertex array */
    if (p->classes != NULL) {
        for (int i = 0; i < p->count; i++) {
            if (p->classes[i].vertices != NULL) {
                free(p->classes[i].vertices);
                p->classes[i].vertices = NULL;
            }
            p->classes[i].size = 0;
        }

        free(p->classes);
        p->classes = NULL;
    }

    p->count = 0;

    /* Free vertex-to-class mapping */
    if (p->v2c != NULL) {
        free(p->v2c);
        p->v2c = NULL;
    }
}