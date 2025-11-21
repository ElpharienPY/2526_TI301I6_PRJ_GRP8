//
// Created by alexj on 19/11/2025.
//
#ifndef MATRIX_H
#define MATRIX_H

#include "adj_list.h"
#include "partition.h"

/* Square matrix of size N x N */
typedef struct s_matrix {
    int size;        // Dimension N (matrix is size x size)
    double **data;   // 2D array: data[i][j]
} t_matrix;

/* Creation / destruction */
t_matrix matrixCreate(int n);
void     matrixFree(t_matrix *mat);

/* Display */
void     matrixPrint(t_matrix mat);

/* Build transition matrix from adjacency list */
t_matrix adjToMatrix(const AdjList *adj);

/* Basic operations */
void     matrixCopy(t_matrix dest, t_matrix src);
void     matrixMultiply(t_matrix A, t_matrix B, t_matrix result);
double   matrixDiff(t_matrix A, t_matrix B);

/* Part 3 – Step 2: submatrix for one class (component) */
t_matrix subMatrix(t_matrix matrix, Partition part, int compo_index);

/* Bonus: period of a class */
int      getPeriod(t_matrix sub_matrix);

#endif // MATRIX_H
