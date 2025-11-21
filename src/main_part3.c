#include <stdio.h>
#include <stdlib.h>

#include "adj_list.h"
#include "matrix.h"
#include "tarjan.h"
#include "partition.h"

/* ---------------------------------------------------------
   Helper : compute stationary distribution of a matrix
   --------------------------------------------------------- */
static void compute_stationary_for_matrix(t_matrix M, double epsilon,
                                          int max_iter, const char *label)
{
    int n = M.size;

    t_matrix powM  = matrixCreate(n);
    t_matrix prevM = matrixCreate(n);
    t_matrix tmp   = matrixCreate(n);

    matrixCopy(powM, M);

    double diff = 1.0;
    int iter = 1;

    while (diff > epsilon && iter < max_iter) {
        matrixCopy(prevM, powM);        // M^(k-1)
        matrixMultiply(prevM, M, tmp);  // tmp = M^(k-1) * M
        matrixCopy(powM, tmp);          // powM = M^k

        diff = matrixDiff(powM, prevM);
        iter++;
    }

    printf("\n=== %s ===\n", label);

    if (iter >= max_iter) {
        printf("  No convergence after %d iterations (graph may be periodic)\n",
               max_iter);
    } else {
        printf("  Convergence reached at n = %d (difference = %g)\n",
               iter, diff);
        printf("  Candidate stationary distribution:\n");
        matrixPrint(powM);
    }

    matrixFree(&powM);
    matrixFree(&prevM);
    matrixFree(&tmp);
}

/* ---------------------------------------------------------
   MAIN
   --------------------------------------------------------- */
int main(void)
{
    char filename[256];

    /* 1. Ask user for graph file */
    printf("Enter graph file path: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error: invalid input.\n");
        return EXIT_FAILURE;
    }

    printf("\n--- LOADING GRAPH: %s ---\n", filename);
    AdjList *adj = adjReadFile(filename);

    if (!adj) {
        fprintf(stderr, "Error: unable to read file or invalid graph.\n");
        return EXIT_FAILURE;
    }

    /* 2. Build transition matrix M */
    printf("\n--- 1. TRANSITION MATRIX M ---\n");
    t_matrix M = adjToMatrix(adj);
    matrixPrint(M);

    int n = M.size;

    t_matrix res  = matrixCreate(n);
    t_matrix powM = matrixCreate(n);

    matrixCopy(powM, M);

    /* 3. Compute M^3 (2 more multiplications) */
    for (int k = 0; k < 2; k++) {
        matrixMultiply(powM, M, res);
        matrixCopy(powM, res);
    }

    printf("\n--- 2. MATRIX M^3 (3-step transition) ---\n");
    matrixPrint(powM);

    /* 4. Compute M^7 (continue from M^3) */
    for (int k = 0; k < 4; k++) {
        matrixMultiply(powM, M, res);
        matrixCopy(powM, res);
    }

    printf("\n--- 3. MATRIX M^7 (7-step transition) ---\n");
    matrixPrint(powM);

    /* 5. Global convergence on the full matrix */
    printf("\n--- 4. GLOBAL CONVERGENCE TEST ---\n");
    compute_stationary_for_matrix(M, 0.01, 1000, "Full Matrix M");

    /* 6. Compute partition with Tarjan */
    printf("\n--- 5. TARJAN PARTITION (STRONGLY CONNECTED COMPONENTS) ---\n");

    Partition part = partitionCreate(adj->n);
    int err = tarjanRun(adj, &part);

    if (err != 0) {
        fprintf(stderr, "Error: tarjanRun failed with code %d\n", err);

        matrixFree(&M);
        matrixFree(&res);
        matrixFree(&powM);
        adjFree(adj);
        partitionFree(&part);

        return EXIT_FAILURE;
    }

    printf("Number of classes: %d\n", part.count);
    for (int c = 0; c < part.count; c++) {
        Class cls = part.classes[c];
        printf("  Class C%d (size %d): { ", c + 1, cls.size);

        for (int i = 0; i < cls.size; i++) {
            printf("%d", cls.vertices[i]);
            if (i < cls.size - 1) printf(", ");
        }

        printf(" }\n");
    }

    /* 7. Stationary distribution per class */
    printf("\n--- 6. STATIONARY DISTRIBUTION PER CLASS ---\n");

    for (int c = 0; c < part.count; c++) {
        t_matrix sub = subMatrix(M, part, c);

        char label[64];
        snprintf(label, sizeof(label), "Class C%d", c + 1);

        compute_stationary_for_matrix(sub, 0.01, 1000, label);

        int period = getPeriod(sub);
        printf("  Period of %s = %d\n\n", label, period);

        matrixFree(&sub);
    }

    /* Cleanup */
    matrixFree(&M);
    matrixFree(&res);
    matrixFree(&powM);
    adjFree(adj);
    partitionFree(&part);

    return EXIT_SUCCESS;
}
