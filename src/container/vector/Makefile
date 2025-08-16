# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

# === Paths ===

BUILD_PATH  = ../../../build/vector

VEC_SRC     = vector.c
VEC_OBJ     = $(BUILD_PATH)/libvector.o
VEC_TARGET  = $(BUILD_PATH)/libvector.a

# ===Targets===
.PHONY: vector install uninstall

vector: $(VEC_TARGET) 

install:

uninstall:

# ===Build rules===

$(VEC_TARGET) : $(VEC_OBJ)
	ar rcs $@ $<
	rm $(VEC_OBJ)

$(VEC_OBJ) : $(VEC_SRC) $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_PATH):
	mkdir -p $@
