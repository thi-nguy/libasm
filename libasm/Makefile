# ---------- Compiler / Assembler settings -------------------
CC      = gcc
NASM    = nasm
AR      = ar

CC_FLAGS  	= -Wall -Wextra -Werror -g -I$(INC_DIR)
NASM_FLAGS 	= -f elf64 -g -Fdwarf     	# Linux x86-64; change to `macho64  -Fmacho` on macOS, be able to debug with gdp too.
AR_FLAGS 	= rcs               		# r=insert, c=create, s=add symbol index

# ---------- Directories ------------------------------------
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
OBJ_DIR = obj

# ---------- Targets ----------------------------------------
LIB     = $(LIB_DIR)/libasm.a
TEST  = test_libasm

# ---------- Source → Object lists --------------------------
# Every .s in src/ becomes an object in obj/
ASM_SRC = $(wildcard $(SRC_DIR)/*.s)
ASM_OBJ = $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM_SRC))

# ---------- Targets ----------------------------------
all: $(LIB) $(TEST)

$(LIB): $(ASM_OBJ) | $(LIB_DIR)
	$(AR) $(AR_FLAGS) $@ $^
	@echo "✓  Static library built: $@"

$(TEST): main.c $(LIB)
	$(CC) $(CC_FLAGS) $< -L$(LIB_DIR) -lasm -o $@
	@echo "✓  Test built:    $@"

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CC_FLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	@echo "✓  Object folder built."

$(LIB_DIR):
	mkdir -p $(LIB_DIR)
	@echo "✓  Lib folder built."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	$(NASM) $(NASM_FLAGS) $< -o $@
	@echo "✓  Object files built."

test: $(TEST)
	@echo "——— Running tests ———"
	./$(TEST)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TEST)
	@echo "✓  Cleaned."

re: clean all

.PHONY: all test clean re