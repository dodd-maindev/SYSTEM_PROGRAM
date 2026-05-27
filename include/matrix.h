/**
 * @file matrix.h
 * @brief Dense matrix data structure and basic operations.
 */
#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

/**
 * Dense matrix stored in row-major layout.
 * Access pattern: data[row * cols + col].
 */
typedef struct {
    int rows;
    int cols;
    double *data;
} Matrix;

/** Allocate a rows x cols matrix. Returns NULL on failure. */
Matrix *matrix_create(int rows, int cols);

/** Free matrix memory. Safe to call with NULL. */
void matrix_free(Matrix *mat);

/** Fill matrix with pseudo-random values in [0, 1). */
void matrix_fill_random(Matrix *mat, unsigned int seed);

/** Fill matrix with zeros. */
void matrix_fill_zero(Matrix *mat);

/** Element read access: mat[row][col]. */
static inline double matrix_get(const Matrix *m, int r, int c) {
    return m->data[r * m->cols + c];
}

/** Element write access: mat[row][col] = val. */
static inline void matrix_set(Matrix *m, int r, int c, double v) {
    m->data[r * m->cols + c] = v;
}

#endif /* MATRIX_H */
