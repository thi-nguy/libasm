
NAME = sum64

SRC = $(NAME).s

OBJ = $(SRC:.s=.o)

all: 
	nasm -f elf64 -g -F dwarf $(SRC) -o $(OBJ)
	ld $(OBJ) -o $(NAME) 

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all 

.PHONY: all clean fclean re
