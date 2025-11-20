#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"

static int ensureCapacity(t_link_array *array) {
    if (array == NULL) {
        return 1;
    }

    if (array->capacity == 0) {
        array->capacity = 4;
        array->data = (t_link *)malloc(array->capacity * sizeof(t_link));
        if (array->data == NULL) {
            array->capacity = 0;
            return 2;
        }
    } else if (array->size >= array->capacity) {
        int newCapacity = array->capacity * 2;
        t_link *newData = (t_link *)realloc(array->data, newCapacity * sizeof(t_link));
        if (newData == NULL) return 3;
        array->data = newData;
        array->capacity = newCapacity;
    }

    return 0;
}

void initLinkArray(t_link_array *array) {
    if (array == NULL) return;
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}

void freeLinkArray(t_link_array *array) {
    if (array == NULL) return;
    if (array->data != NULL) free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}

int linkExists(const t_link_array *array, int fromClass, int toClass) {
    if (array == NULL || array->data == NULL) return 0;
    for (int i = 0; i < array->size; i++) {
        if (array->data[i].from_class == fromClass &&
            array->data[i].to_class == toClass)
            return 1;
    }
    return 0;
}

void addLink(t_link_array *array, int fromClass, int toClass)
{
    if (array == NULL) return;
    if (ensureCapacity(array) != 0) return;

    array->data[array->size].from_class = fromClass;
    array->data[array->size].to_class = toClass;
    array->size++;
}

void buildLinksBetweenClasses(const AdjList *adj,
                              const Partition *p,
                              t_link_array *links)
{
    if (adj == NULL || p == NULL || links == NULL) {
        return;
    }

    const int *v2c = p->v2c;
    int n = adj->n;

    for (int i = 0; i < n; i++) {
        int vertex = i + 1;              // sommets numérotés 1..n
        int classFrom = v2c[vertex];     // classe du sommet

        if (classFrom < 0) {
            continue;                    // on ignore si pas de classe
        }

        EdgeCell *cell = adj->L[i].head; // liste sortante de "vertex"

        while (cell != NULL) {
            int neighbour = cell->v;         // voisin 1..n
            int classTo = v2c[neighbour];    // classe du voisin

            if (classTo >= 0 && classFrom != classTo) {
                printf("edge %d -> %d gives class C%d -> C%d\n", vertex, neighbour, classFrom + 1, classTo + 1);
                if (!linkExists(links, classFrom, classTo)) {
                    addLink(links, classFrom, classTo);
                }
            }

            cell = cell->next;
        }
    }
}


void printHasseMermaidToFile(const Partition *p, const t_link_array *links, const char *filepath){
    FILE *f = fopen(filepath, "w");
    if (f == NULL) {
        perror("fopen");
        return;
    }

    fprintf(f, "graph TD;\n");

    for (int c = 0; c < p->count; c++) {
        const Class *cl = &p->classes[c];
        fprintf(f, "  C%d[\"C%d: {", c + 1, c + 1);
        for (int k = 0; k < cl->size; k++) {
            fprintf(f, "%d", cl->vertices[k]);
            if (k < cl->size - 1) fprintf(f, ", ");
        }
        fprintf(f, "}\"];\n");
    }

    for (int i = 0; i < links->size; i++) {
        fprintf(f, "  C%d --> C%d;\n",
                links->data[i].from_class + 1,
                links->data[i].to_class + 1);
    }

    fclose(f);
}

void removeTransitiveLinks(t_link_array *array) {
    int i = 0;
    while (i < array->size)
    {
        t_link link1 = array->data[i];
        int j = 0;
        int toRemove = 0;
        while (j < array->size && !toRemove)
        {
            if (j != i)
            {
                t_link link2 = array->data[j];
                if (link1.from_class == link2.from_class)
                {
                    int k = 0;
                    while (k < array->size && !toRemove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = array->data[k];
                            if ((link3.from_class == link2.to_class) &&
                                (link3.to_class == link1.to_class))
                            {
                                toRemove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (toRemove)
        {
            array->data[i] = array->data[array->size - 1];
            array->size--;
        }
        else
        {
            i++;
        }
    }
}

