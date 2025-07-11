# === PROJECT NAME ===
PROJECT_NAME = CSTL

# === PATHS ===

BUILD_DIR = build/
BIN_DIR   = bin/

UNITY_DIR   = test/Unity
UNITY_BUILD = $(BUILD_DIR)/unity

CONTAINER_DIR = src/container

# === TARGETS ===

.PHONY: all vector string llist dlist stack queue vtest strtest lltest stest dltest qtest unity clbuild

vector:
	$(MAKE) -C $(CONTAINER_DIR)/vector

string:
	$(MAKE) -C $(CONTAINER_DIR)/string

llist:
	$(MAKE) -C $(CONTAINER_DIR)/llist

dlist:
	$(MAKE) -C $(CONTAINER_DIR)/dlist

stack:
	$(MAKE) -C $(CONTAINER_DIR)/stack

queue:
	$(MAKE) -C $(CONTAINER_DIR)/queue

unity: $(UNITY_BUILD)
	cd $(UNITY_BUILD) && cmake ../../$(UNITY_DIR)
	cd $(UNITY_BUILD) && make
	find build/unity -mindepth 1 ! -name 'libunity.a' -exec rm -rf {} +

vtest:
	$(MAKE) -C test/vector

strtest:
	$(MAKE) -C test/string

lltest:
	$(MAKE) -C test/llist

dltest:
	$(MAKE) -C test/dlist

stest:
	$(MAKE) -C test/stack

qtest:
	$(MAKE) -C test/queue

clbuild:
	rm -rf $(BIN_DIR)/*
	find $(BUILD_DIR) -mindepth 1 -maxdepth 1 -not -name 'unity' -exec rm -rf {} +

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*

# ===RULES===

$(UNITY_BUILD):
	mkdir -p $@



