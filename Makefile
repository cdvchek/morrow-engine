# === CONFIGURATION ===
CC := clang
CFLAGS := -g -Wall -Werror
LDFLAGS := -luser32
INCLUDES := -Isrc
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
TARGET := $(BIN_DIR)/morrow.exe

# === GATHER SOURCES AND OBJECTS ===
SRCS := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c) $(wildcard $(SRC_DIR)/*/*/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# === DEFAULT RULE ===
.PHONY: all
all: scaffold $(TARGET)

# === CREATE FOLDERS ===
.PHONY: scaffold
scaffold:
	if not exist obj mkdir obj
	if not exist bin mkdir bin


# === COMPILE OBJECTS ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# === LINK FINAL EXECUTABLE ===
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# === CLEAN ===
.PHONY: clean
clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)