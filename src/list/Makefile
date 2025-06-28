# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

DEBUG_FLAGS   = -g -O0 -fsanitize=address
RELEASE_FLAGS = -O2

# === Paths ===

BUILD_PATH  = ../../build/list/

LIST_SRC     = linked_list.c
LIST_OBJ     = $(BUILD_PATH)/liblist.o
LIST_TARGET  = $(BUILD_PATH)/liblist.a

# ===Targets===
.PHONY: list install uninstall

list: $(LIST_TARGET) 

install:

uninstall:

# ===Build rules===

$(LIST_TARGET) : $(LIST_OBJ)
	ar rcs $@ $<
	rm $(BUILD_PATH)/$(LIST_OBJ)

$(LIST_OBJ) : $(LIST_SRC) $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_PATH):
	mkdir -p $@

