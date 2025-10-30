/* This file is 100% created by chatgpt to test all adj_list function */


#include <stdio.h>
#include <stdlib.h>
#include "adj_list.h"


int main(void) {

    printf("=== TEST 1 : Création manuelle ===\n");

    // Création d'une liste d'adjacence vide pour 3 sommets
    AdjList *adj = adj_create(3);
    if (!adj) {
        fprintf(stderr, "Erreur : adj_create a échoué\n");
        return EXIT_FAILURE;
    }

    // Ajout manuel d'arêtes
    adj_add(adj, 0, 1, 0.6f);
    adj_add(adj, 0, 2, 0.4f);
    adj_add(adj, 1, 0, 1.0f);
    adj_add(adj, 2, 2, 1.0f);

    // Affichage
    printf("Graphe créé manuellement :\n");
    adj_print(adj);

    // Libération mémoire
    adj_free(adj);
    printf("Mémoire libérée.\n\n");

    // ================================
    printf("=== TEST 2 : Lecture depuis fichier ===\n");

    const char *filename = "../data/example1.txt";
    AdjList *file_graph = adj_read_file(filename);

    if (!file_graph) {
        fprintf(stderr, "Erreur : lecture du fichier %s\n", filename);
        return EXIT_FAILURE;
    }

    printf("Contenu du graphe lu :\n");
    adj_print(file_graph);

    adj_free(file_graph);
    printf("Mémoire libérée (lecture fichier).\n");

    printf("\n=== Tous les tests terminés avec succès ===\n");
    return EXIT_SUCCESS;
}
