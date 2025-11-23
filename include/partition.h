
#ifndef INC_2526_TI301I6_PRJ_GRP8_PARTITION_H
#define INC_2526_TI301I6_PRJ_GRP8_PARTITION_H

// Data Structures

//represents a class
//contains the list of vertices belonging to this class
typedef struct {
    int *vertices;
    int size;
} Class;

//represents the complete partition of the graph (set of all classes)
typedef struct {
    Class *classes;
    int count;
    int *v2c;
} Partition;

// Partition management

//creates and initializes and empty partition
Partition partitionCreate(int vertexCount);

//frees all memory used for the partition and its classes
void partitionFree(Partition *p);

//adds a new class to the partition
int partitionAddClass(Partition *p, const int *vertices, int count);



#endif //INC_2526_TI301I6_PRJ_GRP8_PARTITION_H