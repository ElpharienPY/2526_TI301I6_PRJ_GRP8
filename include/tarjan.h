
#ifndef INC_2526_TI301I6_PRJ_GRP8_TARJAN_H
#define INC_2526_TI301I6_PRJ_GRP8_TARJAN_H
 
#include  "adj_list.h"

#include  "partition.h"
 
typedef struct {
    int id;
    int index;
    int lowLink;
    int onStack;
} TarjanVertex;
 
typedef struct {
    int *data;
    int top;
    int size;
} IntStack;
 
// Global metadata used by Tarjan's algorithm.
 
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
 
TarjanMeta tarjanMetaCreate(int vertexCount);

void tarjanMetaFree(TarjanMeta *meta);

int tarjanRun(const AdjList *adj, Partition *partition);
 
 
 
 
#endif //INC_2526_TI301I6_PRJ_GRP8_TARJAN_H

 