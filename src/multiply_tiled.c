/**
 * @file multiply_tiled.c
 * @brief Cache-blocked (tiled) matrix multiplication.
 *
 * Divides matrices into block_size x block_size tiles to maximize
 * data reuse within L1/L2 cache. Uses (i,k,j) order within tiles.
 */
#include "multiply.h"

/**
 * Multiply a single tile block.
 * Accumulates C[ii..ii+bs, jj..jj+bs] += A[ii..., kk...] * B[kk..., jj...].
 */
static void multiply_tile(const double *a, const double *b, double *c,
                           int ii, int jj, int kk, int bs, int n) {
    int i_end = (ii + bs < n) ? ii + bs : n;
    int j_end = (jj + bs < n) ? jj + bs : n;
    int k_end = (kk + bs < n) ? kk + bs : n;

    for (int i = ii; i < i_end; i++) {
        for (int k = kk; k < k_end; k++) {
            double a_ik = a[i * n + k];
            for (int j = jj; j < j_end; j++) {
                c[i * n + j] += a_ik * b[k * n + j];
            }
        }
    }
}

void multiply_tiled(const Matrix *a, const Matrix *b, Matrix *c,
                    int block_size) {
    int n = a->rows;
    matrix_fill_zero(c);

    for (int ii = 0; ii < n; ii += block_size) {
        for (int kk = 0; kk < n; kk += block_size) {
            for (int jj = 0; jj < n; jj += block_size) {
                multiply_tile(a->data, b->data, c->data,
                              ii, jj, kk, block_size, n);
            }
        }
    }
}
