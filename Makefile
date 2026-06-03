INCLUDE_DIR:=Include
OBJ_DIR:=Object
SRC_DIR:=Source
LIB_DIR:=Library

C:=clang
CXX:=clang++
CXXFLAGS:=-std=c++23 -I$(INCLUDE_DIR) -I$(SRC_DIR) -Wmsvc-not-found --target=x86_64-w64-windows-gnu -Wextra -Wall -pedantic
LIBSFLAGS:=$(LIB_DIR)/libglfw3.a -lopengl32 -lgdi32

OUT_FILE:=Binary/medae.exe

CONSOLE_GREEN = $(shell echo [32m)
CONSOLE_RED = $(shell echo [31m)
CONSOLE_CYAN = $(shell echo [36m)
CONSOLE_YELLOW = $(shell echo [33m)
CONSOLE_RESET = $(shell echo [0m)

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
CPP_FILES := $(call rwildcard,$(SRC_DIR)/,*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))

build_glad: $(LIB_DIR)/glad.c
	@$(C) -c $(LIB_DIR)/glad.c -I$(INCLUDE_DIR) -o $(OBJ_DIR)/glad.o -Wmsvc-not-found --target=x86_64-w64-windows-gnu -Wextra -Wall -pedantic -O3

build: $(OBJ_FILES)
	@make build_glad
	@$(CXX) $(CXXFLAGS) $^ $(OBJ_DIR)/glad.o $(LIBSFLAGS) -o $(OUT_FILE)
	@echo ${CONSOLE_CYAN}Program compiled${CONSOLE_RESET}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	@powershell -Command "if (-not (Test-Path '$(dir $@)')) { mkdir '$(dir $@)' }"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo ${CONSOLE_GREEN}Compiled ${CONSOLE_RESET}$@ ${CONSOLE_YELLOW}object file${CONSOLE_RESET}

clean:
	@powershell -Command "if (Test-Path '$(OBJ_DIR)') { rmdir /s /q '$(OBJ_DIR)' }"
	@powershell -Command "if (Test-Path '$(OUT_FILE)') { del '$(OUT_FILE)' }"