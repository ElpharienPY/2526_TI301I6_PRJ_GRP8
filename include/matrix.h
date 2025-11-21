//
// Created by alexj on 19/11/2025.
//
#ifndef MATRIX_H
#define MATRIX_H

#include "adj_list.h"

typedef struct {
    int size;       // Dimension N (Matrice N x N)
    double **data;  // Tableau 2D
} t_matrix;

t_matrix matrixCreate(int n);

void matrixFree(t_matrix *mat);

void matrixPrint(t_matrix mat);

t_matrix adjToMatrix(const AdjList *adj);

void matrixCopy(t_matrix dest, t_matrix src);

void matrixMultiply(t_matrix A, t_matrix B, t_matrix result);

double matrixDiff(t_matrix A, t_matrix B);

#endif // MATRIX_H