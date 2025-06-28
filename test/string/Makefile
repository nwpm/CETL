# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

# === Paths ===

INCLUDE         = ../../include
STR_BIN_DIR     = ../../bin/string
BUILD_DIR       = ../../build
STR_LIB_DIR     = $(BUILD_DIR)/string
UNITY_LIB_DIR   = $(BUILD_DIR)/unity

STR_LIB   = $(STR_LIB_DIR)/libstring.a
UNITY_LIB = $(UNITY_LIB_DIR)/libunity.a 

STR_TEST_SRC = test_string.c
STR_TEST_OBJ = test_string.o

STR_TEST_BIN = $(STR_BIN_DIR)/test_string

# === Targets ===

.PHONY: strtest

strtest: $(STR_TEST_BIN)

# === Build rules ===

$(STR_TEST_BIN): $(STR_TEST_OBJ)
	$(CC) $(CFLAGS) $< -o $@ -L$(STR_LIB_DIR) -lstring -L$(UNITY_LIB_DIR) -lunity
	rm $(STR_TEST_OBJ)

$(STR_TEST_OBJ) : $(STR_TEST_SRC) $(STR_BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(STR_BIN_DIR):
	mkdir -p $@

