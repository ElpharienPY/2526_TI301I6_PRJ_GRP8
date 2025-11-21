#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adj_list.h"
#include "export_mermaid.h"
#include "markov_check.h"

static void printAdjacencyAndCheck(AdjList *adj) {
    printf("=== Adjacency List (%d vertices) ===\n", adj->n);
    adjPrint(adj);

    const float LO = 0.99f;
    const float HI = 1.00f;

    printf("\n=== Markov Check (tolerance [%.2f ; %.2f]) ===\n", LO, HI);
    markovReport(adj, LO, HI);
}

int main(void)
{
    char filename[256];
    char outputPath[256];

    printf("\n=== Main part 1 ===\n");
    printf("Enter graph file path: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error: invalid input.\n");
        return EXIT_FAILURE;
    }

    printf("\nLoading graph from file: %s\n", filename);

    AdjList *adj = adjReadFile(filename);
    if (adj == NULL) {
        fprintf(stderr, "Error: could not read graph from file.\n");
        return EXIT_FAILURE;
    }

    int n = adj->n;
    printf("Graph loaded with %d vertices.\n\n", n);

    printAdjacencyAndCheck(adj);

    const float LO = 0.99f;
    const float HI = 1.00f;
    MarkovResult result = markovIsValid(adj, LO, HI);

    if (!result.is_markov) {
        fprintf(stderr, "\nGraph is NOT Markov-valid.\n");
        printf("Vertices failing check: %d\n", result.bad_count);
        adjFree(adj);
        return 1;
    }

    printf("\nGraph is Markov-valid. Vertices failing check: %d\n",
           result.bad_count);

    /* Build output Mermaid file path */
    const char *base = filename;
    const char *slash1 = strrchr(filename, '/');
    const char *slash2 = strrchr(filename, '\\');
    if (slash1 && slash1 > base) base = slash1 + 1;
    if (slash2 && slash2 > base) base = slash2 + 1;

    strcpy(outputPath, "../output_files/");
    strcat(outputPath, base);

    char *dot = strrchr(outputPath, '.');
    if (dot) {
        strcpy(dot, "_graph.mmd");
    } else {
        strcat(outputPath, "_graph.mmd");
    }

    printf("\nSaving Mermaid Markov graph to %s...\n", outputPath);

    int rc = writeMermaid(adj, outputPath);
    if (rc != 0) {
        fprintf(stderr, "Error: could not write Mermaid file.\n");
        adjFree(adj);
        return 2;
    }

    printf("Done.\n");

    adjFree(adj);
    return EXIT_SUCCESS;
}
