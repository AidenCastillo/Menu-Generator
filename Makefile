# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Project name
TARGET = menu-generator

# Directories
SRC_DIR = src
OBJ_DIR = obj
EXP_DIR = $(SRC_DIR)/exp

# Source and object files
SRCS = $(SRC_DIR)/main.c \
		$(SRC_DIR)/menu.c \
		$(SRC_DIR)/parser.c \
		$(SRC_DIR)/actions.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Experimental executables
EXP_SRCS = $(wildcard $(EXP_DIR)/*.c)
EXP_TARGETS = $(EXP_SRCS:$(EXP_DIR)/%.c=./%)

# Default target
all: $(TARGET) $(EXP_TARGETS)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile experimental files as separate executables
./% : $(EXP_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Run the program (builds first if needed)
run: all
	./$(TARGET)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(EXP_TARGETS)

# Phony targets
.PHONY: all run clean
