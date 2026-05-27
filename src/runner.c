/**
 * @file runner.c
 * @brief Benchmark runner — executes all methods for each matrix size.
 */
#include "runner.h"

#include "benchmark.h"
#include "matrix.h"
#include "multiply.h"
#include "verify.h"

#include <stdio.h>

/** Run a single method variant and record the result. */
static void run_single(const char *name, const Matrix *ref,
                       const Matrix *result, double elapsed,
                       int n, int threads, int block_size,
                       double baseline, int do_verify, CsvWriter *csv) {
    double gflops = benchmark_calc_gflops(n, elapsed);
    double speedup = (baseline > 0.0) ? baseline / elapsed : 1.0;

    benchmark_print_result(name, n, threads, elapsed, gflops);

    if (do_verify && !verify_matrices(ref, result, VERIFY_TOLERANCE)) {
        fprintf(stderr, "  >> %s FAILED verification!\n", name);
    }
    if (csv) {
        csv_writer_add_row(csv, name, n, threads, block_size,
                           elapsed, gflops, speedup);
    }
}

/** Benchmark all methods for a single matrix size. */
static void run_for_size(int n, const RunnerConfig *cfg, double *baseline) {
    Matrix *a = matrix_create(n, n);
    Matrix *b = matrix_create(n, n);
    Matrix *ref = matrix_create(n, n);
    Matrix *c = matrix_create(n, n);

    matrix_fill_random(a, 42);
    matrix_fill_random(b, 123);

    printf("\n=== Matrix Size: %d x %d ===\n", n, n);
    benchmark_print_header();
    benchmark_print_separator();

    /* 1. Naive baseline */
    double t0 = benchmark_get_time();
    multiply_naive(a, b, ref);
    double t1 = benchmark_get_time();
    *baseline = t1 - t0;
    run_single("naive_ijk", ref, ref, *baseline,
               n, 1, 0, *baseline, 0, cfg->csv);

    /* 2. Loop-swapped (i,k,j) */
    t0 = benchmark_get_time();
    multiply_ikj(a, b, c);
    t1 = benchmark_get_time();
    run_single("ikj_swapped", ref, c, t1 - t0,
               n, 1, 0, *baseline, cfg->run_verify, cfg->csv);

    /* 3. Cache-blocked (tiled) */
    t0 = benchmark_get_time();
    multiply_tiled(a, b, c, cfg->block_size);
    t1 = benchmark_get_time();
    run_single("tiled", ref, c, t1 - t0,
               n, 1, cfg->block_size, *baseline, cfg->run_verify, cfg->csv);

    /* 4. Threaded + 5. Combined — for each thread count */
    for (int ti = 0; ti < cfg->num_thread_counts; ti++) {
        int thr = cfg->thread_counts[ti];
        char label[64];

        snprintf(label, sizeof(label), "parallel_%dt", thr);
        t0 = benchmark_get_time();
        multiply_parallel(a, b, c, thr);
        t1 = benchmark_get_time();
        run_single(label, ref, c, t1 - t0,
                   n, thr, 0, *baseline, cfg->run_verify, cfg->csv);

        snprintf(label, sizeof(label), "combined_%dt", thr);
        t0 = benchmark_get_time();
        multiply_combined(a, b, c, thr, cfg->block_size);
        t1 = benchmark_get_time();
        run_single(label, ref, c, t1 - t0,
                   n, thr, cfg->block_size, *baseline,
                   cfg->run_verify, cfg->csv);
    }

    matrix_free(a);
    matrix_free(b);
    matrix_free(ref);
    matrix_free(c);
}

void runner_execute(const RunnerConfig *config) {
    for (int i = 0; i < config->num_sizes; i++) {
        double baseline = 0.0;
        run_for_size(config->sizes[i], config, &baseline);
    }
}
