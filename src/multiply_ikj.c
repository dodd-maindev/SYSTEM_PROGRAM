/**
 * @file multiply_ikj.c
 * @brief Loop-swapped (i,k,j) multiplication — improved cache locality.
 *
 * By swapping j and k loops, both A and B are accessed with stride-1
 * (row-major), dramatically reducing cache misses compared to naive.
 */
#include "multiply.h"

void multiply_ikj(const Matrix *a, const Matrix *b, Matrix *c) {
    int n = a->rows;
    matrix_fill_zero(c);

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double a_ik = a->data[i * n + k];
            for (int j = 0; j < n; j++) {
                c->data[i * n + j] += a_ik * b->data[k * n + j];
            }
        }
    }
}
