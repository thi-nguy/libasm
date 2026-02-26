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
NAME  = $(LIB_DIR)/libasm.a
TEST  = test_libasm
DEBUG = debug_libasm

# ---------- Source → Object lists --------------------------
# Every .s in src/ becomes an object in obj/
ASM_SRC = $(wildcard $(SRC_DIR)/*.s)
ASM_OBJ = $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM_SRC))

# ---------- Targets ----------------------------------
all: $(NAME) $(TEST)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	@echo "✓  Folder built: $@."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	$(NASM) $(NASM_FLAGS) $< -o $@
	@echo "✓  Object files built."

$(LIB_DIR):
	mkdir -p $(LIB_DIR)
	@echo "✓  Folder built: $@."

$(NAME): $(ASM_OBJ) | $(LIB_DIR)
	$(AR) $(AR_FLAGS) $@ $^
	@echo "✓  Static library built: $@."

main.o: main.c
	$(CC) $(CC_FLAGS) -c $< -o $@
	@echo "✓  $@ built."

$(TEST): main.o $(NAME)
	$(CC) $(CC_FLAGS) main.o -L./$(LIB_DIR) -lasm -o $(TEST)
	@echo "✓  Test built:    $@."

debug: $(NAME) $(DEBUG)
main-debug.c: main-debug.o
	$(CC) $(CC_FLAGS) -c $< -o $@
	@echo "✓  $@ built."
$(DEBUG): main-debug.o $(NAME)
	$(CC) $(CC_FLAGS) main-debug.o -L./$(LIB_DIR) -lasm -o $(DEBUG)
	@echo "✓  Debug built:    $@."



test: $(TEST)
	@echo "——— Running tests ———"
	./$(TEST)

clean:
	rm -rf $(OBJ_DIR) main.o main-debug.o
	@echo "✓  Object files cleaned."

fclean: clean
	rm -rf $(LIB_DIR) $(TEST) $(DEBUG)
	@echo "✓  Everything cleaned."

re: clean all


.PHONY: all test clean fclean re