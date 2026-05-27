/**
 * @file multiply_parallel.c
 * @brief Threaded row-wise decomposition using pthreads.
 *
 * Each thread computes a contiguous stripe of rows in C.
 * Uses (i,k,j) loop order within each stripe for cache friendliness.
 */
#include "multiply.h"

#include <pthread.h>

/** Thread argument for row-stripe multiplication. */
typedef struct {
    const Matrix *a;
    const Matrix *b;
    Matrix *c;
    int start_row;
    int end_row;
} ParallelArgs;

/** Worker function: multiplies rows [start_row, end_row). */
static void *parallel_worker(void *arg) {
    ParallelArgs *p = (ParallelArgs *)arg;
    int n = p->a->cols;

    for (int i = p->start_row; i < p->end_row; i++) {
        for (int k = 0; k < n; k++) {
            double a_ik = p->a->data[i * n + k];
            for (int j = 0; j < n; j++) {
                p->c->data[i * n + j] += a_ik * p->b->data[k * n + j];
            }
        }
    }
    return NULL;
}

void multiply_parallel(const Matrix *a, const Matrix *b, Matrix *c,
                       int num_threads) {
    int n = a->rows;
    matrix_fill_zero(c);

    pthread_t threads[num_threads];
    ParallelArgs args[num_threads];
    int rows_per_thread = n / num_threads;

    for (int t = 0; t < num_threads; t++) {
        args[t].a = a;
        args[t].b = b;
        args[t].c = c;
        args[t].start_row = t * rows_per_thread;
        args[t].end_row = (t == num_threads - 1) ? n
                                                  : (t + 1) * rows_per_thread;
        pthread_create(&threads[t], NULL, parallel_worker, &args[t]);
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }
}
