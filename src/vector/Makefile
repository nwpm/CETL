# === Compiler ===
CC     = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99

DEBUG_FLAGS   = -g -O0 -fsanitize=address
RELEASE_FLAGS = -O2

# === Paths ===

BUILD_PATH  = ../../build/vector/

VEC_SRC     = vector.c
VEC_OBJ     = $(BUILD_PATH)/libvector.o
VEC_TARGET  = $(BUILD_PATH)/libvector.a

# ===Targets===
.PHONY: vector install uninstall clean

vector: $(VEC_TARGET) 

install:

uninstall:

clean :
	rm -rf $(BUILD_PATH)/*

# ===Build rules===

$(VEC_TARGET) : $(VEC_OBJ)
	ar rcs $@ $<
	rm $(BUILD_PATH)/$(VEC_OBJ)

$(VEC_OBJ) : $(VEC_SRC) $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_PATH):
	mkdir -p $@










