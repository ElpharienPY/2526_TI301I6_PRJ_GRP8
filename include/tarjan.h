
#ifndef INC_2526_TI301I6_PRJ_GRP8_TARJAN_H
#define INC_2526_TI301I6_PRJ_GRP8_TARJAN_H
 
#include  "adj_list.h"

#include  "partition.h"

//metadata for vertex during tarjan's algorithm execution
typedef struct {
    int id;
    int index;
    int lowLink;
    int onStack;
} TarjanVertex;

//LIFO stack
typedef struct {
    int *data;
    int top;
    int size;
} IntStack;
 
// Global structure containing the state of the algorithm
typedef struct {
    TarjanVertex *vertices;
    int vertexCount;
    int currentIndex;
    IntStack *stack;
} TarjanMeta;
 
// ========== Stack Functions ==========
 
int stackInit(IntStack *stack, int size);
 
int stackPush(IntStack *stack, int value);
 
int stackPop(IntStack *stack, int *value);
 
void stackFree(IntStack *stack);
 
int stackIsEmpty(const IntStack *stack);
 
int stackTop(const IntStack *stack, int *value);
 
// ========== Tarjan Functions ==========

//initialize metadata for Tarjan's algorithm
TarjanMeta tarjanMetaCreate(int vertexCount);

//frees metadata memory
void tarjanMetaFree(TarjanMeta *meta);

//runs tarjan's algorithm on the graph to populate the partition
int tarjanRun(const AdjList *adj, Partition *partition);

 
#endif //INC_2526_TI301I6_PRJ_GRP8_TARJAN_H
