NAME = ft_printf

INC =	-I./		\
		-I./libft/	\
	
SRC =	./main.c	\
		./libft/*.c	\

${NAME}:
	@gcc -o ${NAME} ${SRC} ${INC}

all: ${NAME}

clean:
	@rm -f *.o

fclean: clean
	@rm -f ${NAME}

re: fclean all
