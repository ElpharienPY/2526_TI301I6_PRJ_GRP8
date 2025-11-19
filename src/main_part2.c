#include <stdio.h>
#include <stdlib.h>
#include "adj_list.h"
#include "tarjan.h"
#include "partition.h"

/* ---- Fonction d'affichage des composantes ---- */
static void printPartition(const Partition *p) {
    if (p == NULL) {
        printf("Partition is NULL.\n");
        return;
    }

    for (int i = 0; i < p->count; i++) {
        printf("Component C%d: {", i + 1);

        for (int j = 0; j < p->classes[i].size; j++) {
            printf("%d", p->classes[i].vertices[j]);
            if (j < p->classes[i].size - 1) {
                printf(", ");
            }
        }

        printf("}\n");
    }
}

/* ---- MAIN ---- */
int main(void)
{
    char filename[256];

    printf("Enter graph file path: ");
    if (scanf("%255s", filename) != 1) {
        printf("Error reading the file name.\n");
        return EXIT_FAILURE;
    }

    /* Load graph */
    AdjList *adj = adj_read_file(filename);
    if (adj == NULL) {
        printf("Error: could not read graph.\n");
        return EXIT_FAILURE;
    }

    printf("Graph loaded: %d vertices.\n\n", adj->n);

    /* Create empty partition */
    Partition partition = partitionCreate(adj->n);
    if (partition.v2c == NULL) {
        printf("Error: could not initialize partition.\n");
        adj_free(adj);
        return EXIT_FAILURE;
    }

    /* Run Tarjan algorithm */
    int status = tarjanRun(adj, &partition);
    if (status != 0) {
        printf("Error during Tarjan execution. Code: %d\n", status);
        partitionFree(&partition);
        adj_free(adj);
        return EXIT_FAILURE;
    }

    /* Print result */
    printf("=== Strongly Connected Components (Tarjan Result) ===\n");
    printPartition(&partition);

    /* Clean memory */
    partitionFree(&partition);
    adj_free(adj);

    return EXIT_SUCCESS;
}
