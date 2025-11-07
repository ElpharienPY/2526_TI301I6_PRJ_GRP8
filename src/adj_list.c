#include "adj_list.h"
#include <stdlib.h>

// Create an empty adjacency list of size n
AdjList *adj_create(int n) {
    if (n <= 0) {
        return NULL;
    }
    AdjList *adj = malloc(sizeof(AdjList));
    if (!adj) {
        return NULL;
    }

    adj->n = n;
    adj->L = (EdgeList*)calloc((size_t)n, sizeof(EdgeList));
    if (!adj->L) {
        free(adj);
        return NULL;
    }

    return adj;
}

// Free all memory used
void adj_free(AdjList *adj) {
    if (!adj) {
        return;
    }

    for (int i = 0; i < adj->n; i++) {
        EdgeCell *cur = adj->L[i].head;
        while (cur != NULL) {
            EdgeCell *next = cur->next;
            free(cur);
            cur = next;
        }
        adj->L[i].head = NULL;
    }

    free(adj->L);
    adj->L = NULL;
    adj->n = 0;
    free(adj);
}

// Add an edge u -> v with a probability p
void adj_add(AdjList *adj, int u, int v, float p) {
    if (!adj) {
        return;
    }

    if (u < 0 || v < 0 || u >= adj->n || v >= adj->n) {
        return;
    }

    if (p < 0.0 || p > 1.0) {
        return;
    }

    EdgeCell *new = (EdgeCell*)malloc(sizeof(*new));
    if (!new) {
        return;
    }

    new->p = p;
    new->v = v;
    new->next = NULL;

    if (adj->L[u].head == NULL) {
        adj->L[u].head = new;
    }
    else {
        new->next = adj->L[u].head;
        adj->L[u].head = new;
    }
}

// Read graph from file and build adjacency list
AdjList *adj_read_file(const char *filename) {
    FILE *f = fopen(filename, "rt");
    if (!f) {
        return NULL;
    }

    int n = 0;

    if (fscanf(f, "%d", &n) != 1) {
        fclose(f);
        return NULL;
    }

    AdjList *adj = malloc(sizeof(*adj));
    if (!adj) {
        free(adj);
        fclose(f);
        return NULL;
    }
    adj->n = n;

    adj->L = malloc(n * sizeof(*(adj->L)));
    for (int i=0; i<n; i++) {
        adj->L[i].head = NULL; // Pointer to the first edge
    }

    int u, v;
    float p;
    while (fscanf(f,"%d %d %f", &u, &v,&p) == 3) {
        int from = u -1;
        int to = v -1;

        adj_add(adj,from, to, p);
    }

    fclose(f);
    return adj;

}

// Display adj list (debugging)
void adj_print(AdjList *adj) {
    if (!adj) {
        return;
    }

    for (int i = 0; i < adj->n; i++) {
        printf("%d :",i);

        EdgeCell *cur = adj->L[i].head;

        while (cur != NULL) {
            printf(" -> %d (%.2f)", cur->v, cur->p);
            cur = cur->next;
        }
    }
}

