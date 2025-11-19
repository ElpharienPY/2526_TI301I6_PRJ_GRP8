#ifndef HASSE_H
#define HASSE_H

#include "adj_list.h"
#include "partition.h"

/* ---------- Lien entre deux classes ---------- */

typedef struct {
    int from_class;   // indice de la classe de départ
    int to_class;     // indice de la classe d'arrivée
} t_link;

/* ---------- Tableau dynamique de liens ---------- */

typedef struct {
    t_link *data;
    int size;
    int capacity;
} t_link_array;

/* ---------- Gestion du tableau de liens ---------- */

void init_link_array(t_link_array *p_array);
void free_link_array(t_link_array *p_array);
int  link_exists(const t_link_array *p_array, int from_class, int to_class);
void add_link(t_link_array *p_array, int from_class, int to_class);

/* ---------- Construction des liens entre classes ---------- */

void build_links_between_classes(const AdjList *adj,
                                 const Partition *p,
                                 t_link_array *p_links);

/* ---------- Affichage du diagramme de Hasse (format Mermaid) ---------- */

void print_hasse_mermaid(const Partition *p,
                         const t_link_array *p_links);

/* ---------- Optionnel : suppression des liens transitifs ---------- */

void removeTransitiveLinks(t_link_array *p_link_array);

#endif
