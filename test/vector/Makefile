# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -pedantic -std=gnu99

# === Paths ===

INCLUDE         = ../../include
VEC_BIN_DIR     = ../../bin/vector
BUILD_DIR       = ../../build
VEC_LIB_DIR     = $(BUILD_DIR)/vector
UNITY_LIB_DIR   = $(BUILD_DIR)/unity

VEC_LIB   = $(VEC_LIB_DIR)/libvector.a
UNITY_LIB = $(UNITY_LIB_DIR)/libunity.a 

VEC_TEST_SRC = test_vector.c
VEC_TEST_OBJ = test_vector.o

VEC_TEST_BIN = $(VEC_BIN_DIR)/test_vector

# === Targets ===

.PHONY: vtest

vtest: $(VEC_TEST_BIN)

# === Build rules ===

$(VEC_TEST_BIN): $(VEC_TEST_OBJ)
	$(CC) $(CFLAGS) $< -o $@ -L$(VEC_LIB_DIR) -lvector -L$(UNITY_LIB_DIR) -lunity
	rm $(VEC_TEST_OBJ)


$(VEC_TEST_OBJ) : $(VEC_TEST_SRC) $(VEC_BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(VEC_BIN_DIR):
	mkdir -p $@
