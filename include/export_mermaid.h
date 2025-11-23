#ifndef EXPORT_MERMAID_H
#define EXPORT_MERMAID_H

#include  <stddef.h>
#include "adj_list.h"

/* ---------- Constants (Mermaid boilerplate) ---------- */

#define MMD_CONFIG_HEADER \
"---\n" \
"config:\n"\
"   layout: elk\n"        \
"   theme: neo\n"         \
"   look: neo\n"          \
"---\n\n"

#define MMD_FLOWCHART_HEADER "flowchart LR\n"

#define NODE_ID_MAX 8

/* ---------- Return codes ---------- */
typedef enum {
    MMD_OK = 0,
    MMD_ERR_MARKOV = 1,
    MMD_ERR_FILE = 2,
} MermaidStatus;

/* --------- API ---------- */

//generates a text id for mermaid from an integer index
size_t nodeId(int idx1, char *out, size_t out_cap);

//exports the graph to a .mmd file
int writeMermaid(const AdjList *adj, const char *filepath);

#endif //EXPORT_MERMAID_H
