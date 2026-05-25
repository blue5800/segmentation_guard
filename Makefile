CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -g -I$(SRC_DIR) -I$(XED_INCLUDE) -pthread
LDFLAGS = -L$(OUT_DIR) -lsegmentation_guard -Wl,-rpath='$$ORIGIN/../'

SRC_DIR = src
OUT_DIR = out
EXAMPLES_DIR = examples

XED_DIR = xed
XED_KIT_DIR = xed_kit
XED_INCLUDE = $(XED_KIT_DIR)/include
XED_LIB_DIR = $(XED_KIT_DIR)/lib
XED_LIB = $(XED_LIB_DIR)/libxed.a

# Source files
SRCS = $(SRC_DIR)/segmentation_guard.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)
EXAMPLE_SRCS = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_EXES = $(EXAMPLE_SRCS:$(EXAMPLES_DIR)/%.c=$(OUT_DIR)/examples/%)

# Default target
all: shared static examples

# Ensure output directories exist
$(OUT_DIR) $(OUT_DIR)/examples:
	mkdir -p $@

# Build XED
$(XED_LIB):
	@echo "Building XED..."
	cd $(XED_DIR) && python3 mfile.py --static --extra-ccflags="-fPIC" --install-dir=$(abspath $(XED_KIT_DIR)) install

# Compile objects
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c $(XED_LIB) | $(OUT_DIR)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Static Library
static: $(OUT_DIR)/libsegmentation_guard.a

$(OUT_DIR)/libsegmentation_guard.a: $(OBJS)
	$(AR) rcs $@ $^

# Dynamic Library
shared: $(OUT_DIR)/libsegmentation_guard.so

$(OUT_DIR)/libsegmentation_guard.so: $(OBJS) $(XED_LIB)
	$(CC) -shared -o $@ $^

# Examples
examples: $(EXAMPLE_EXES)

$(OUT_DIR)/examples/%: $(EXAMPLES_DIR)/%.c shared | $(OUT_DIR)/examples
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(OUT_DIR)
	rm -rf $(XED_KIT_DIR)
	cd $(XED_DIR) && python3 mfile.py --clean

.PHONY: all static shared examples clean
