# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

DEBUG_FLAGS   = -g -O0 -fsanitize=address
RELEASE_FLAGS = -O2

# === Paths ===

BUILD_PATH  = ../../build/dlist/

DLIST_SRC     = dlist.c
DLIST_OBJ     = $(BUILD_PATH)/libdlist.o
DLIST_TARGET  = $(BUILD_PATH)/libdlist.a

# ===Targets===
.PHONY: dlist install uninstall

dlist: $(DLIST_TARGET) 

install:

uninstall:

# ===Build rules===

$(DLIST_TARGET) : $(DLIST_OBJ)
	ar rcs $@ $<
	rm $(BUILD_PATH)/$(DLIST_OBJ)

$(DLIST_OBJ) : $(DLIST_SRC) $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_PATH):
	mkdir -p $@
