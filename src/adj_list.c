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

    adj->L = calloc(n, sizeof(EdgeList));
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
    }

    free(adj->L);
    free(adj);
}

// Add an edge u -> v with a probability p
void adj_add(AdjList *adj, int u, int v, float p) {

    if (!adj) return;

    /* Validate */
    if (u < 0 || v < 0 || u >= adj->n || v >= adj->n) {
        return;
    }
    if (p < 0.0f || p > 1.0f) {
        return;
    }

    /* Allocate edge */
    EdgeCell *new = malloc(sizeof(EdgeCell));
    if (!new) return;

    new->p = p;
    new->v = v;
    new->next = adj->L[u].head;

    /* Insert at head */
    adj->L[u].head = new;
}

// Read graph from file and build adjacency list
AdjList *adj_read_file(const char *filename) {

    FILE *f = fopen(filename, "rt");
    if (!f) {
        return NULL;
    }

    int n = 0;
    if (fscanf(f, "%d", &n) != 1 || n <= 0) {
        fclose(f);
        return NULL;
    }

    AdjList *adj = adj_create(n);
    if (!adj) {
        fclose(f);
        return NULL;
    }

    int u, v;
    float p;

    /* Read edges until EOF */
    while (1) {

        int count = fscanf(f, "%d %d %f", &u, &v, &p);

        if (count == EOF || count == 0) {
            break;  // end or blank line
        }

        if (count != 3) {
            // invalid or partial line → ignore safely
            continue;
        }

        /* Convert from 1-based (file) to 0-based (internal) */
        int from = u - 1;
        int to   = v - 1;

        /* Validate before insertion */
        if (from >= 0 && from < adj->n &&
            to >= 0 &&   to < adj->n   )
        {
            adj_add(adj, from, to, p);
        }
    }

    fclose(f);
    return adj;
}

// Display adj list (debugging)
void adj_print(AdjList *adj) {
    if (!adj) return;

    for (int i = 0; i < adj->n; i++) {

        printf("%d :", i + 1);  // print as 1-based for clarity

        EdgeCell *cur = adj->L[i].head;
        while (cur != NULL) {
            printf(" -> %d (%.2f)", cur->v + 1, cur->p);  // convert back to 1-based
            cur = cur->next;
        }

        printf("\n");
    }
}

