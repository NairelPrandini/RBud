# Compiler and linker
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include
RES_DIR = res

# Output executable
TARGET = $(BUILD_DIR)/game

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Resource files
RES_FILES = $(wildcard $(RES_DIR)/*)

# Compiler flags
CFLAGS = -I$(INCLUDE_DIR) -Wall -std=c99

# Linker flags
LDFLAGS = -lraylib -lgdi32 -lwinmm -ldwmapi -lpthread

# Default target
all: $(TARGET) copy-res clean-objects

# Copy resource files
copy-res:
	@mkdir -p $(BUILD_DIR)
	cp -r $(RES_DIR) $(BUILD_DIR)

# Build target
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build and binary files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Clean up object files
clean-objects:
	rm -f $(BUILD_DIR)/*.o

# Phony targets
.PHONY: all clean copy-res clean-objects
