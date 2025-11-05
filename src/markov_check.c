#include <stddef.h>
#include <stdio.h>
#include "markov_check.h"

MarkovResult markov_is_valid(const AdjList *adj, float lo, float hi) {
    // Initialize the result assuming the graph is valid
    MarkovResult res = {1, 0};

    // Input validation
    if (adj == NULL || adj->n <= 0 || lo > hi) {
        res.is_markov = 0;
        return res;
    }

    // Iterate over all vertices
    for (int i = 0; i < adj->n; ++i) {
        float sum = 0.0f;
        EdgeCell *cur = adj->L[i].head;

        // Accumulate outgoing probabilities for vertex i
        while (cur != NULL) {
            sum += cur->p;
            cur = cur->next;
        }

        // Check if the total sum is within the allowed tolerance
        if (sum < lo || sum > hi) {
            res.is_markov = 0;
            res.bad_count++;
        }
    }

    return res;
}

void markov_report(const AdjList *adj, float lo, float hi) {
    // Basic input validation
    if (adj == NULL || adj->n <= 0 || lo > hi) {
        printf("[markov_report] Invalid arguments.\n");
        return;
    }

    // Report header
    printf("---- Markov Verification Report ----\n");
    printf("Tolerance: [%.3f, %.3f]\n", lo, hi);

    // Iterate over all vertices
    for (int i = 0; i < adj->n; ++i) {
        float sum = 0.0f;                // accumulator for outgoing probabilities
        EdgeCell *cur = adj->L[i].head;  // pointer to the first edge of vertex i

        // Traverse adjacency list and accumulate probabilities
        while (cur != NULL) {
            sum += cur->p;
            cur = cur->next;
        }

        // Check if the sum is within the valid tolerance range
        if (sum < lo || sum > hi) {
            printf("Vertex %d: sum = %.3f  --> NOT OK\n", i + 1, sum);
        } else {
            printf("Vertex %d: sum = %.3f  --> OK\n", i + 1, sum);
        }
    }
}