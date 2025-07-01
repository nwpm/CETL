# === PROJECT NAME ===
PROJECT_NAME = CSTL

# === PATHS ===

BUILD_DIR = build/
BIN_DIR   = bin/

UNITY_DIR   = test/Unity
UNITY_BUILD = $(BUILD_DIR)/unity

# === TARGETS ===

.PHONY: all vector string llist stack vtest strtest lltest stest unity clbuild

vector:
	$(MAKE) -C src/vector

string:
	$(MAKE) -C src/string

llist:
	$(MAKE) -C src/llist

stack:
	$(MAKE) -C src/stack

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

stest:
	$(MAKE) -C test/stack

clbuild:
	rm -rf $(BIN_DIR)/*
	find $(BUILD_DIR) -mindepth 1 -maxdepth 1 -not -name 'unity' -exec rm -rf {} +

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*

# ===RULES===

$(UNITY_BUILD):
	mkdir -p $@



