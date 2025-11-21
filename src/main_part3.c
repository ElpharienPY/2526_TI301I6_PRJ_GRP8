#include <stdio.h>
#include <stdlib.h>
#include "adj_list.h"
#include "matrix.h"

int main(void) {
    char filename[256];

    // 1. Demande du fichier à l'utilisateur (Style Partie 2)
    printf("Enter graph file path: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error: invalid input.\n");
        return EXIT_FAILURE;
    }

    printf("\n--- CHARGEMENT DU GRAPHE : %s ---\n", filename);
    AdjList *adj = adjReadFile(filename);
    if (!adj) {
        fprintf(stderr, "Erreur : Impossible de lire le fichier ou graphe invalide.\n");
        return EXIT_FAILURE;
    }

    // 2. Conversion et Affichage de la Matrice M
    printf("\n--- 1. MATRICE DE TRANSITION M ---\n");
    t_matrix M = adjToMatrix(adj);
    matrixPrint(M);

    int n = M.size;
    t_matrix res = matrixCreate(n);   // Stockera le résultat temporaire
    t_matrix powM = matrixCreate(n);  // Stockera la puissance courante (M^k)

    // Initialisation powM = M
    matrixCopy(powM, M);

    // 3. Calcul M^3 (Prédiction à 3 jours)
    // On multiplie M par elle-même 2 fois de plus (M -> M^2 -> M^3)
    for (int k = 0; k < 2; k++) {
        matrixMultiply(powM, M, res); // res = powM * M
        matrixCopy(powM, res);        // powM prend la valeur de res
    }

    printf("--- 2. MATRICE M^3 (Prediction a 3 jours) ---\n");
    matrixPrint(powM);

    // 4. Calcul M^7 (Prédiction à 7 jours)
    // On continue à partir de M^3. Il reste 4 multiplications pour arriver à 7.
    for (int k = 0; k < 4; k++) {
        matrixMultiply(powM, M, res);
        matrixCopy(powM, res);
    }

    printf("--- 3. MATRICE M^7 (Prediction a 7 jours) ---\n");
    matrixPrint(powM);

    // 5. Test de Convergence (Stationnarité)
    printf("--- 4. TEST DE CONVERGENCE ---\n");

    // On repart du début (M) pour être propre
    matrixCopy(powM, M);
    t_matrix prevM = matrixCreate(n); // Pour stocker M^(n-1)

    double epsilon = 0.01; // Critère d'arrêt du sujet
    double diff = 1.0;
    int iter = 1;
    int max_iter = 1000;

    while (diff > epsilon && iter < max_iter) {
        matrixCopy(prevM, powM);       // Sauvegarde état précédent
        matrixMultiply(prevM, M, res); // res = prev * M
        matrixCopy(powM, res);         // Mise à jour

        diff = matrixDiff(powM, prevM);
        iter++;
    }

    if (iter >= max_iter) {
        printf("Pas de convergence atteinte apres %d iterations (Graphe periodique ?)\n", max_iter);
    } else {
        printf("Convergence atteinte a n = %d (Diff = %g)\n", iter, diff);
        printf("Distribution stationnaire (lignes identiques ?) :\n");
        matrixPrint(powM);
    }

    // Nettoyage mémoire
    matrixFree(&M);
    matrixFree(&res);
    matrixFree(&powM);
    matrixFree(&prevM);
    adjFree(adj);

    return EXIT_SUCCESS;
}