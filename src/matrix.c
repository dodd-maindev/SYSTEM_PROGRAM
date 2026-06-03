/**
 * @file matrix.c
 * @brief Matrix allocation, deallocation, and initialization.
 */
#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix *matrix_create(int rows, int cols) {
    Matrix *mat = malloc(sizeof(Matrix));
    if (!mat) {
        return NULL;
    }

    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc((size_t)rows * cols * sizeof(double));

    if (!mat->data) {
        free(mat);
        return NULL;
    }

    return mat;
}

void matrix_free(Matrix *mat) {
    if (!mat) {
        return;
    }
    free(mat->data);
    free(mat);
}

void matrix_fill_random(Matrix *mat, unsigned int seed) {
    srand(seed);
    int total = mat->rows * mat->cols;

    for (int i = 0; i < total; i++) {
        mat->data[i] = (double)rand() / RAND_MAX;
    }
}

void matrix_fill_zero(Matrix *mat) {
    size_t bytes = (size_t)mat->rows * mat->cols * sizeof(double);
    memset(mat->data, 0, bytes);
}

void matrix_print(const Matrix *mat, const char *label) {
    printf("  %s (%dx%d):\n", label, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        printf("    [");
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf(" ]\n");
    }
}

