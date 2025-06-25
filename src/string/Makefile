# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

DEBUG_FLAGS   = -g -O0 -fsanitize=address
RELEASE_FLAGS = -O2

# === Paths ===

BUILD_PATH  = ../../build/string/

STR_SRC     = string.c
STR_OBJ     = $(BUILD_PATH)/libstring.o
STR_TARGET  = $(BUILD_PATH)/libstring.a

# ===Targets===
.PHONY: string install uninstall clean

string: $(STR_TARGET) 

install:

uninstall:

clean :
	rm -rf $(BUILD_PATH)/*

# ===Build rules===

$(STR_TARGET) : $(STR_OBJ)
	ar rcs $@ $<
	rm $(BUILD_PATH)/$(STR_OBJ)

$(STR_OBJ) : $(STR_SRC) $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_PATH):
	mkdir -p $@
