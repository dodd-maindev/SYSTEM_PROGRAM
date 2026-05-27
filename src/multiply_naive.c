/**
 * @file multiply_naive.c
 * @brief Naive (i,j,k) matrix multiplication — cache-unfriendly baseline.
 *
 * This is the textbook triple-loop with column-striding access on B,
 * resulting in poor spatial locality and high cache miss rate.
 */
#include "multiply.h"

void multiply_naive(const Matrix *a, const Matrix *b, Matrix *c) {
    int n = a->rows;
    matrix_fill_zero(c);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += a->data[i * n + k] * b->data[k * n + j];
            }
            c->data[i * n + j] = sum;
        }
    }
}
