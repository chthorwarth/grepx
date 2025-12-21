# ========================
# Compiler und Flags
# ========================
CC = gcc

SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build
BIN_DIR = bin

CFLAGS = -std=c11 -Wall -Wextra -I$(SRC_DIR)

TARGET = grepx

# ========================
# Quellcode
# ========================
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/files.c \
      $(SRC_DIR)/search.c \
      $(SRC_DIR)/queue.c

SRC_NO_MAIN = $(filter-out $(SRC_DIR)/main.c,$(SRC))
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)
OBJ_NO_MAIN = $(SRC_NO_MAIN:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)

# ========================
# Testdateien
# ========================
TEST_SRC = $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%.o)
TEST_BIN = $(TEST_SRC:$(TEST_DIR)/%.c=$(BIN_DIR)/%)

# ========================
# Verzeichnisse
# ========================
DIRS = $(SRC_DIR) $(TEST_DIR) $(BUILD_DIR)/src $(BUILD_DIR)/test $(BIN_DIR)

directories:
	mkdir -p $(DIRS)

# ========================
# Produktionscode bauen
# ========================
all: directories $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ========================
# Tests bauen
# ========================
tests: directories $(TEST_BIN)

$(BIN_DIR)/%: $(BUILD_DIR)/test/%.o $(OBJ_NO_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ========================
# Testlauf
# ========================
test: tests
	@echo "=== Running Tests ==="
	@for t in $(TEST_BIN); do \
		echo "-> Running $$t"; \
		$$t; \
	done
	@echo "=== Tests finished ==="

# ========================
# Aufräumen
# ========================
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
