# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=gnu99

# === Paths ===

INCLUDE         = ../../include
LIST_BIN_DIR     = ../../bin/llist
BUILD_DIR       = ../../build
LIST_LIB_DIR     = $(BUILD_DIR)/llist
UNITY_LIB_DIR   = $(BUILD_DIR)/unity

LIST_LIB   = $(LIST_LIB_DIR)/libllist.a
UNITY_LIB = $(UNITY_LIB_DIR)/libunity.a 

LIST_TEST_SRC = test_llist.c
LIST_TEST_OBJ = test_llist.o

LIST_TEST_BIN = $(LIST_BIN_DIR)/test_llist

# === Targets ===

.PHONY: lltest

lltest: $(LIST_TEST_BIN)

# === Build rules ===

$(LIST_TEST_BIN): $(LIST_TEST_OBJ)
	$(CC) $(CFLAGS) $< -o $@ -L$(LIST_LIB_DIR) -lllist -L$(UNITY_LIB_DIR) -lunity
	rm $(LIST_TEST_OBJ)

$(LIST_TEST_OBJ) : $(LIST_TEST_SRC) $(LIST_BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(LIST_BIN_DIR):
	mkdir -p $@


