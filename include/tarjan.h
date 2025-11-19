
#ifndef INC_2526_TI301I6_PRJ_GRP8_TARJAN_H
#define INC_2526_TI301I6_PRJ_GRP8_TARJAN_H
 
#include  "adj_list.h"

#include  "partition.h"
 
typedef struct {

    int id;

    int index;

    int lowlink;

    int on_stack;

} TarjanVertex;
 
typedef struct {

    int *data;

    int top;

    int size;

} IntStack;
 
// Global metadata used by Tarjan's algorithm.
 
typedef struct {

    TarjanVertex *vertices;

    int vertex_count;

    int current_index;

    IntStack *stack;

} TarjanMeta;
 
// ========== Stack Functions ==========
 
int stackInit(IntStack *stack, int size);
 
int stackPush(IntStack *stack, int value);
 
int stackPop(IntStack *stack);
 
int stackFree(IntStack *stack);
 
int stackIsEmpty(IntStack *stack);
 
int stackTop(IntStack *stack);
 
// ========== Tarjan Functions ==========
 
TarjanMeta TarjanMetaCreate();
 
void tarjanMetaFree(TarjanMeta *tarjan_meta);
 
void tarjanRun(const AdjList *adj, Partition *partition);
 
 
 
 
#endif //INC_2526_TI301I6_PRJ_GRP8_TARJAN_H

 