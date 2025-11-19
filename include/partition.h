
#ifndef INC_2526_TI301I6_PRJ_GRP8_PARTITION_H
#define INC_2526_TI301I6_PRJ_GRP8_PARTITION_H

// Data Structures

typedef struct {
    int *vertices;
    int size;
} Class;

typedef struct {
    Class *classes;
    int count;
    int *v2c;
} Partition;

// Partition management

Partition partitionCreate(int vertexCount);

void partitionFree(Partition *p);

int partitionAddClass(Partition *p, const int *vertices, int count);



#endif //INC_2526_TI301I6_PRJ_GRP8_PARTITION_H