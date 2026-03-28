CC = gcc
BUILD_DIR = ./build
COMPILERS = ./Compilers

all: build-chaoslang-compiler
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/CCC main.c

build-chaoslang-compiler:
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/ChaosLangCompiler $(COMPILERS)/ChaosLangCompiler.c