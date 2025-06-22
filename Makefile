# === PROJECT NAME ===
PROJECT_NAME = CSTL

# === PATHS ===

BUILD_DIR = build/
BIN_DIR   = bin/

UNITY_DIR   = test/Unity
UNITY_BUILD = $(BUILD_DIR)/unity

VEC_SUBDIR = src/vector

# === TARGETS ===

.PHONY: all vector vtest unity clean

vector:
	$(MAKE) -C src/vector

unity: $(UNITY_BUILD)
	cd $(UNITY_BUILD) && cmake ../../$(UNITY_DIR)
	cd $(UNITY_BUILD) && make
	find build/unity -mindepth 1 ! -name 'libunity.a' -exec rm -rf {} +

vtest:
	$(MAKE) -C test/vector

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*

# ===RULES===

$(UNITY_BUILD):
	mkdir -p $@



