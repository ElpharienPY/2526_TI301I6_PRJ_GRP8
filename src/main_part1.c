#include <stdio.h>
#include <stdlib.h>

#include "adj_list.h"
#include "export_mermaid.h"
#include "markov_check.h"

static void print_usage(const char *prog) {
    fprintf(stderr,"Usage:\n"
            "  %s <input.txt> [output.mmd]\n\n"
            "Arguments:\n"
            "  input.txt   Data file (first line: N; then lines 'u v p').\n"
            "  output.mmd  (optional) Mermaid output file. Default: graph.mmd\n",
            prog);
}

int main(int argc, char **argv ) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 2; // error file/allocation
    }

    const char *in_path = argv[1];
    const char *out_path = argv[2];

    AdjList *adj = adj_read_file(in_path);
    if (adj == NULL) {
        fprintf(stderr, "Error reading graph\n");
        return 2;
    }

    // Step 1 (validation) — Display adjacency list

    printf("=== Adjacency List (%d vertices) ===\n", adj->n);
    adj_print(adj);

    // Step 2 — Check Markov property with tolerance [0.99 ; 1.00]

    const float LO = 0.99f;
    const float HI = 1.00f;

    markov_report(adj, LO, HI);

    MarkovResult result = markov_is_valid(adj, LO, HI);
    if (!result.is_markov) {
        fprintf(stderr, "Error validating graph\n");
        printf("Vertices failing check: %d\n", result.bad_count);
        adj_free(adj);
        return 1; // 1 because it's a non-markov graph
    }

    // Step 3 — Export Mermaid

    int rc = write_mermaid(adj, out_path);
    if (rc != 0) {
        fprintf(stderr, "Error writing graph\n");
        adj_free(adj);
        return 2;
    }

    printf("Vertices failing check: %d\n", result.bad_count);

    // Step 4 - Cleanup
    adj_free(adj);
    return 0;
}
