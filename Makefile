ifeq ($(OS),Windows_NT)
	MKDIR=powershell -file .\utils\mkdir.ps1
	CMAKE=cmake -G "MinGW Makefiles" -S . -B . -DCMAKE_INSTALL_PREFIX=C:\mingw64\x86_64-w64-mingw32
	RMRF=powershell -file .\utils\rm.ps1
	DIRNAME=Split-Path
	REALPATH=Resolve-Path

	LINKS=-lopengl32 -lglew32 -lm -lgdi32
else
	MKDIR=mkdir -p
	CMAKE=cmake
	RMRF=rm -rf
	DIRNAME=dirname
	REALPATH=realpath

	LINKS=-lGL -lGLEW -lm -ldl -pthread
endif

CC=gcc -std=c17
CFLAGS=-g
CFLAGS_LIB=-c

GLFW_SRC=lib/glfw
GLFW_BUILD_DIR=$(GLFW_SRC)/build
GLFW_LIB=$(GLFW_BUILD_DIR)/src/libglfw3.a

MINIAUDIO_SRC=lib/miniaudio

INCLUDE_DIRS=-I. -I$(GLFW_SRC)/include -Ilib/ -I$(MINIAUDIO_SRC)
LINK_DIRS=
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
	$(MKDIR) "$(GLFW_BUILD_DIR)"
	$(CMAKE) -S $(GLFW_SRC) -B $(GLFW_BUILD_DIR)
	$(MAKE) -C $(GLFW_BUILD_DIR)

$(OBJ_DIR):
	$(MKDIR) "$@"

$(LIB_PART): $(OBJ)
	ar rcs $@ $^

$(LIB): $(LIB_PART) $(GLFW_LIB)
	ar -rcT $@ $^

$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CFLAGS_LIB) $< -o $@ $(INCLUDE_DIRS) $(DEFINE)

full-clean: clean
	$(RMRF) $(GLFW_BUILD_DIR)

clean:
	$(RMRF) $(OBJ_DIR) $(LIB) $(LIB_PART)

includes:
	$(eval LOCAL_INCLUDE_DIRS := . $(GLFW_SRC))
	$(eval JOGO_ABSOLUTE_PATH := $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
	$(eval INCLUDES := $(foreach dir,$(LOCAL_INCLUDE_DIRS),-I$(JOGO_ABSOLUTE_PATH)/$(dir)/include))
	@echo $(INCLUDES)

links:
	@echo $(LINKS)


docs:
	$(MKDIR) "docs/"
	doxygen

clean-docs:
	$(RMRF) docs/{html,man}