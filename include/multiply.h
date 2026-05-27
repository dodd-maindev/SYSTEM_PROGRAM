/**
 * @file multiply.h
 * @brief Matrix multiplication method declarations.
 *
 * Provides 6 implementations from naive baseline to BLAS-optimized,
 * each demonstrating a different optimization technique.
 */
#ifndef MULTIPLY_H
#define MULTIPLY_H

#include "matrix.h"

/** Naive (i,j,k) triple-loop multiplication: C = A * B. */
void multiply_naive(const Matrix *a, const Matrix *b, Matrix *c);

/** Loop-swapped (i,k,j) multiplication: C = A * B. */
void multiply_ikj(const Matrix *a, const Matrix *b, Matrix *c);

/** Cache-blocked (tiled) multiplication: C = A * B. */
void multiply_tiled(const Matrix *a, const Matrix *b, Matrix *c,
                    int block_size);

/** Threaded row-wise decomposition: C = A * B. */
void multiply_parallel(const Matrix *a, const Matrix *b, Matrix *c,
                       int num_threads);

/** Combined threaded + tiled: C = A * B (final optimized version). */
void multiply_combined(const Matrix *a, const Matrix *b, Matrix *c,
                       int num_threads, int block_size);

/** BLAS dgemm wrapper: C = A * B. Requires compile with USE_BLAS=1. */
void multiply_blas(const Matrix *a, const Matrix *b, Matrix *c);

#endif /* MULTIPLY_H */
