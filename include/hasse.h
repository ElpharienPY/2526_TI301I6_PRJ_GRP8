#ifndef HASSE_H
#define HASSE_H

#include "adj_list.h"
#include "partition.h"

typedef struct {
    int from_class;
    int to_class;
} t_link;



typedef struct {
    t_link *data;
    int size;
    int capacity;
} t_link_array;



void initLinkArray(t_link_array *p_array);
void freeLinkArray(t_link_array *p_array);
int  linkExists(const t_link_array *p_array, int from_class, int to_class);
void addLink(t_link_array *p_array, int from_class, int to_class);



void buildLinksBetweenClasses(const AdjList *adj, const Partition *p, t_link_array *p_links);

void printHasseMermaidToFile(const Partition *p, const t_link_array *links, const char *filepath);

void removeTransitiveLinks(t_link_array *p_link_array);

#endif
