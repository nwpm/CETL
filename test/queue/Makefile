# === Compiler ===
CC     = gcc
CFLAGS = -g -O0 -fsanitize=address -Wall -Wextra -Wpedantic -std=c99

# === Paths ===

INCLUDE         = ../../include
QUEUE_BIN_DIR   = ../../bin/queue
BUILD_DIR       = ../../build
QUEUE_LIB_DIR   = $(BUILD_DIR)/queue
LIST_LIB_DIR    = $(BUILD_DIR)/llist
UNITY_LIB_DIR   = $(BUILD_DIR)/unity

LIST_DIR  = $(LIST_LIB_DIR).libllist.a
QUEUE_LIB = $(QUEUE_LIB_DIR)/libqueue.a
UNITY_LIB = $(UNITY_LIB_DIR)/libunity.a 

QUEUE_TEST_SRC = test_queue.c
QUEUE_TEST_OBJ = test_queue.o

QUEUE_TEST_BIN = $(QUEUE_BIN_DIR)/test_queue

# === Targets ===

.PHONY: qtest

qtest: $(QUEUE_TEST_BIN)

# === Build rules ===

$(QUEUE_TEST_BIN): $(QUEUE_TEST_OBJ)
	$(CC) $(CFLAGS) $< -o $@ -L$(QUEUE_LIB_DIR) -lqueue -L$(UNITY_LIB_DIR) -lunity -L$(LIST_LIB_DIR) -lllist
	rm $(QUEUE_TEST_OBJ)

$(QUEUE_TEST_OBJ) : $(QUEUE_TEST_SRC) $(QUEUE_BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(QUEUE_BIN_DIR):
	mkdir -p $@



