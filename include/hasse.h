#ifndef HASSE_H
#define HASSE_H

#include "adj_list.h"
#include "partition.h"

//direct link between two classes
typedef struct {
    int from_class;
    int to_class;
} t_link;

//dynamic array of links
typedef struct {
    t_link *data;
    int size;
    int capacity;
} t_link_array;

//initialize an empty array of links
void initLinkArray(t_link_array *p_array);

//frees memory associated to an array of links
void freeLinkArray(t_link_array *p_array);

int  linkExists(const t_link_array *p_array, int from_class, int to_class);
void addLink(t_link_array *p_array, int from_class, int to_class);

//analyzes the graph and partition to find which classes communicate
void buildLinksBetweenClasses(const AdjList *adj, const Partition *p, t_link_array *p_links);

//exports the class diagram in mermaid format to a file
void printHasseMermaidToFile(const Partition *p, const t_link_array *links, const char *filepath);

//simplifies the diagram (remove transitive links)
void removeTransitiveLinks(t_link_array *p_link_array);

#endif
