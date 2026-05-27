/**
 * @file multiply_combined.c
 * @brief Combined threaded + tiled multiplication (final optimized version).
 *
 * Each thread processes a stripe of rows using cache-blocked tiling.
 * This combines thread-level parallelism with cache optimization.
 */
#include "multiply.h"

#include <pthread.h>

/** Thread argument for combined threaded-tiled multiplication. */
typedef struct {
    const Matrix *a;
    const Matrix *b;
    Matrix *c;
    int start_row;
    int end_row;
    int block_size;
} CombinedArgs;

/** Worker: tiled multiplication on the assigned row stripe. */
static void *combined_worker(void *arg) {
    CombinedArgs *p = (CombinedArgs *)arg;
    int n = p->a->cols;
    int bs = p->block_size;

    for (int ii = p->start_row; ii < p->end_row; ii += bs) {
        int i_end = (ii + bs < p->end_row) ? ii + bs : p->end_row;
        for (int kk = 0; kk < n; kk += bs) {
            int k_end = (kk + bs < n) ? kk + bs : n;
            for (int jj = 0; jj < n; jj += bs) {
                int j_end = (jj + bs < n) ? jj + bs : n;
                for (int i = ii; i < i_end; i++) {
                    for (int k = kk; k < k_end; k++) {
                        double a_ik = p->a->data[i * n + k];
                        for (int j = jj; j < j_end; j++) {
                            p->c->data[i * n + j] +=
                                a_ik * p->b->data[k * n + j];
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

void multiply_combined(const Matrix *a, const Matrix *b, Matrix *c,
                       int num_threads, int block_size) {
    int n = a->rows;
    matrix_fill_zero(c);

    pthread_t threads[num_threads];
    CombinedArgs args[num_threads];
    int rows_per_thread = n / num_threads;

    for (int t = 0; t < num_threads; t++) {
        args[t].a = a;
        args[t].b = b;
        args[t].c = c;
        args[t].start_row = t * rows_per_thread;
        args[t].end_row = (t == num_threads - 1) ? n
                                                  : (t + 1) * rows_per_thread;
        args[t].block_size = block_size;
        pthread_create(&threads[t], NULL, combined_worker, &args[t]);
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }
}
