/**
 * @file csv_writer.c
 * @brief CSV writer implementation for benchmark result export.
 */
#include "csv_writer.h"

#include <stdlib.h>

CsvWriter *csv_writer_open(const char *filepath) {
    CsvWriter *writer = malloc(sizeof(CsvWriter));
    if (!writer) {
        return NULL;
    }

    writer->file = fopen(filepath, "w");
    if (!writer->file) {
        free(writer);
        return NULL;
    }

    /* Write CSV header */
    fprintf(writer->file,
            "method,matrix_size,threads,block_size,"
            "elapsed_sec,gflops,speedup\n");

    return writer;
}

void csv_writer_add_row(CsvWriter *writer, const char *method,
                        int matrix_size, int threads, int block_size,
                        double elapsed, double gflops, double speedup) {
    if (!writer || !writer->file) {
        return;
    }

    fprintf(writer->file, "%s,%d,%d,%d,%.6f,%.4f,%.2f\n",
            method, matrix_size, threads, block_size,
            elapsed, gflops, speedup);
}

void csv_writer_close(CsvWriter *writer) {
    if (!writer) {
        return;
    }
    if (writer->file) {
        fclose(writer->file);
    }
    free(writer);
}
