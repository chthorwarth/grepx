CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

TARGET = grepx

SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/files.c \
      $(SRC_DIR)/search.c

OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: directories $(BIN_DIR)/$(TARGET)

directories:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(BIN_DIR)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

test: all
	./$(BIN_DIR)/$(TARGET)