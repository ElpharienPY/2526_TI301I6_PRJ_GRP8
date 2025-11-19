#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <stdio.h>

typedef struct EdgeCell {
    int v;
    float p;
    struct EdgeCell *next;
} EdgeCell;

typedef struct {
    EdgeCell *head;
} EdgeList;

typedef struct {
    int n;
    EdgeList *L;
} AdjList;

/* ---- Prototypes functions ---- */

// Create an empty adjacency list
AdjList *adjCreate(int n);

// Free all the memory
void adjFree(AdjList *adj);

// Add a directed edge u,v with probability p
void adjAdd(AdjList *adj, int u, int v, float p);

// Read a graph from another file and create an adjacency list
AdjList *adjReadFile(const char *filename);

// Display the adjacency list
void adjPrint(AdjList *adj);

#endif //ADJ_LIST_H
