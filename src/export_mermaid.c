#include <stdio.h>
#include <string.h>
#include "export_mermaid.h"

size_t node_id(int idx1, char *out, size_t out_cap) {

    if (out != NULL && out_cap > 0) {
        out[0] = '\0';
    }

    if (out == NULL ||out_cap == 0 || idx1 < 1) {
        return 0;
    }

    long n = (long)idx1 - 1;   // zero-based index for base-26 “Excel” logic
    char tmp[NODE_ID_MAX];     // temporary stack to collect chars in reverse
    size_t len = 0;    // number of chars currently in tmp

    while (n >= 0) {
        int r = (int)(n % 26);
        char c = (char)('A' + r);

        if (len + 1 >= out_cap || len >= (size_t)(NODE_ID_MAX - 1)) {
            return 0;
        }

        tmp[len++] = c;
        n = (n/26) - 1;
    }

    for (size_t i = 0; i < len; ++i) {
        out[i] = tmp[len - 1 - i];
    }
    out[len] = '\0';
    return len;
}

int write_mermaid(const AdjList *adj, const char *filepath) {
    if (adj == NULL || filepath == NULL) {
        return MMD_ERR_FILE;
    }

    FILE *f = fopen(filepath, "wt");
    if (!f) {
        fprintf(stderr, "Error writing graph: cannot open '%s' (%s)\n",
                filepath, strerror(errno));
        return MMD_ERR_FILE;
    }

    // Write Mermaid headers
    if (fputs(MMD_CONFIG_HEADER, f) == EOF) {
        fclose(f);
        return MMD_ERR_FILE;
    }
    if (fputs(MMD_FLOWCHART_HEADER, f) == EOF) {
        fclose(f);
        return MMD_ERR_FILE;
    }

    // ---- Step 2: Write nodes (A((1)), B((2)), ...) ----
    char id[NODE_ID_MAX];
    for (size_t i = 0; i < adj->n; ++i) {
        size_t len = node_id((int)(i + 1), id, NODE_ID_MAX);
        if (len == 0) {
            fclose(f);
            return MMD_ERR_FILE;
        }
        if (fprintf(f, "%s((%d))\n", id, (int)(i + 1)) < 0) {
            fclose(f);
            return MMD_ERR_FILE;
        }
    }

    // ---- Step 3: Write edges (A -->|p| B) ----
    char src_id[NODE_ID_MAX];
    char dst_id[NODE_ID_MAX];

    for (size_t u = 0; u < adj->n; ++u) {
        // Generate source node ID (A, B, C...)
        if (node_id((int)(u + 1), src_id, NODE_ID_MAX) == 0) {
            fclose(f);
            return MMD_ERR_FILE;
        }

        // Traverse adjacency list of vertex u
        EdgeCell *cur = adj->L[u].head;
        while (cur != NULL) {
            // Generate destination node ID
            if (node_id((int)cur->v + 1, dst_id, NODE_ID_MAX) == 0) {
                fclose(f);
                return MMD_ERR_FILE;
            }

            // Write edge: A -->|0.50| B
            if (fprintf(f, "%s -->|%.2f| %s\n", src_id, cur->p, dst_id) < 0) {
                fclose(f);
                return MMD_ERR_FILE;
            }

            cur = cur->next;
        }
    }

    fclose(f);
    return MMD_OK;
}


