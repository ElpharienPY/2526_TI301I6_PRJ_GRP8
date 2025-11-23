#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <stdio.h>

// Structure representing an edge (transition) to a target vertex
//(It is a node in a singly linked list)

typedef struct EdgeCell {
    int v;
    float p;
    struct EdgeCell *next;
} EdgeCell;

// list of outgoing edges for a given vertex
typedef struct {
    EdgeCell *head;
} EdgeList;

// Main graph structure (Adjacency List)
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
