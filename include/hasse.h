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



void init_link_array(t_link_array *p_array);
void free_link_array(t_link_array *p_array);
int  link_exists(const t_link_array *p_array, int from_class, int to_class);
void add_link(t_link_array *p_array, int from_class, int to_class);



void build_links_between_classes(const AdjList *adj,
                                 const Partition *p,
                                 t_link_array *p_links);



void print_hasse_mermaid(const Partition *p,
                         const t_link_array *p_links);



void removeTransitiveLinks(t_link_array *p_link_array);

#endif
