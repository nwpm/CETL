# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

# === Paths ===

BUILD_PATH  = ../../../build/queue/

QUEUE_SRC     = queue.c
QUEUE_OBJ     = $(BUILD_PATH)/libqueue.o
QUEUE_TARGET  = $(BUILD_PATH)/libqueue.a

# ===Targets===
.PHONY: queue install uninstall

queue: $(QUEUE_TARGET) 

install:

uninstall:

# ===Build rules===

$(QUEUE_TARGET) : $(QUEUE_OBJ)
	ar rcs $@ $<
	rm $(QUEUE_OBJ)

$(QUEUE_OBJ) : $(QUEUE_SRC) $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_PATH):
	mkdir -p $@


