# =============================================================================
# MatBench — Parallel Matrix Multiplication Benchmark
# Compiler: GCC on Linux x86-64, C11 standard
# =============================================================================

CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -O2 -Iinclude
LDFLAGS  = -lpthread -lm

# Optional BLAS support: make USE_BLAS=1
ifdef USE_BLAS
CFLAGS  += -DUSE_BLAS
LDFLAGS += -lcblas
endif

# Directories
SRC_DIR  = src
TEST_DIR = tests
BUILD    = build

# Source files (excluding main.c for test/demo targets)
SRCS     = $(SRC_DIR)/matrix.c \
           $(SRC_DIR)/multiply_naive.c \
           $(SRC_DIR)/multiply_ikj.c \
           $(SRC_DIR)/multiply_tiled.c \
           $(SRC_DIR)/multiply_parallel.c \
           $(SRC_DIR)/multiply_combined.c \
           $(SRC_DIR)/multiply_blas.c \
           $(SRC_DIR)/benchmark.c \
           $(SRC_DIR)/verify.c \
           $(SRC_DIR)/csv_writer.c \
           $(SRC_DIR)/runner.c

OBJS      = $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRCS))
MAIN_OBJ  = $(BUILD)/main.o
TEST_OBJS = $(BUILD)/test_main.o $(BUILD)/test_basic.o \
            $(BUILD)/test_methods.o
DEMO_OBJS = $(BUILD)/test_demo_main.o $(BUILD)/test_demo.o \
            $(BUILD)/test_demo_methods.o

TARGET   = matbench
TEST_BIN = matbench_test
DEMO_BIN = matbench_demo

# === Targets =================================================================

.PHONY: all clean test demo benchmark

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TEST_BIN): $(OBJS) $(TEST_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(DEMO_BIN): $(OBJS) $(DEMO_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

test: $(TEST_BIN)
	./$(TEST_BIN)

demo: $(DEMO_BIN)
	./$(DEMO_BIN)

benchmark: $(TARGET)
	mkdir -p results
	./$(TARGET) --sizes 128,256,512,1024 --threads 1,2,4,8 \
	            --verify --csv results/benchmark.csv
	@echo "Results saved to results/benchmark.csv"

# === Compilation rules =======================================================

$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/test_%.o: $(TEST_DIR)/test_%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD) $(TARGET) $(TEST_BIN) $(DEMO_BIN)
