#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adj_list.h"
#include "tarjan.h"
#include "partition.h"
#include "hasse.h"

/* Print the partition (one line per strongly connected component). */
static void printPartition(const Partition *p)
{
    for (int c = 0; c < p->count; c++) {
        const Class *cl = &p->classes[c];
        printf("Component C%d: {", c + 1);
        for (int k = 0; k < cl->size; k++) {
            printf("%d", cl->vertices[k]);
            if (k < cl->size - 1) {
                printf(", ");
            }
        }
        printf("}\n");
    }
}

int main(void)
{
    char filename[256];

    printf("Enter graph file path: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error: invalid input.\n");
        return EXIT_FAILURE;
    }

    printf("\nLoading graph from file: %s\n", filename);

    /* Build adjacency list from file. */
    AdjList *adj = adjReadFile(filename);
    if (adj == NULL) {
        fprintf(stderr, "Error: could not read graph from file.\n");
        return EXIT_FAILURE;
    }

    int n = adj->n;
    printf("Graph loaded with %d vertices.\n\n", n);

    /* Create partition structure and run Tarjan to compute SCCs. */
    Partition partition = partitionCreate(n);
    if (partition.v2c == NULL) {
        fprintf(stderr, "Error: could not allocate partition.\n");
        adjFree(adj);
        return EXIT_FAILURE;
    }

    int status = tarjanRun(adj, &partition);
    if (status != 0) {
        fprintf(stderr, "Error: Tarjan algorithm failed (code %d).\n", status);
        partitionFree(&partition);
        adjFree(adj);
        return EXIT_FAILURE;
    }

    printf("=== Strongly Connected Components (Tarjan) ===\n");
    printPartition(&partition);

    printf("\nVertex to class mapping (v2c):\n");
    for (int v = 1; v <= n; ++v) {
        printf("  vertex %d -> C%d\n", v, partition.v2c[v] + 1);
    }
    printf("\n");

    /* Build Hasse links between classes and remove transitive edges. */
    t_link_array links;
    initLinkArray(&links);
    buildLinksBetweenClasses(adj, &partition, &links);
    removeTransitiveLinks(&links);

    /* Added by AI to make Mermaid file creation easier. Create the corresponding output file name
     "<name>_hasse.mmd" inside ../data/. */

    char outputPath[256];

    const char *base = filename;
    const char *slash1 = strrchr(filename, '/');
    const char *slash2 = strrchr(filename, '\\');
    if (slash1 && slash1 > base) base = slash1 + 1;
    if (slash2 && slash2 > base) base = slash2 + 1;

    // we copy the name "example2.txt"
    strcpy(outputPath, "../output_files/");
    strcat(outputPath, base);

    // we replace ".txt" by "_hasse.mmd"
    char *dot = strrchr(outputPath, '.');
    if (dot) {
        strcpy(dot, "_hasse.mmd");
    } else {
        strcat(outputPath, "_hasse.mmd");
    }
    printf("\nSaving Mermaid diagram to %s...\n", outputPath);
    printHasseMermaidToFile(&partition, &links, outputPath);
    printf("Done.\n");

    freeLinkArray(&links);
    partitionFree(&partition);
    adjFree(adj);

    return EXIT_SUCCESS;
}
