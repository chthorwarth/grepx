# ==========================================
# Settings & Variables
# ==========================================
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I$(SRC_DIR) -MMD -MP

SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build
BIN_DIR = bin

TARGET = grepx

# ==========================================
# File Discovery
# ==========================================
SRC := $(wildcard $(SRC_DIR)/*.c)
SRC_NO_MAIN := $(filter-out $(SRC_DIR)/main.c,$(SRC))

OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)
OBJ_NO_MAIN := $(SRC_NO_MAIN:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)

TEST_SRC := $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJ := $(TEST_SRC:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%.o)
TEST_BIN := $(TEST_SRC:$(TEST_DIR)/%.c=$(BIN_DIR)/%)

DEPS := $(OBJ:.o=.d) $(TEST_OBJ:.o=.d)

# ==========================================
# Targets
# ==========================================
.PHONY: all clean tests test directories

all: directories $(BIN_DIR)/$(TARGET)

directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)/src
	@mkdir -p $(BUILD_DIR)/test

# --- Production Build ---

$(BIN_DIR)/$(TARGET): $(OBJ)
	@echo "Linking: $@"
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# --- Test Build ---

tests: directories $(TEST_BIN)

# Special rule: test_thread_handler must exclude search.o to avoid symbol conflicts
THREAD_TEST_OBJS = $(filter-out $(BUILD_DIR)/src/search.o, $(OBJ_NO_MAIN))

$(BIN_DIR)/test_thread_handler: $(BUILD_DIR)/test/test_thread_handler.o $(THREAD_TEST_OBJS)
	@echo "Linking Special Test: $@"
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule for all other tests
$(BIN_DIR)/%: $(BUILD_DIR)/test/%.o $(OBJ_NO_MAIN)
	@echo "Linking Test: $@"
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c
	@echo "Compiling Test: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# --- Execution & Cleanup ---

test: all tests
	@echo "\n=== Running Tests ==="
	@for t in $(TEST_BIN); do \
		echo "-> Executing $$t"; \
		./$$t || exit 1; \
	done
	@echo "=== Tests finished ==="

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Include dependency files
-include $(DEPS)