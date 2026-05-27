/**
 * @file csv_writer.h
 * @brief CSV file writer for benchmark results export.
 */
#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <stdio.h>

/** CSV writer handle wrapping a FILE pointer. */
typedef struct {
    FILE *file;
} CsvWriter;

/** Open a CSV file and write the header row. Returns NULL on failure. */
CsvWriter *csv_writer_open(const char *filepath);

/** Write one benchmark result row to the CSV file. */
void csv_writer_add_row(CsvWriter *writer, const char *method,
                        int matrix_size, int threads, int block_size,
                        double elapsed, double gflops, double speedup);

/** Close the CSV file and free the writer. */
void csv_writer_close(CsvWriter *writer);

#endif /* CSV_WRITER_H */
