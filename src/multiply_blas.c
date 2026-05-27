/**
 * @file multiply_blas.c
 * @brief BLAS dgemm wrapper for professional-grade comparison.
 *
 * Conditionally compiled with -DUSE_BLAS and linked against libcblas.
 * Without USE_BLAS, prints a warning and returns without computation.
 */
#include "multiply.h"

#include <stdio.h>

#ifdef USE_BLAS
#include <cblas.h>

void multiply_blas(const Matrix *a, const Matrix *b, Matrix *c) {
    int n = a->rows;

    /* C = 1.0 * A * B + 0.0 * C */
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                n, n, n,
                1.0, a->data, n,
                b->data, n,
                0.0, c->data, n);
}

#else

void multiply_blas(const Matrix *a, const Matrix *b, Matrix *c) {
    (void)a;
    (void)b;
    (void)c;
    fprintf(stderr, "[WARN] BLAS not available. "
                    "Recompile with: make USE_BLAS=1\n");
}

#endif /* USE_BLAS */
