# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Project name
TARGET = menu-generator

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source and object files
SRCS = $(SRC_DIR)/main.c \
		$(SRC_DIR)/menu.c \
		$(SRC_DIR)/parser.c \
		$(SRC_DIR)/actions.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Run the program (builds first if needed)
run: all
	./$(TARGET) config/menu.cfg

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony targets
.PHONY: all run clean
