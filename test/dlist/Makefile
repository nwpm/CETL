# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

# === Paths ===

INCLUDE         = ../../include
DLIST_BIN_DIR   = ../../bin/dlist
BUILD_DIR       = ../../build
DLIST_LIB_DIR   = $(BUILD_DIR)/dlist
UNITY_LIB_DIR   = $(BUILD_DIR)/unity

DLIST_LIB   = $(DLIST_LIB_DIR)/libdlist.a
UNITY_LIB = $(UNITY_LIB_DIR)/libunity.a 

DLIST_TEST_SRC = test_dlist.c
DLIST_TEST_OBJ = test_dlist.o

DLIST_TEST_BIN = $(DLIST_BIN_DIR)/test_dlist

# === Targets ===

.PHONY: dltest

dltest: $(DLIST_TEST_BIN)

# === Build rules ===

$(DLIST_TEST_BIN): $(DLIST_TEST_OBJ)
	$(CC) $(CFLAGS) $< -o $@ -L$(DLIST_LIB_DIR) -ldlist -L$(UNITY_LIB_DIR) -lunity
	rm $(DLIST_TEST_OBJ)

$(DLIST_TEST_OBJ) : $(DLIST_TEST_SRC) $(DLIST_BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(DLIST_BIN_DIR):
	mkdir -p $@
