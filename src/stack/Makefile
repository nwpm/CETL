# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

DEBUG_FLAGS   = -g -O0 -fsanitize=address
RELEASE_FLAGS = -O2

# === Paths ===

BUILD_PATH  = ../../build/stack/

STACK_SRC     = stack.c
STACK_OBJ     = $(BUILD_PATH)/libstack.o
STACK_TARGET  = $(BUILD_PATH)/libstack.a

# ===Targets===
.PHONY: stack install uninstall

stack: $(STACK_TARGET) 

install:

uninstall:

# ===Build rules===

$(STACK_TARGET) : $(STACK_OBJ)
	ar rcs $@ $<
	rm $(BUILD_PATH)/$(STACK_OBJ)

$(STACK_OBJ) : $(STACK_SRC) $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_PATH):
	mkdir -p $@
