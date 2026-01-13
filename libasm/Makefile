SRC= 
OBJ = $(SRC:.s = .o)
NAME = libasm.a

all:
	nasm -f elf64 -g -F dwarf $(SRC) -o $(OBJ)
	ar rcs $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
