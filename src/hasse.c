#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"

/* =========================================================
 *      PARTIE 1 : gestion dynamique du tableau de liens
 * ========================================================= */

static void ensure_capacity(t_link_array *p_array)
{
    if (p_array->capacity == 0) {
        p_array->capacity = 4;
        p_array->data = (t_link *)malloc(p_array->capacity * sizeof(t_link));
    } else if (p_array->size >= p_array->capacity) {
        p_array->capacity *= 2;
        p_array->data = (t_link *)realloc(p_array->data,
                                          p_array->capacity * sizeof(t_link));
    }
}

void init_link_array(t_link_array *p_array)
{
    p_array->data = NULL;
    p_array->size = 0;
    p_array->capacity = 0;
}

void free_link_array(t_link_array *p_array)
{
    if (p_array->data != NULL)
        free(p_array->data);

    p_array->data = NULL;
    p_array->size = 0;
    p_array->capacity = 0;
}

int link_exists(const t_link_array *p_array, int from_class, int to_class)
{
    for (int i = 0; i < p_array->size; ++i) {
        if (p_array->data[i].from_class == from_class &&
            p_array->data[i].to_class   == to_class)
            return 1;
    }
    return 0;
}

void add_link(t_link_array *p_array, int from_class, int to_class)
{
    ensure_capacity(p_array);
    p_array->data[p_array->size].from_class = from_class;
    p_array->data[p_array->size].to_class   = to_class;
    p_array->size++;
}

/* =========================================================
 *     PARTIE 2 : Construction du diagramme de Hasse
 * ========================================================= */

void build_links_between_classes(const AdjList *adj,
                                 const Partition *p,
                                 t_link_array *p_links)
{
    int nb_vertices = adj->n;

    // v2c = tableau qui donne la classe de chaque vertex
    const int *v2c = p->v2c;

    for (int i = 0; i < nb_vertices; ++i) {

        int Ci = v2c[i];  // classe du vertex i

        EdgeCell *cell = adj->L[i].head;

        while (cell != NULL) {
            int j  = cell->v;     // voisin
            int Cj = v2c[j];      // classe du voisin

            if (Ci != Cj) {
                if (!link_exists(p_links, Ci, Cj)) {
                    add_link(p_links, Ci, Cj);
                }
            }

            cell = cell->next;
        }
    }
}

/* =========================================================
 *       PARTIE 3 : Affichage en Mermaid
 * ========================================================= */

void print_hasse_mermaid(const Partition *p,
                         const t_link_array *p_links)
{
    printf("graph TD;\n");

    /* --- 1) Afficher chaque classe comme un noeud Mermaid --- */
    for (int c = 0; c < p->count; ++c) {

        const Class *cl = &p->classes[c];

        printf("  C%d[\"C%d: {", c+1, c+1);

        for (int k = 0; k < cl->size; ++k) {
            printf("%d", cl->vertices[k]);
            if (k < cl->size - 1)
                printf(", ");
        }

        printf("}\"];\n");
    }

    /* --- 2) Afficher les liens entre classes --- */
    for (int i = 0; i < p_links->size; ++i) {
        int from = p_links->data[i].from_class;
        int to   = p_links->data[i].to_class;

        // Classes indexées de 0 → Mermaid en 1,2,3,...
        printf("  C%d --> C%d;\n", from + 1, to + 1);
    }
}

/* =========================================================
 *       PARTIE 4 : liens transitifs (optionnel)
 * ========================================================= */

void removeTransitiveLinks(t_link_array *p_link_array)
{
    (void)p_link_array; // si tu veux laisser vide pour l’instant
}
