CC=gcc -std=c17
CFLAGS=-g
CFLAGS_LIB=-c

GLFW_SRC=lib/glfw
GLFW_BUILD_DIR=$(GLFW_SRC)/build
GLFW_LIB=$(GLFW_BUILD_DIR)/src/libglfw3.a

MINIAUDIO_SRC=lib/miniaudio

INCLUDE_DIRS=-I. -I$(GLFW_SRC)/include -Ilib/ -I$(MINIAUDIO_SRC)
LINK_DIRS=
LINKS=-lGL -lGLEW -lm -ldl -pthread
DEFINE=

SRC_DIR=src
SRC=$(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
OBJ_DIR=obj
OBJ=$(foreach src,$(SRC),$(OBJ_DIR)/$(notdir $(basename $(src))).o)

LIB=libjogo.a
LIB_PART=libjogo-part.a

.PHONY: includes links docs
VPATH=$(wildcard $(SRC_DIR)/*) $(dir GLFW_LIB)

all: $(GLFW_LIB) $(OBJ_DIR) $(LIB)

$(GLFW_LIB):
	mkdir -p $(GLFW_BUILD_DIR)
	cmake -S $(GLFW_SRC) -B $(GLFW_BUILD_DIR)
	$(MAKE) -C $(GLFW_BUILD_DIR)

$(OBJ_DIR):
	mkdir -p $@

$(LIB_PART): $(OBJ)
	ar rcs $@ $^

$(LIB): $(LIB_PART) $(GLFW_LIB)
	ar -rcT $@ $^

$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CFLAGS_LIB) $< -o $@ $(INCLUDE_DIRS) $(DEFINE)

full-clean: clean
	rm -rf $(GLFW_BUILD_DIR)

clean:
	rm -rf $(OBJ_DIR) $(LIB)

includes:
	$(eval LOCAL_INCLUDE_DIRS := . $(GLFW_SRC))
	$(eval JOGO_ABSOLUTE_PATH := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))")
	$(eval INCLUDES := $(foreach dir,$(LOCAL_INCLUDE_DIRS),-I$(JOGO_ABSOLUTE_PATH)/$(dir)/include))
	@echo "$(INCLUDES)"

links:
	@echo "$(LINKS)"


docs:
	mkdir -p docs/
	doxygen

clean-docs:
	rm -rf docs/{html,man}