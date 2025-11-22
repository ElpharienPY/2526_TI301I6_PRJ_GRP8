#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "partition.h"

/* Create N×N matrix initialized to 0 */
t_matrix matrixCreate(int n) {
    t_matrix mat;
    mat.size = n;

    if (n <= 0) {
        mat.data = NULL;
        return mat;
    }

    mat.data = (double **)malloc(n * sizeof(double *));
    if (!mat.data) {
        perror("alloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        mat.data[i] = (double *)calloc(n, sizeof(double));
        if (!mat.data[i]) {
            perror("alloc row");
            exit(EXIT_FAILURE);
        }
    }
    return mat;
}

/* Free matrix memory */
void matrixFree(t_matrix *mat) {
    if (!mat || !mat->data) return;

    for (int i = 0; i < mat->size; i++) {
        free(mat->data[i]);
    }
    free(mat->data);

    mat->data = NULL;
    mat->size = 0;
}

/* Build transition matrix from adjacency list */
t_matrix adjToMatrix(const AdjList *adj) {
    if (!adj) {
        t_matrix empty = {0, NULL};
        return empty;
    }

    int n = adj->n;
    t_matrix mat = matrixCreate(n);

    for (int i = 0; i < n; i++) {
        EdgeCell *curr = adj->L[i].head;
        while (curr) {
            int j = curr->v;      /* 0-based destination */
            float p = curr->p;
            if (j >= 0 && j < n) {
                mat.data[i][j] = p;
            }
            curr = curr->next;
        }
    }
    return mat;
}

/* Copy src → dest */
void matrixCopy(t_matrix dest, t_matrix src) {
    if (dest.size != src.size) {
        fprintf(stderr, "size mismatch copy\n");
        return;
    }
    for (int i = 0; i < src.size; i++) {
        for (int j = 0; j < src.size; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

/* result = A × B */
void matrixMultiply(t_matrix A, t_matrix B, t_matrix result) {
    if (A.size != B.size || result.size != A.size) {
        fprintf(stderr, "size mismatch mult\n");
        return;
    }

    int n = A.size;

    /* Reset output */
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result.data[i][j] = 0.0;

    /* Standard triple-loop */
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

/* Compute L1 difference between matrices */
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

/* Print matrix */
void matrixPrint(t_matrix mat) {
    if (!mat.data) return;

    for (int i = 0; i < mat.size; i++) {
        printf("| ");
        for (int j = 0; j < mat.size; j++) {
            if (fabs(mat.data[i][j]) < 0.0001) printf("  .   ");
            else printf("%5.2f ", mat.data[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

/* Extract submatrix for class index */
t_matrix subMatrix(t_matrix matrix, Partition part, int compo_index) {
    if (compo_index < 0 || compo_index >= part.count) {
        fprintf(stderr, "bad comp index\n");
        t_matrix empty = {0, NULL};
        return empty;
    }

    Class cls = part.classes[compo_index];
    int k = cls.size;

    t_matrix sub = matrixCreate(k);

    for (int i = 0; i < k; i++) {
        int oi = cls.vertices[i] - 1; /* convert to 0-based */
        for (int j = 0; j < k; j++) {
            int oj = cls.vertices[j] - 1;
            sub.data[i][j] = matrix.data[oi][oj];
        }
    }
    return sub;
}

/* gcd internal helper */
static int gcd_int(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* gcd for array */
static int gcd_array(int *vals, int n) {
    if (n <= 0) return 0;
    int g = vals[0];
    for (int i = 1; i < n; i++)
        g = gcd_int(g, vals[i]);
    return g;
}

/* Compute class period */
int getPeriod(t_matrix sub_matrix) {
    int n = sub_matrix.size;
    if (n <= 0) return 0;

    int *periods = malloc(n * sizeof(int));
    if (!periods) {
        perror("malloc");
        return 0;
    }
    int pcount = 0;

    t_matrix power = matrixCreate(n);
    t_matrix tmp   = matrixCreate(n);

    matrixCopy(power, sub_matrix);

    for (int k = 1; k <= n; k++) {
        int diag_nonzero = 0;
        for (int i = 0; i < n; i++) {
            if (power.data[i][i] > 0.0) {
                diag_nonzero = 1;
                break;
            }
        }
        if (diag_nonzero)
            periods[pcount++] = k;

        matrixMultiply(power, sub_matrix, tmp);
        matrixCopy(power, tmp);
    }

    int period = gcd_array(periods, pcount);

    free(periods);
    matrixFree(&power);
    matrixFree(&tmp);

    return period;
}
