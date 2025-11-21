//
// Created by alexj on 19/11/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "partition.h"

/* Crée une matrice vide (remplie de 0.0) */
t_matrix matrixCreate(int n) {
    t_matrix mat;
    mat.size = n;

    if (n <= 0) {
        mat.data = NULL;
        return mat;
    }

    // Allocation des lignes
    mat.data = (double **)malloc(n * sizeof(double *));
    if (mat.data == NULL) {
        perror("Erreur allocation matrice");
        exit(EXIT_FAILURE);
    }

    // Allocation des colonnes
    for (int i = 0; i < n; i++) {
        mat.data[i] = (double *)calloc(n, sizeof(double)); // calloc met à 0
        if (mat.data[i] == NULL) {
            perror("Erreur allocation ligne matrice");
            exit(EXIT_FAILURE);
        }
    }
    return mat;
}

/* Libération mémoire */
void matrixFree(t_matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;

    for (int i = 0; i < mat->size; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    mat->data = NULL;
    mat->size = 0;
}

/* Conversion AdjList -> Matrix */
t_matrix adjToMatrix(const AdjList *adj) {
    if (adj == NULL) {
        t_matrix empty = {0, NULL};
        return empty;
    }

    int n = adj->n;
    t_matrix mat = matrixCreate(n);

    // Parcours de chaque sommet i (source)
    for (int i = 0; i < n; i++) {
        // On récupère la tête de liste pour le sommet i
        EdgeCell *curr = adj->L[i].head;

        while (curr != NULL) {
            int j = curr->v; // Destination (déjà 0-based grâce à adjReadFile)
            float p = curr->p;

            // Sécurité bornes
            if (j >= 0 && j < n) {
                mat.data[i][j] = (double)p;
            }

            curr = curr->next;
        }
    }
    return mat;
}

/* Copie src -> dest */
void matrixCopy(t_matrix dest, t_matrix src) {
    if (dest.size != src.size) {
        fprintf(stderr, "Erreur: Tailles incompatibles pour copie\n");
        return;
    }
    for (int i = 0; i < src.size; i++) {
        for (int j = 0; j < src.size; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

/* Multiplication : result = A * B */
void matrixMultiply(t_matrix A, t_matrix B, t_matrix result) {
    if (A.size != B.size || result.size != A.size) {
        fprintf(stderr, "Erreur: Tailles incompatibles pour multiplication\n");
        return;
    }

    int n = A.size;
    // Reset result à 0 avant calcul
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result.data[i][j] = 0.0;
        }
    }

    // Calcul classique O(N^3)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A.data[i][k] * B.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
}

/* Différence (norme L1 des différences) */
double matrixDiff(t_matrix A, t_matrix B) {
    if (A.size != B.size) return -1.0;

    double diff = 0.0;
    for (int i = 0; i < A.size; i++) {
        for (int j = 0; j < A.size; j++) {
            diff += fabs(A.data[i][j] - B.data[i][j]);
        }
    }
    return diff;
}

/* Affichage propre */
void matrixPrint(t_matrix mat) {
    if (mat.data == NULL) return;

    for (int i = 0; i < mat.size; i++) {
        printf("| ");
        for (int j = 0; j < mat.size; j++) {
            // Affiche 0 si très proche de 0, sinon la valeur
            if (mat.data[i][j] < 0.0001) printf("   .  ");
            else printf("%5.2f ", mat.data[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

/* Sous-matrice pour une classe (composante fortement connexe) */
t_matrix subMatrix(t_matrix matrix, Partition part, int compo_index)
{
    if (compo_index < 0 || compo_index >= part.count) {
        fprintf(stderr, "ERROR: invalid component index %d in subMatrix().\n", compo_index);
        t_matrix empty = {0, NULL};
        return empty;
    }

    Class cls = part.classes[compo_index];
    int k = cls.size;   // nombre de sommets dans cette classe

    t_matrix sub = matrixCreate(k);

    for (int i = 0; i < k; i++) {
        int orig_i = cls.vertices[i] - 1;  // convertit en indice 0-based

        for (int j = 0; j < k; j++) {
            int orig_j = cls.vertices[j] - 1; // idem
            sub.data[i][j] = matrix.data[orig_i][orig_j];
        }
    }

    return sub;
}

/* ---------------- Période d'une classe (bonus) ---------------- */

static int gcd_int(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static int gcd_array(int *vals, int n) {
    if (n <= 0) return 0;
    int g = vals[0];
    for (int i = 1; i < n; i++) {
        g = gcd_int(g, vals[i]);
    }
    return g;
}

int getPeriod(t_matrix sub_matrix)
{
    int n = sub_matrix.size;
    if (n <= 0) return 0;

    int *periods = malloc(n * sizeof(int));
    if (!periods) {
        perror("getPeriod: malloc");
        return 0;
    }
    int pcount = 0;

    t_matrix power = matrixCreate(n);
    t_matrix tmp   = matrixCreate(n);

    matrixCopy(power, sub_matrix);

    /* Pour k = 1..n, on regarde les éléments diagonaux de S^k */
    for (int k = 1; k <= n; k++) {

        int diag_nonzero = 0;
        for (int i = 0; i < n; i++) {
            if (power.data[i][i] > 0.0) {
                diag_nonzero = 1;
                break;
            }
        }

        if (diag_nonzero) {
            periods[pcount++] = k;
        }

        matrixMultiply(power, sub_matrix, tmp);
        matrixCopy(power, tmp);
    }

    int period = gcd_array(periods, pcount);

    free(periods);
    matrixFree(&power);
    matrixFree(&tmp);

    return period;
}
