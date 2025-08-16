# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

# === Paths ===

BUILD_PATH  = ../../../build/llist/

LIST_SRC     = llist.c
LIST_OBJ     = $(BUILD_PATH)/libllist.o
LIST_TARGET  = $(BUILD_PATH)/libllist.a

# ===Targets===
.PHONY: llist install uninstall

llist: $(LIST_TARGET) 

install:

uninstall:

# ===Build rules===

$(LIST_TARGET) : $(LIST_OBJ)
	ar rcs $@ $<
	rm $(LIST_OBJ)

$(LIST_OBJ) : $(LIST_SRC) $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_PATH):
	mkdir -p $@

