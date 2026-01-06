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

all: directories $(BIN_DIR)/$(TARGET)

directories:
	mkdir -p $(DIRS)

# ========================
# Quellcode
# ========================
SRC := $(wildcard $(SRC_DIR)/*.c)
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

# ========================
# Produktionscode bauen
# ========================
$(BIN_DIR)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ========================
# Tests bauen
# ========================
tests: directories $(TEST_BIN)

# normale Tests (alle Objektdateien)
$(BIN_DIR)/%: $(BUILD_DIR)/test/%.o $(OBJ_NO_MAIN)
	# Spezialregel: thread_handler-Tests dürfen search.o NICHT linken
	if [ "$@" = "$(BIN_DIR)/test_thread_handler" ]; then \
		$(CC) $(CFLAGS) -o $@ $< $(filter-out $(BUILD_DIR)/src/search.o,$(OBJ_NO_MAIN)); \
	else \
		$(CC) $(CFLAGS) -o $@ $^; \
	fi

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
