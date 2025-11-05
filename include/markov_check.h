#ifndef MARKOV_CHECK_H
#define MARKOV_CHECK_H

#include <adj_list.h>

// Structure used to store the result of the Markov verification
typedef struct {
    int is_markov;
    int bad_count;
} MarkovResult;

// Check if each vertex has an outgoing probability sum within [lo, hi]
MarkovResult markov_is_valid (const AdjList * adj, float lo, float hi);

// Display a detailed report showing the sum of probabilities for each vertex
void markov_report(const AdjList * adj, float lo, float hi);

#endif //MARKOV_CHECK_H
