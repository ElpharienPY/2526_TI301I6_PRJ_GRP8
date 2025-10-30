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
AdjList *adj_create(int n);

// Free all the memory
void adj_free(AdjList *adj);

// Add a directed edge u,v with probability p
void adj_add(AdjList *adj, int u, int v, float p);

// Read a graph from another file and create an adjacency list
AdjList *adj_read_file(const char *filename);

// Display the adjacency list
void adj_print(AdjList *adj);

#endif //ADJ_LIST_H
