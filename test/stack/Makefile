# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

# === Paths ===

INCLUDE         = ../../include
STACK_BIN_DIR   = ../../bin/stack
BUILD_DIR       = ../../build
STACK_LIB_DIR   = $(BUILD_DIR)/stack
UNITY_LIB_DIR   = $(BUILD_DIR)/unity
LIST_LIB_DIR    = $(BUILD_DIR)/llist

STACK_LIB = $(STACK_LIB_DIR)/libstack.a
UNITY_LIB = $(UNITY_LIB_DIR)/libunity.a 
LIST_LIB  = $(LIST_LIB_DIR)/libllist.a

STACK_TEST_SRC = test_stack.c
STACK_TEST_OBJ = test_stack.o

STACK_TEST_BIN = $(STACK_BIN_DIR)/test_stack

# === Targets ===

.PHONY: stest

stest: $(STACK_TEST_BIN)

# === Build rules ===

$(STACK_TEST_BIN): $(STACK_TEST_OBJ)
	$(CC) $(CFLAGS) $< -o $@ -L$(STACK_LIB_DIR) -lstack -L$(UNITY_LIB_DIR) -lunity -L$(LIST_LIB_DIR) -lllist
	rm $(STACK_TEST_OBJ)

$(STACK_TEST_OBJ) : $(STACK_TEST_SRC) $(STACK_BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(STACK_BIN_DIR):
	mkdir -p $@



