#include <stdlib.h>
#include "tarjan.h"

/* Return the number of vertices in the adjacency list. */
static int getVertexCount(const AdjList *adj)
{
    return adj->n;
}

/* Initialize an integer stack. */
int stackInit (IntStack *stack, int size) {
    if (stack == NULL || size <= 0) {
        return 1;
    }

    stack->data= (int *)malloc(size * sizeof(int));
    if (stack->data == NULL) {
        stack->size = 0;
        stack->top = -1;
        return 2;
    }

    stack->size = size;
    stack->top = -1;
    return 0;
}


/* Free the memory used by the stack. */
void stackFree(IntStack *stack) {
    if (stack == NULL) {
        return;
    }

    if (stack->data != NULL) {
        free(stack->data);
        stack->data = NULL;
    }

    stack->size = 0;
    stack->top = -1;
}

/* Return 1 if the stack is empty, 0 otherwise. */
int stackIsEmpty(const IntStack *stack) {

    if (stack == NULL) {
        return 1;
    }
    return (stack->top < 0);

}

/* Push a value on top of the stack. */
int stackPush(IntStack *stack, int value) {

    if (stack == NULL || stack->data == NULL) {
        return 1;
    }

    if (stack->top + 1 >= stack->size) {
        return 2;
    }

    stack->top++;
    stack->data[stack->top] = value;
    return 0;
}

/* Read top element of the stack without removing it. */
int stackTop(const IntStack *stack, int *value) {
    if (stack == NULL || value == NULL) {
        return 1;
    }

    if (stack->top < 0) {
        return 2;
    }

    *value = stack->data[stack->top];
    return 0;
}

/* Pop the top element from the stack. */
int stackPop(IntStack *stack, int *value ) {
    if (stack == NULL || value == NULL || stack->data == NULL) {
        return 1;
    }

    if (stack->top < 0) {
        return 2;
    }

    *value = stack->data[stack->top];
    stack->top--;
    return 0;
}

/* TARJAN META INITIALISATION */
/* Create and initialize Tarjan metadata for all vertices. */
TarjanMeta tarjanMetaCreate(int vertexCount)
{
    TarjanMeta meta;
    meta.vertices = NULL;
    meta.vertexCount = vertexCount;
    meta.currentIndex = 0;
    meta.stack = NULL;

    if (vertexCount <= 0) {
        return meta;
    }

    /* Allocate vertices (1..vertexCount) */
    meta.vertices = (TarjanVertex *)malloc((vertexCount + 1) * sizeof(TarjanVertex));
    if (meta.vertices == NULL) {
        return meta;
    }

    for (int i = 0; i <= vertexCount; i++) {
        meta.vertices[i].id = i;
        meta.vertices[i].index = -1;
        meta.vertices[i].lowLink = -1;
        meta.vertices[i].onStack = 0;
    }

    /* Allocate stack structure */
    meta.stack = (IntStack *)malloc(sizeof(IntStack));
    if (meta.stack == NULL) {
        free(meta.vertices);
        meta.vertices = NULL;
        return meta;
    }

    if (stackInit(meta.stack, vertexCount) != 0) {
        free(meta.stack);
        meta.stack = NULL;
        free(meta.vertices);
        meta.vertices = NULL;
        return meta;
    }

    return meta;
}

/* Free all memory used by Tarjan metadata. */
void tarjanMetaFree(TarjanMeta *meta) {
    if (meta == NULL) {
        return;
    }

    if (meta->vertices != NULL) {
        free(meta->vertices);
        meta->vertices = NULL;
    }

    if (meta->stack != NULL) {
        stackFree(meta->stack);
        free(meta->stack);
        meta->stack = NULL;
    }

    meta->vertexCount = 0;
    meta->currentIndex = 0;
}

/* INTERNAL DFS FUNCTION — TARJAN VISIT */
/* Depth-first search implementing Tarjan's SCC algorithm (pseudo-code translated to C). */
static void tarjanVisit(const AdjList *adj, int v, TarjanMeta *meta, Partition *partition)
{
    if (v < 1 || v > meta->vertexCount) {
        printf("[ERROR] tarjanVisit: invalid v = %d (allowed: 1..%d)\n",
               v, meta->vertexCount);
        return;
    }

    TarjanVertex *tv = &meta->vertices[v];

    tv->index   = meta->currentIndex;
    tv->lowLink = meta->currentIndex;
    meta->currentIndex++;

    stackPush(meta->stack, v);
    tv->onStack = 1;

    /* Compute list index */
    int idx = v - 1;

    if (idx < 0 || idx >= adj->n) {
        printf("[ERROR] tarjanVisit: idx=%d out of range (adj->n=%d)\n",
               idx, adj->n);
        return;
    }

    EdgeCell *edge = adj->L[idx].head;

    /* Scan neighbourhood */
    while (edge != NULL) {

        int w = edge->v;   // w is 0-based in adjacency list

        /* Convert 0-based storage to 1-based Tarjan vertices */
        int w_vertex = w + 1;

        if (w_vertex < 1 || w_vertex > meta->vertexCount) {
            printf("[ERROR] Invalid edge: %d -> %d (converted %d)  [vertexCount=%d]\n",
                   v, edge->v, w_vertex, meta->vertexCount);
            return;
        }

        TarjanVertex *tw = &meta->vertices[w_vertex];

        if (tw->index == -1) {
            /* Unvisited → recursion */
            tarjanVisit(adj, w_vertex, meta, partition);

            if (tw->lowLink < tv->lowLink) {
                tv->lowLink = tw->lowLink;
            }
        }
        else if (tw->onStack) {
            if (tw->index < tv->lowLink) {
                tv->lowLink = tw->index;
            }
        }

        edge = edge->next;
    }

    /* If v is a root of SCC, pop stack until v to form this component. */
    if (tv->lowLink == tv->index) {

        int *component = malloc(meta->vertexCount * sizeof(int));
        if (!component) return;

        int count = 0, w = -1;

        do {
            stackPop(meta->stack, &w);

            if (w < 1 || w > meta->vertexCount) {
                printf("[ERROR] Popped invalid w=%d\n", w);
                break;
            }

            meta->vertices[w].onStack = 0;
            component[count++] = w;

        } while (w != v && !stackIsEmpty(meta->stack));

        partitionAddClass(partition, component, count);
        free(component);
    }
}

/* Run Tarjan algorithm on the whole graph to fill the partition with SCCs. */
int tarjanRun(const AdjList *adj, Partition *partition)
{
    if (adj == NULL || partition == NULL) {
        return 1;
    }

    int n = getVertexCount(adj);
    if (n <= 0) {
        return 2;
    }

    TarjanMeta meta = tarjanMetaCreate(n);
    if (meta.vertices == NULL || meta.stack == NULL || meta.stack->data == NULL) {
        tarjanMetaFree(&meta);
        return 3;
    }

    for (int v = 1; v <= n; v++) {
        if (meta.vertices[v].index == -1) {
            tarjanVisit(adj, v, &meta, partition);
        }
    }

    tarjanMetaFree(&meta);
    return 0;
}



